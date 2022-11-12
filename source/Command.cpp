#include<Command.hpp>



namespace Shell{
    Command_st* COMMAND_Activitypub_Create(Command_st* Caller){
        //Activitypub_Create [Identifier type]
        std::string Local_Profile_Location="",UserName=(*Caller->Arguments)[2];
        std::ifstream MainRecord;
        MainRecord.open("Article/MainRecord");
        if(MainRecord.is_open()){
            std::string line;
            size_t iline=0;
            bool usernameFound=false;
            while(getline(MainRecord, line)) {
                std::cout<<iline<<":"<<((iline%2)? "location" : "username")<<":"<<line<<std::endl;

                if(usernameFound){
                    Local_Profile_Location=line;
                }
                usernameFound=(!(iline%2) && line == UserName);
                iline++;
            }
            MainRecord.close();
        }
        size_t iIT=0;
        for(size_t i=0;i<Format::Activitypub::ObjectDefinitions::Identifier_Type::IT_SIZE;i++){
            if((*Caller->Arguments)[1] == Format::Activitypub::ObjectDefinitions::Identifier_TypeString[i])iIT=i;
        }
        Format::Activitypub::ObjectDefinitions::Identifier_Type IT=(Format::Activitypub::ObjectDefinitions::Identifier_Type)iIT;
        switch(IT){
            case Format::Activitypub::ObjectDefinitions::Identifier_Type::IT_PERSON:{
                //[1] == Type;
                //[2] == UserName;
                //[3] == NULL; <- group name if multiple (groups will be implemented later)
                //[4] == NULL;
                //[5] == NULL;
                if(Local_Profile_Location != ""){
                    Caller->ReturnString=std::string("Already exists.");
                    return Caller;
                }
                std::string DirToMake=std::string("Article");mkdir(DirToMake.c_str(),0777);
                DirToMake+=std::string("/")+Format::NumberToHex(IT);mkdir(DirToMake.c_str(),0777);
                std::deque<std::string> d = Format::listDirectories(DirToMake);
                Format::Activitypub::ObjectDefinitions::Actor_st Actor(Settings::URL,UserName,{Format::Activitypub::ObjectDefinitions::Identifier_Type::IT_PERSON,d.size()+1});
                
                DirToMake+=std::string("/")+Format::NumberToHex(d.size()+1);mkdir(DirToMake.c_str(),0777);
                mkdir(std::string(DirToMake+std::string("/inbox")).c_str(),0777);
                
                mkdir(std::string(DirToMake+std::string("/outbox")).c_str(),0777);
                std::ofstream Profile(std::string(DirToMake+std::string("/Profile.json")).c_str());
                Profile<<Actor.Actor_Json()<<std::endl;
                Profile.close();
                //write file `Article/Person/(Format::Activitypub::ObjectDefinitions::Identifier_Type::Object_st::idNumber)/Profile.json`
                std::ofstream MainRecord(std::string(std::string("Article/MainRecord")).c_str(), std::ios::out | std::ios::app);
                MainRecord<<UserName<<std::endl;
                MainRecord<<DirToMake<<std::endl;
                MainRecord.close();
                Caller->ReturnString=Actor.Actor_Json();
            }break;
            case Format::Activitypub::ObjectDefinitions::Identifier_Type::IT_NOTE:{
                //[1] == Type;
                //[2] == Author_UserName;
                //[3] == Audience_UserName; <- group name if multiple (groups will be implemented later)
                //[4] == name of the note;
                //[5+] == the content of the note;
                std::string DirToMake=std::string("Article");mkdir(DirToMake.c_str(),0777);
                DirToMake+=std::string("/")+Format::NumberToHex(IT);mkdir(DirToMake.c_str(),0777);
                std::deque<std::string> d = Format::listDirectories(DirToMake);
                std::string to=(*Caller->Arguments)[3];
                std::string name=(*Caller->Arguments)[3];
                std::string content;
                for(size_t i=5;i<(*Caller->Arguments).size();i++){
                    content+=(*Caller->Arguments)[i];
                    if(i<(*Caller->Arguments).size()-1)content+=std::string(" ");
                }
                DirToMake+=std::string("/")+Format::NumberToHex(d.size()+1);mkdir(DirToMake.c_str(),0777);
                Format::Activitypub::ObjectDefinitions::Note_st Note(Settings::URL,name,content,Format::currentDateAndTime(),UserName,to,{Format::Activitypub::ObjectDefinitions::Identifier_Type::IT_NOTE,d.size()+1});
                std::ofstream NoteFile(std::string(DirToMake+std::string("/Note.json")).c_str());
                NoteFile<<Note.Note_Json()<<std::endl;
                NoteFile.close();
            }break;
            default:{
                Caller->ReturnString=std::string("nothing created.");
            }break;
        };
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
        Command_st* _Activitypub_Create = BaseCommand.AddSubCommand(new Command_st("Activitypub_Create",COMMAND_Activitypub_Create,"(Activitypub_Create)"));
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
        return ((CurrentCommand == nullptr) ? ((PreviousCommand == nullptr)? "Unknown command." : "Uknown Subcommand") : CurrentCommand->ReturnString);
    }
};