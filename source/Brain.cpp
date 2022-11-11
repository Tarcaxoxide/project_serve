#include<Brain.hpp>

namespace Brain{

    void Initialize(){
    }
    std::string Think(std::string Input,Network::Socket_st* Client,Network::Server_cl& Server){
        std::string Ret,GetText;
        
        Format::RequestHeader_st RequestHeader(Input);
        GetText=Format::urlDecode(RequestHeader.Property("GET")[0]);

        std::cout<<Format::urlDecode(RequestHeader.AllProperties())<<std::endl;

        size_t State=0;
        std::string Get_UserName="",Get_Site="";
        for(size_t i=0;i<GetText.size();i++){
            switch(State){
                case 0:{
                    if(GetText[i] == 'a' && GetText[i+1] == 'c' && GetText[i+2] == 'c' && GetText[i+3] == 't' && GetText[i+4] == ':'){
                        i+=4;
                        State++;
                    }
                }break;
                case 1:{
                    if(GetText[i] == '@'){State++;}else{
                        Get_UserName+=GetText[i];
                    }
                }break;
                case 2:{
                    Get_Site+=GetText[i];
                }break;
            }
        }
        if(GetText == "/favicon.ico"){
            Ret="404 file not found.";
            Format::AddHeader(Ret,"text/html",Format::HeaderCodes::NOT_FOUND);
            return Ret;
        }
        if(Get_UserName == ""){// /actor/
            for(size_t i=8;i<GetText.size();i++){
                Get_UserName+=GetText[i];
            }
        }
        if(Get_Site == ""){
            Get_Site=RequestHeader.Property("Host")[0];
        }
        
        if(GetText == std::string("/Person/")+Get_UserName || GetText == std::string("/.well-known/webfinger?resource=acct:")+Get_UserName+std::string("@")+Get_Site){
            Format::Activitypub::ObjectDefinitions::Actor_st TestPerson(Get_Site,Get_UserName,"Person");
            Ret=TestPerson.Actor_Json();
            Format::AddHeader(Ret,"application/json",Format::HeaderCodes::OK);
        }
        std::cout<<"GetText:{\n"<<GetText<<"\n"<<Get_Site<<"\n"<<Get_UserName<<"\n}"<<std::endl;
        std::cout<<"Return:{\n"<<Ret<<"\n}"<<std::endl;
        return Ret;
    }
};