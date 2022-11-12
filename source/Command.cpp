#include<Command.hpp>



namespace Shell{
    Command_st* COMMAND_Activitypub_Create(Command_st* Caller){
        //Activitypub_Create [Identifier type]
        size_t iIT=0;
        for(size_t i=0;i<Format::Activitypub::ObjectDefinitions::Identifier_Type::IT_SIZE;i++){
            if((*Caller->Arguments)[1] == Format::Activitypub::ObjectDefinitions::Identifier_TypeString[i])iIT=i;
        }
        Format::Activitypub::ObjectDefinitions::Identifier_Type IT=(Format::Activitypub::ObjectDefinitions::Identifier_Type)iIT;
        switch(IT){
            case Format::Activitypub::ObjectDefinitions::Identifier_Type::IT_PERSON:{
                Format::Activitypub::ObjectDefinitions::Actor_st Actor(Settings::URL,(*Caller->Arguments)[2],{Format::Activitypub::ObjectDefinitions::Identifier_Type::IT_PERSON,Format::Activitypub::ObjectDefinitions::Object_st::idNumber});
                std::string DirToMake=std::string("Article");mkdir(DirToMake.c_str(),0777);
                DirToMake+=std::string("/")+Format::NumberToHex(IT);mkdir(DirToMake.c_str(),0777);
                DirToMake+=std::string("/")+Format::NumberToHex(Format::Activitypub::ObjectDefinitions::Object_st::idNumber);mkdir(DirToMake.c_str(),0777);
                mkdir(std::string(DirToMake+std::string("/inbox")).c_str(),0777);
                
                mkdir(std::string(DirToMake+std::string("/outbox")).c_str(),0777);
                std::ofstream Profile(std::string(DirToMake+std::string("/Profile.json")).c_str());
                Profile<<Actor.Actor_Json()<<std::endl;
                Profile.close();
                //write file `Article/Person/(Format::Activitypub::ObjectDefinitions::Identifier_Type::Object_st::idNumber)/Profile.json`
                Format::Activitypub::ObjectDefinitions::Object_st::idNumber++;
                std::ofstream MainRecord(std::string(std::string("Article/MainRecord")).c_str());
                MainRecord<<(*Caller->Arguments)[2]<<std::endl;
                MainRecord<<DirToMake<<std::endl;
                MainRecord.close();
                Caller->ReturnString=Actor.Actor_Json();
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