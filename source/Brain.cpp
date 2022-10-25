#include<Brain.hpp>

namespace Brain{

    void Initialize(){
    }
    std::string Think(std::string Input,Network::Socket_st* Client,Network::Server_cl& Server){
        std::string Ret,GetText;

        bool trig=false;
        for(size_t i=0;i<Input.size();i++){
            if(trig){
                if(Input[i] == ' '){trig=false;break;}
                GetText+=Input[i];
            }
            if(Input[i] == 'G' && Input[i+1] == 'E' && Input[i+2] == 'T' && Input[i+3] == ' '){
                i+=4;
                trig=true;
            }
        }

        Ret=std::string("(Unimplemented):")+GetText;
        
        Format::AddHeader(Ret,"text/html",false);

        return Ret;
    }
};