#include<Command.hpp>


namespace Shell{
    Command_st* TestCommandA_Body(Command_st* Caller){
        Caller->ReturnString="Hello from TestCommandB_Body.";
        return Caller;
    }
    Command_st* TestCommandB_Body(Command_st* Caller){
        Caller->ReturnString="Hello from TestCommandB_Body.";
        return Caller;
    }
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
    Command_st* Command_st::operator()(std::string Argument){
        this->Argument=Argument;
        if(ExecutableBody != nullptr){
            return (*ExecutableBody)(this);
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
    void Initialize(){
        Command_st* nCommand=BaseCommand.AddSubCommand(new Command_st("Test"));
        nCommand->AddSubCommand(new Command_st("a",TestCommandA_Body));
        nCommand->AddSubCommand(new Command_st("b",TestCommandA_Body));
    }
    std::string Command(std::deque<std::string> args,bool& KeepGoing){
        Command_st* PreviousCommand=(Command_st*)nullptr;
        Command_st* CurrentCommand=(Command_st*)&BaseCommand;
        for(size_t i=0;i<args.size()+1;i++){
            if(args.size() == i){CurrentCommand=(*CurrentCommand)(args[i-1]);}else{CurrentCommand=(*CurrentCommand)(args[i]);}
            if(CurrentCommand == nullptr || PreviousCommand == CurrentCommand){
                break;
            }
            PreviousCommand=CurrentCommand;
        }
        return ((CurrentCommand == nullptr) ? ((PreviousCommand == nullptr)? "Unknown command." : "Unsupported argument.") : CurrentCommand->ReturnString);
    }
};