#include<Brain.hpp>

namespace Brain{

    void Initialize(){
    }
    std::string Think(std::string Input,Network::Socket_st* Client,Network::Server_cl& Server){
        std::string Ret,GetText;
        
        Format::RequestHeader_st RequestHeader(Input);
        GetText=Format::urlDecode(RequestHeader.Property("GET")[0]);

        size_t State=0;
        std::string Get_UserName,Get_Site;
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

        Filesystem::File_st* File = Filesystem::FilesystemManager.FileSearch(GetText);
        if(File == nullptr){
            Ret="404 file not found.";
            Format::AddHeader(Ret,"text/html",false);
        }else{
            Ret=File->Contents;
            Format::AddHeader(Ret,File->ContentType,true);
        }

        //Test
            std::cout<<"?TEST?"<<std::endl;
            Format::Activitypub::ObjectDefinitions::Activity_st* nActivty = Format::Activitypub::Create_CreateNote_Activity(Get_Site,Get_UserName,"1","2","test",{"nobody"});
            std::cout<<Format::Activitypub::Activity_ToString(nActivty)<<std::endl;
            delete nActivty->object;
            delete nActivty;
            std::cout<<"?TEST?"<<std::endl;
        //Test

        return Ret;
    }
};


//	0: Connection
//		0: close
//	1: GET
//		0: /.well-known/webfinger?resource=acct:main@dragoncrawler.loganjohndarylgraham.xyz
//		1: HTTP/1.0
//	2: Host
//		0: dragoncrawler.loganjohndarylgraham.xyz
//	3: accept
//		0: text/html,
//		1: application/xhtml xml,
//		2: application/xml;q=0.9,
//		3: image/avif,
//		4: image/webp,
//		5: */*;q=0.8
//	4: accept-encoding
//		0: gzip,
//		1: deflate,
//		2: br
//	5: accept-language
//		0: en-US,
//		1: en;q=0.5
//	6: sec-fetch-dest
//		0: document
//	7: sec-fetch-mode
//		0: navigate
//	8: sec-fetch-site
//		0: none
//	9: sec-fetch-user
//		0: ?1
//	10: upgrade-insecure-requests
//		0: 1
//	11: user-agent
//		0: Mozilla/5.0
//		1: (X11;
//		2: Linux
//		3: x86_64;
//		4: rv:104.0)
//		5: Gecko/20100101
//		6: Firefox/104.0