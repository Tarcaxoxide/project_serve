#include<Command.hpp>


namespace Shell{
    Command_st* Test_Body(Command_st* Caller){
        Caller->ReturnString=std::string("Test_Body, Arguments provided [");
        for(size_t i=(Caller->ArgumentIndex);i<(*Caller->Arguments).size();i++){
            Caller->ReturnString+=std::string("(")+std::to_string(i)+std::string(")");
            Caller->ReturnString+=(*Caller->Arguments)[i]+std::string((i<(*Caller->Arguments).size()-1)? "," : "");
        }
        Caller->ReturnString+=std::string("]");
        return Caller;
    }
    Command_st* Filesystem_Create_Body(Command_st* Caller){
        std::string Path=(*Caller->Arguments)[(Caller->ArgumentIndex)];
        std::deque<std::string> TokenizedPath=Format::split(Path,"/");
        if(TokenizedPath[TokenizedPath.size()-1].size() < 1){
            Filesystem::Folder_st* Folder = Filesystem::FilesystemManager.FolderCreate(Path);
            if(Folder == nullptr){
                Caller->ReturnString=std::string("Failed to create folder ")+Path;
            }else{
                Caller->ReturnString=std::string("Created folder ")+Path;
                Caller->ReturnPtr=(void*)Folder;
            }
        }else{
            Filesystem::File_st* File = Filesystem::FilesystemManager.FileCreate(Path);
            if(File == nullptr){
                Caller->ReturnString=std::string("Failed to create file ")+Path;
            }else{
                Caller->ReturnString=std::string("Created file ")+Path;
                Caller->ReturnPtr=(void*)File;
            }
        }
        return Caller;
    }
    Command_st* Filesystem_Find_Body(Command_st* Caller){
        std::string Path=(*Caller->Arguments)[(Caller->ArgumentIndex)];
        std::deque<std::string> TokenizedPath=Format::split(Path,"/");
        
        if(TokenizedPath[TokenizedPath.size()-1].size() < 1){
            Filesystem::Folder_st* Folder = Filesystem::FilesystemManager.FolderSearch(Path);
            if(Folder == nullptr){
                Caller->ReturnString=std::string("Not found");
                Caller->ReturnPtr=(void*)Folder;
            }else{
                Caller->ReturnString=std::string("Folder found");
            }
        }else{
            Filesystem::File_st* File = Filesystem::FilesystemManager.FileSearch(Path);
            if(File == nullptr){
                Caller->ReturnString=std::string("Not found");
                Caller->ReturnPtr=(void*)File;
            }else{
                Caller->ReturnString=std::string("File found");
            }
        }
        return Caller;
    }
    Command_st* Filesystem_Delete_Body(Command_st* Caller){
        std::string Path=(*Caller->Arguments)[(Caller->ArgumentIndex)];
        std::deque<std::string> TokenizedPath=Format::split(Path,"/");
        if(TokenizedPath[TokenizedPath.size()-1].size() < 1){
            std::string TargetPath="";
            for(size_t i=0;i<TokenizedPath.size()-2;i++){
                TargetPath+=TokenizedPath[i]+std::string((i<TokenizedPath.size()-3)? "/" : "");
            }
            Filesystem::Folder_st* Folder = Filesystem::FilesystemManager.FolderSearch(TargetPath);
            if(Folder->DeleteFolder(TokenizedPath[TokenizedPath.size()-2])){
                Caller->ReturnString=std::string("Folder deleted");
            }else{
                Caller->ReturnString=std::string("Folder not deleted");
            }
        }else{
            Filesystem::Folder_st* Folder = Filesystem::FilesystemManager.FolderSearch(Path);
            if(Folder->DeleteFile(TokenizedPath[TokenizedPath.size()-1])){
                Caller->ReturnString=std::string("File deleted");
            }else{
                Caller->ReturnString=std::string("File not deleted");
            }

        }
        return Caller;
    }
    Command_st* Filesystem_Get_Body(Command_st* Caller){
        //Filesystem_Get_Body, Arguments provided [(2)test.txt,(3)ContentType]
        if((*Caller->Arguments).size() == 3){
            Caller->ReturnString="Invalid amount of arguments: `Filesystem Get [File path] [Property]`";
            return Caller;
        }
        std::string Path=(*Caller->Arguments)[2];
        std::string Property=(*Caller->Arguments)[3];

        Filesystem::File_st* File = Filesystem::FilesystemManager.FileSearch(Path);
        if(File == nullptr){
            Caller->ReturnString=std::string("File Not found");
            Caller->ReturnPtr=(void*)File;
        }else{
            //Caller->ReturnString=std::string("File found");
            if(Property == "Content"){
                Caller->ReturnString=File->Contents;
            }
            else if(Property == "Type"){
                Caller->ReturnString=File->ContentType;
            }
            else{
                Caller->ReturnString="Unknown Property";
            }
        }
        return Caller;
    }
    Command_st* Filesystem_Set_Body(Command_st* Caller){
        //Filesystem_Get_Body, Arguments provided [(2)test.txt,(3)ContentType]
        if((*Caller->Arguments).size() < 4){
            Caller->ReturnString="Invalid amount of arguments: `Filesystem Set [File path] [Property] [Value]`";
            return Caller;
        }
        std::string Path=(*Caller->Arguments)[2];
        std::string Property=(*Caller->Arguments)[3];
        std::string PropertyValue;
        for(size_t i=4;i<(*Caller->Arguments).size();i++){
            PropertyValue+=(*Caller->Arguments)[i]+std::string((i<(*Caller->Arguments).size()-1)? " " : "");
        }
        Filesystem::File_st* File = Filesystem::FilesystemManager.FileSearch(Path);
        if(File == nullptr){
            Caller->ReturnString=std::string("File Not found");
            Caller->ReturnPtr=(void*)File;
        }else{
            //Caller->ReturnString=std::string("File found");
            if(Property == "Content"){
                Caller->ReturnString=File->Contents;
                File->Contents=PropertyValue;
                Caller->ReturnString=Property+std::string(" Set");
            }
            else if(Property == "Type"){
                File->ContentType=PropertyValue;
                Caller->ReturnString=Property+std::string(" Set");
            }
            else{
                Caller->ReturnString="Unknown Property";
            }
        }
        return Caller;
    }
    //Command_st* Filesystem_Save_Body(Command_st* Caller){}
    //Command_st* Filesystem_Load_Body(Command_st* Caller){}
};
namespace Shell{
    Command_st* Command_st::BaseCommandReference=nullptr;
    Command_st::Command_st(std::string CommandString,std::deque<Command_st*> SubCommands){
        if(BaseCommandReference == nullptr)BaseCommandReference=this;
        this->CommandString=CommandString;
        ExecutableBody=nullptr;
    }
    Command_st::Command_st(std::string CommandString,Command_st* (*ExecutableBody)(Command_st* Caller)){
        if(BaseCommandReference == nullptr)BaseCommandReference=this;
        this->CommandString=CommandString;
        this->ExecutableBody=ExecutableBody;
    }
    Command_st::Command_st(std::string CommandString){
        if(BaseCommandReference == nullptr)BaseCommandReference=this;
        ExecutableBody=nullptr;
        this->CommandString=CommandString;
    }
    Command_st::~Command_st(){
        for(size_t i=0;i<SubCommands.size();i++){
            delete[] SubCommands[i];
        }
        SubCommands.clear();
    }
    Command_st* Command_st::operator()(size_t ArgumentIndex,std::deque<std::string>* Arguments){
        ReturnPtr=(void*)nullptr;
        ReturnString="";
        this->ArgumentIndex=ArgumentIndex;
        this->Arguments=Arguments;
        std::string Argument = (*Arguments)[ArgumentIndex];
        if(ExecutableBody != nullptr){
            Command_st* Return = (*ExecutableBody)(this);
            Return->Arguments->clear();
            return Return;
        }else{
            for(size_t i=0;i<SubCommands.size();i++){
                if(SubCommands[i]->CommandString == Argument)return SubCommands[i];
            }
        }
        return nullptr;
    }
    Command_st* Command_st::AddSubCommand(Command_st* nCommand){
        SubCommands.push_back(nCommand);
        return nCommand;
    }
    Command_st BaseCommand("BaseCommand");
};

