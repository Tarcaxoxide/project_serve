#include<Command.hpp>


namespace Shell{
    Command_st* Command_st::BaseCommandReference=nullptr;
    Database::Manager_cl DatabaseManager("data/");
    Command_st BaseCommand("BaseCommand","(?this shouldn't ever be activated?)");
};

namespace Shell{
    namespace Commands{
        Command_st* CreateFile(Command_st* Caller){
            std::string FileName=(*Caller->Arguments)[Caller->ArgumentIndex];

            if(FileName == "CreateFile"){
                Caller->ReturnString="File name not specified.";
                return Caller;
            }
            if(!DatabaseManager.Create_File(FileName)){
                Caller->ReturnString="File already exists.";
                return Caller;
            }
            Caller->ReturnString="File created.";
            return Caller;
        }
    };
};
namespace Shell{
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
};
namespace Shell{
    void Initialize(){
        BaseCommand.AddSubCommand(new Command_st("CreateFile",Shell::Commands::CreateFile,"(Creates a file)"));
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