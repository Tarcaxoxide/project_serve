#pragma once

#include <iostream>
#include <string>
#include <deque>
#include <Server.hpp>
#include <Format.hpp>
#include <Brain.hpp>
#include <Filesystem.hpp>

namespace Shell{
    struct Command_st{
        std::deque<Command_st*> SubCommands;
        std::string CommandString,ReturnString;
        std::deque<std::string>* Arguments;
        size_t ArgumentIndex;
        static Command_st* BaseCommandReference;
        Command_st* (*ExecutableBody)(Command_st* Caller);
        Command_st(std::string CommandString,std::deque<Command_st*> SubCommands);
        Command_st(std::string CommandString,Command_st* (*ExecutableBody)(Command_st* Caller));
        Command_st(std::string CommandString);
        ~Command_st();
        Command_st* operator()(size_t ArgumentIndex,std::deque<std::string>* Arguments);
        Command_st* AddSubCommand(Command_st* SubCommands);
    };
    void Initialize();
    std::string Command(std::deque<std::string> args,bool& KeepGoing);
};