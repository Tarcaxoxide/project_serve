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
    Command_st* Help_Body(Command_st* Caller){
        if((*Caller->Arguments).size() == 1){
            for(size_t iBase=0;iBase<Caller->BaseCommandReference->SubCommands.size();iBase++){
                Caller->ReturnString+=Caller->BaseCommandReference->SubCommands[iBase]->CommandString+std::string(" ")+Caller->BaseCommandReference->SubCommands[iBase]->HelpString+std::string((iBase<Caller->BaseCommandReference->SubCommands.size()-1)? "\n" : "");
            }
        }else{
            std::deque<std::string> TokenizedArgument;
            for(size_t i=(Caller->ArgumentIndex);i<(*Caller->Arguments).size();i++){
                TokenizedArgument.push_back((*Caller->Arguments)[i]);
            }
            Command_st* CurrentCommand=Caller->BaseCommandReference;
            Command_st* PreviousCommand;
            for(size_t ia=0;ia<TokenizedArgument.size();ia++){
                PreviousCommand=CurrentCommand;
                for(size_t ib=0;ib<CurrentCommand->SubCommands.size();ib++){
                    if(CurrentCommand->SubCommands[ib]->CommandString == TokenizedArgument[ia]){
                        CurrentCommand=CurrentCommand->SubCommands[ib];
                    }
                }
                if(CurrentCommand == PreviousCommand){
                    Caller->ReturnString=std::string("Sub command or argument not found");
                    return Caller;
                }
            }
            Caller->ReturnString=CurrentCommand->HelpString;
        }
        return Caller;
    }
    Command_st* Filesystem_Create_Body(Command_st* Caller){
        if((*Caller->Arguments).size() != 3){
            Caller->ReturnString="Invalid amount of arguments: `Filesystem Create [File_Path]`";
            return Caller;
        }
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
        if((*Caller->Arguments).size() != 3){
            Caller->ReturnString="Invalid amount of arguments: `Filesystem Find [File_Path]`";
            return Caller;
        }
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
        if((*Caller->Arguments).size() != 3){
            Caller->ReturnString="Invalid amount of arguments: `Filesystem Delete [File_Path]`";
            return Caller;
        }
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
        if((*Caller->Arguments).size() != 4){
            Caller->ReturnString="Invalid amount of arguments: `Filesystem Get [File_Path] [Property(`Content`,`Type`)]`";
            return Caller;
        }
        std::string Path=(*Caller->Arguments)[2];
        std::string Property=(*Caller->Arguments)[3];

        Filesystem::File_st* File = Filesystem::FilesystemManager.FileSearch(Path);
        if(File == nullptr){
            Caller->ReturnString=std::string("File Not found");
            Caller->ReturnPtr=(void*)File;
        }else{
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
        if((*Caller->Arguments).size() < 4){
            Caller->ReturnString="Invalid amount of arguments: `Filesystem Set [File_Path] [Property(`Content`,`Type`)] [Value]`";
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
    Command_st* Filesystem_Load_Body(Command_st* Caller){
        if((*Caller->Arguments).size() != 4){
            Caller->ReturnString="Invalid amount of arguments: `Filesystem Load [File_Path] [Real_File_Path]`";
            return Caller;
        }
        std::string Path=(*Caller->Arguments)[2];
        std::string Real_File_Path=(*Caller->Arguments)[3];

        Filesystem::File_st* File = Filesystem::FilesystemManager.FileSearch(Path);
        if(File == nullptr){Caller->ReturnString=std::string("Virtual file doesn't exist");return Caller;}
        
        std::ifstream RealFile(Real_File_Path);
        if (RealFile.fail()){Caller->ReturnString=std::string("Real file doesn't exist");return Caller;}
        std::string AllLines="";
        for(std::string Line="";std::getline(RealFile,Line);){
            AllLines+=Line+'\n';
        }
        File->Contents=AllLines;

        RealFile.close();
        return Caller;
    }
    Command_st* Variable_Set_Body(Command_st* Caller){
        if((*Caller->Arguments).size() != 4){
            Caller->ReturnString="incorrect amount of arguments. `Variable Set [variable name] [variable value]`";
        }
        std::string VariableName=(*Caller->Arguments)[2];
        std::string VariableValue=(*Caller->Arguments)[3];

        int64_t iVariableValue=0;
        try {
            iVariableValue=std::stoull(VariableValue);
        }catch(const std::exception& e){
            Caller->ReturnString=std::string("variable value to set is incorrect");
            return Caller;
        }

        Variables::VariableManager.SetVariable(VariableName,iVariableValue);
        Caller->ReturnString=std::string("Variable set");
        return Caller;
    }
    Command_st* Variable_Get_Body(Command_st* Caller){
        if((*Caller->Arguments).size() != 3){
            Caller->ReturnString="incorrect amount of arguments. `Variable Get [variable name]`";
        }
        std::string VariableName=(*Caller->Arguments)[2];
        int64_t iVariableValue;

        int64_t index=Variables::VariableManager.GetVariable(VariableName,iVariableValue);
        if(index == 0){
            Caller->ReturnString=std::string("Variable doesn't exist, please Set it");
        }else{
            Caller->ReturnString=std::to_string(iVariableValue);
        }
        return Caller;
    }
};
namespace Shell{
    Command_st* Command_st::BaseCommandReference=nullptr;
    Command_st::Command_st(std::string CommandString,std::deque<Command_st*> SubCommands,std::string HelpString){
        if(BaseCommandReference == nullptr)BaseCommandReference=this;
        ExecutableBody=nullptr;
        this->CommandString=CommandString;
        this->HelpString=HelpString;
    }
    Command_st::Command_st(std::string CommandString,Command_st* (*ExecutableBody)(Command_st* Caller),std::string HelpString){
        if(BaseCommandReference == nullptr)BaseCommandReference=this;
        this->CommandString=CommandString;
        this->ExecutableBody=ExecutableBody;
        this->HelpString=HelpString;
    }
    Command_st::Command_st(std::string CommandString,std::string HelpString){
        if(BaseCommandReference == nullptr)BaseCommandReference=this;
        ExecutableBody=nullptr;
        this->CommandString=CommandString;
        this->HelpString=HelpString;
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
    Command_st BaseCommand("BaseCommand","(?this shouldn't ever be activated?)");
};

namespace Shell{
    void Initialize(){
        Command_st* Command_Test=BaseCommand.AddSubCommand(new Command_st("Test",Test_Body,"(Test; just a test,takes no arguments)"));
        Command_st* Command_Help=BaseCommand.AddSubCommand(new Command_st("Help",Help_Body,"(Help [SubCommand]; takes 1 optional sub command that is the command to get help for)"));
        Command_st* Command_Filesystem=BaseCommand.AddSubCommand(new Command_st("Filesystem","(Filesystem [SubCommand]; Virtual filesystem in ram only, takes 1 sub command)"));
        Command_st* Command_Filesystem_Create=Command_Filesystem->AddSubCommand(new Command_st("Create",Filesystem_Create_Body,"(Create [Path]; creates files and folders depending on the path provided)"));
        Command_st* Command_Filesystem_Find=Command_Filesystem->AddSubCommand(new Command_st("Find",Filesystem_Find_Body,"(Find [Path]; Finds the file or folder depending on the path provided)"));
        Command_st* Command_Filesystem_Delete=Command_Filesystem->AddSubCommand(new Command_st("Delete",Filesystem_Delete_Body,"(Delete [Path]; Deletes the file or folder depending on the path provided)"));
        Command_st* Command_Filesystem_Get=Command_Filesystem->AddSubCommand(new Command_st("Get",Filesystem_Get_Body,"(Get [File_Path] [Property]; Gets the specified property(Content,Type) of the file)"));
        Command_st* Command_Filesystem_Set=Command_Filesystem->AddSubCommand(new Command_st("Set",Filesystem_Set_Body,"(Set [File_Path] [Property] [Value]; Sets the specified property(Content,Type) of the file)"));
        Command_st* Command_Filesystem_Load=Command_Filesystem->AddSubCommand(new Command_st("Load",Filesystem_Load_Body,"(Load [File_Path] [Real_File_Path]; loads the contents of a real file into the virtual file"));

        Command_st* Command_Variable=BaseCommand.AddSubCommand(new Command_st("Variable","(Variable [SubCommand]; Takes 1 sub command)"));
        Command_st* Command_Variable_Set=Command_Variable->AddSubCommand(new Command_st("Set",Variable_Set_Body,"(Set [Variable_Name] [Value]; Sets a variable to the value provided,the value must be a number(+ or -,64bit))"));
        Command_st* Command_Variable_Get=Command_Variable->AddSubCommand(new Command_st("Get",Variable_Get_Body,"(Get [Variable_Name]; Gets the variable value and displays it)"));
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