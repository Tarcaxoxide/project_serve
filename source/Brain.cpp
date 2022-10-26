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
        Filesystem::File_st* File = Filesystem::FilesystemManager.FileSearch(GetText);
        if(File == nullptr){
            Ret="404 file not found.";
            Format::AddHeader(Ret,"text/html",false);
        }else{
            Ret=File->Contents;
            Format::AddHeader(Ret,File->ContentType,true);
        }
        return Ret;
    }
};