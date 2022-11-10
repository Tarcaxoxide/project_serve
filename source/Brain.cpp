#include<Brain.hpp>

namespace Brain{

    void Initialize(){
        Variables::VariableManager.SetVariable("Print_Requests",1);
    }
    std::string Think(std::string Input,Network::Socket_st* Client,Network::Server_cl& Server){
        std::string Ret,GetText;
        int64_t Print_Requests;Variables::VariableManager.GetVariable("Print_Requests",Print_Requests);
        Format::RequestHeader_st RequestHeader(Input);
        GetText=RequestHeader.Property("GET")[0];
        if(Print_Requests){
            //for(size_t ia=0;ia<RequestHeader.HeaderProperties.size();ia++){
            //    std::cout<<ia<<":`"<<RequestHeader.HeaderProperties[ia].Name<<"`"<<std::endl;
            //    for(size_t ib=0;ib<RequestHeader.HeaderProperties[ia].Values.size();ib++){
            //        std::cout<<"\t"<<ib<<":`"<<Format::urlDecode(RequestHeader.HeaderProperties[ia].Values[ib])<<"`"<<std::endl;
            //    }
            //}
            //std::cout<<"GET? "<<Format::urlDecode(GetText)<<std::endl;
            std::cout<<RequestHeader.AllProperties()<<std::endl;
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
            Format::Activitypub::ObjectDefinitions::Activity_st* nActivty = Format::Activitypub::Create_CreateNote_Activity("dragoncrawler.loganjohndarylgraham.xyz","main","1","2","test",{"nobody"});
            std::cout<<Format::Activitypub::Activity_ToString(nActivty)<<std::endl;
            delete nActivty->object;
            delete nActivty;
            std::cout<<"?TEST?"<<std::endl;
        //Test

        return Ret;
    }
};