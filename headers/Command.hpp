#pragma once

#include <iostream>
#include <string>
#include <deque>
#include <Server.hpp>
#include <Format.hpp>
#include <Brain.hpp>
#include <Variables.hpp>
#include <cstdlib>
#include <fstream>
#include <fcntl.h>           /* Definition of AT_* constants */
#include <sys/stat.h>
#include <Settings.hpp>
#include<Database.hpp>

namespace Shell{
    struct Command_st{
        std::deque<Command_st*> SubCommands;
        std::string CommandString,ReturnString,HelpString;
        std::deque<std::string>* Arguments;
        void* ReturnPtr{nullptr};
        size_t ArgumentIndex;
        static Command_st* BaseCommandReference;
        Command_st* (*ExecutableBody)(Command_st* Caller);
        Command_st(std::string CommandString,std::deque<Command_st*> SubCommands,std::string HelpString);
        Command_st(std::string CommandString,Command_st* (*ExecutableBody)(Command_st* Caller),std::string HelpString);
        Command_st(std::string CommandString,std::string HelpString);
        ~Command_st();
        Command_st* operator()(size_t ArgumentIndex,std::deque<std::string>* Arguments);
        Command_st* AddSubCommand(Command_st* SubCommands);
    };
    void Initialize();
    std::string Command(std::deque<std::string> args,bool& KeepGoing);
};