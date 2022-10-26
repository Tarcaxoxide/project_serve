#include <Variables.hpp>


namespace Variables{
    void VariableManager_cl::SetVariable(std::string Name,int64_t Value){
        int64_t tmp;
        size_t tmpi=GetVariable(Name,tmp);
        if(!tmpi){
            TheseVariables.push_back({Name,Value});
        }else{
            TheseVariables[tmpi-1].Value=Value;
        }
    }
    size_t VariableManager_cl::GetVariable(std::string Name,int64_t& Value){
        for(size_t i=0;i<TheseVariables.size();i++){
            if(TheseVariables[i].Name == Name){
                Value=TheseVariables[i].Value;
                return i+1;
            }
        }
        Value=0;
        return 0;
    }
    VariableManager_cl VariableManager;
};