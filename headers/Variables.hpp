#pragma once

#include <string>
#include <deque>

namespace Variables{
    struct Variable_st{
        std::string Name;
        int64_t Value;
    };
    struct VariableManager_cl{
        std::deque<Variable_st> TheseVariables;
        void SetVariable(std::string Name,int64_t Value);
        size_t GetVariable(std::string Name,int64_t& Value);
    };

    extern VariableManager_cl VariableManager;
};