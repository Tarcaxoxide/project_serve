#include<Command.hpp>


namespace Shell{
    Command_st* TestCommandBody(Command_st* Caller){
        std::cout<<"Hello from TestCommandBody"<<std::endl;
        return Caller;
    }
};

namespace Shell{
    Command_st::Command_st(std::string CommandString,std::deque<Command_st*> SubCommands){
        this->CommandString=CommandString;
        ExecutableBody=nullptr;
    }
    Command_st::Command_st(std::string CommandString,Command_st* (*ExecutableBody)(Command_st* Caller)){
        this->CommandString=CommandString;
        this->ExecutableBody=ExecutableBody;
    }
    Command_st::Command_st(std::string CommandString){
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
    void Command_st::AddSubCommand(Command_st* nCommand){
        SubCommands.push_back(nCommand);
    }
    Command_st BaseCommand("BaseCommand");
    void Initialize(){
        BaseCommand.AddSubCommand(new Command_st("TestCommand",TestCommandBody));
    }
    std::string Command(std::deque<std::string> args,bool& KeepGoing){
        
        Command_st* PreviousCommand=(Command_st*)nullptr;
        Command_st* CurrentCommand=(Command_st*)&BaseCommand;
        for(size_t i=0;i<args.size();i++){
            PreviousCommand=CurrentCommand;
            std::cout<<i<<" arg = "<<args[i]<<std::endl;
            CurrentCommand=(*CurrentCommand)(args[i]);
            if(CurrentCommand == nullptr){
                break;
            }else{
                CurrentCommand=(*CurrentCommand)(args[i]);
            }
        }
        return "Results Unimplemented.";
    }
};