namespace Shell{
    void Initialize(){
        Command_st* Command_Test=BaseCommand.AddSubCommand(new Command_st("Test",Test_Body));
        Command_st* Command_Filesystem=BaseCommand.AddSubCommand(new Command_st("Filesystem"));
        Command_st* Command_Filesystem_Create=Command_Filesystem->AddSubCommand(new Command_st("Create",Filesystem_Create_Body));
        Command_st* Command_Filesystem_Find=Command_Filesystem->AddSubCommand(new Command_st("Find",Filesystem_Find_Body));
        Command_st* Command_Filesystem_Delete=Command_Filesystem->AddSubCommand(new Command_st("Delete",Filesystem_Delete_Body));
        Command_st* Command_Filesystem_Get=Command_Filesystem->AddSubCommand(new Command_st("Get",Filesystem_Get_Body));
        Command_st* Command_Filesystem_Set=Command_Filesystem->AddSubCommand(new Command_st("Set",Filesystem_Set_Body));
    }
    std::string Command(std::deque<std::string> args,bool& KeepGoing){
        Command_st* PreviousCommand=(Command_st*)nullptr;
        Command_st* CurrentCommand=(Command_st*)&BaseCommand;
        for(size_t i=0;i<args.size()+1;i++){
            if(args.size() == i){CurrentCommand=(*CurrentCommand)(i-1,&args);}else{CurrentCommand=(*CurrentCommand)(i,&args);}
            if(CurrentCommand == nullptr || PreviousCommand == CurrentCommand){
                break;
            }
            PreviousCommand=CurrentCommand;
        }
        return ((CurrentCommand == nullptr) ? ((PreviousCommand == nullptr)? "Unknown command." : "Unknown subcommand.") : CurrentCommand->ReturnString);
    }
};