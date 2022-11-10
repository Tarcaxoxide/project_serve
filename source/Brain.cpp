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

        if(Get_UserName == ""){// /users/
            for(size_t i=7;i<GetText.size();i++){
                Get_UserName+=GetText[i];
            }
        }
        
        
        std::string Accounta=std::string("/Accounts/")+Get_UserName;

        Filesystem::File_st* File = Filesystem::FilesystemManager.FileSearch(Accounta);
        if(File != nullptr){
            Ret=File->Contents;
            Format::AddHeader(Ret,File->ContentType,Format::HeaderCodes::OK);
        }else if(GetText == std::string("/.well-known/webfinger?resource=acct:")+Get_UserName+std::string("@")+Get_Site){
            Ret=Format::Activitypub::GenerateProfile(Get_Site,Get_UserName);
            Format::AddHeader(Ret,"application/json",Format::HeaderCodes::OK);
        }
        else{
            Ret="404 file not found.";
            Format::AddHeader(Ret,"text/html",Format::HeaderCodes::NOT_FOUND);
        }

        std::cout<<"Return:{\n"<<Ret<<"\n}"<<std::endl;
        return Ret;
    }
};


/*
{
	"@context": [
		"https://www.w3.org/ns/activitystreams",
		"https://w3id.org/security/v1"
	],

	"id": "https://my-example.com/actor",
	"type": "Person",
	"preferredUsername": "alice",
	"inbox": "https://my-example.com/inbox",

	"publicKey": {
		"id": "https://my-example.com/actor#main-key",
		"owner": "https://my-example.com/actor",
		"publicKeyPem": "-----BEGIN PUBLIC KEY-----...-----END PUBLIC KEY-----"
	}
}
*/