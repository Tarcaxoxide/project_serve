#include<Brain.hpp>


Data::Profile_st test_profile("dragoncrawler","loganjohndarylgraham","xyz","test_profile","0000000000");

/*
    (36738)Request{
    GET /users/0000000000 HTTP/1.0
    Host: 127.0.0.1:8083
    Connection: close
    accept: application/activity json, application/ld json
    accept-encoding: gzip,deflate,br
    user-agent: Misskey/12.108.1 (https://www.loganjohndarylgraham.xyz)


    };
*/

namespace Brain{

    void Initialize(){
        Data::File_st* File;
        Data::ArtificialFileSystem.Create("test.txt");
        size_t FileIndex=Data::ArtificialFileSystem.Search("test.txt",File);
        File->Contents="Hello World!";
    }
    std::string Think(std::string Input,Network::Socket_st* Client,Network::Server_cl& Server){
        std::string Ret,GetText;
        Data::File_st* File;

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

        std::cout<<"Getting '"<<GetText<<"'"<<std::endl;
        size_t FileIndex=Data::ArtificialFileSystem.Search(GetText,File);
        if(FileIndex){
            std::cout<<"File found"<<std::endl;
            Ret=File->Contents;
            Format::AddHeader(Ret,File->Type,true);
        }else{
            std::cout<<"File not found"<<std::endl;
            Format::AddHeader(Ret,"text/html",false);
        }

        return Ret;
    }
};