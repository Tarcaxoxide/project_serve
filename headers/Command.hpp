#pragma once

#include<iostream>
#include<string>
#include<deque>
#include<Network/Server.hpp>
#include<Format.hpp>
#include<Brain.hpp>
#include<Data/ArtificialFileSystem.hpp>


namespace Shell{
    struct Command_st{
        std::deque<Command_st*> SubCommands;
        std::string Argument,CommandString;
        static Command_st* BaseCommandReference;
        Command_st* (*ExecutableBody)(Command_st* Caller);
        Command_st(std::string CommandString,std::deque<Command_st*> SubCommands);
        Command_st(std::string CommandString,Command_st* (*ExecutableBody)(Command_st* Caller));
        Command_st(std::string CommandString);
        ~Command_st();
        Command_st* operator()(std::string Argument);
        Command_st* AddSubCommand(Command_st* SubCommands);
    };
    void Initialize();
    std::string Command(std::deque<std::string> args,bool& KeepGoing);
};