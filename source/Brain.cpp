#include<Brain.hpp>

namespace Brain{
    RequestHeader_st::RequestHeader_st(std::string RequestString){
        boost::replace_all(RequestString, "GET ", "GET:");
        boost::replace_all(RequestString, "POST ", "POST:");
        boost::replace_all(RequestString, ",", ", ");
        std::map<std::string, std::string> m;
        std::istringstream resp(RequestString.c_str());
        std::string header;
        std::string::size_type index;
        while (std::getline(resp, header) && header != "\r") {
          index = header.find(':', 0);
          if(index != std::string::npos) {
            m.insert(std::make_pair(
              boost::algorithm::trim_copy(header.substr(0, index)), 
              boost::algorithm::trim_copy(header.substr(index + 1))
            ));
          }
        }
        std::deque<std::string> Values;
        for(auto& kv: m) {
          std::string Line=kv.second;
          std::string Word;
          for(size_t i=0;i<Line.size();i++){
            if(Line[i] != ' '){
                Word+=Line[i];
            }else{
                if(Word == "")continue;
                Values.push_back(Word);
                Word="";
            }
            if(i==Line.size()-1){
                Values.push_back(Word);
                Word="";
            }
          }
          HeaderProperties.push_back(RequestHeaderProperty_st{kv.first,Values});
          Values.clear();
        }
    }
    std::deque<std::string> RequestHeader_st::Property(std::string Name){
        for(size_t i=0;i<HeaderProperties.size();i++){
            if(HeaderProperties[i].Name == Name)return HeaderProperties[i].Values;
        }
        return std::deque<std::string>{".."};
    }
};

namespace Brain{

    void Initialize(){
        Variables::VariableManager.SetVariable("Print_Requests",1);
    }
    std::string Think(std::string Input,Network::Socket_st* Client,Network::Server_cl& Server){
        std::string Ret,GetText;
        int64_t Print_Requests;Variables::VariableManager.GetVariable("Print_Requests",Print_Requests);
        RequestHeader_st RequestHeader(Input);
        GetText=RequestHeader.Property("GET")[0];
        if(Print_Requests){
            for(size_t ia=0;ia<RequestHeader.HeaderProperties.size();ia++){
                std::cout<<ia<<":`"<<RequestHeader.HeaderProperties[ia].Name<<"`"<<std::endl;
                for(size_t ib=0;ib<RequestHeader.HeaderProperties[ia].Values.size();ib++){
                    std::cout<<"\t"<<ib<<":`"<<Format::urlDecode(RequestHeader.HeaderProperties[ia].Values[ib])<<"`"<<std::endl;
                }
            }
            std::cout<<"GET? "<<Format::urlDecode(GetText)<<std::endl;
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
            Network::Activitypub::ObjectDefinitions::Activity_st* nActivty = Network::Activitypub::Create_CreateNote_Activity("dragoncrawler.loganjohndarylgraham.xyz","main","1","2","test",{"nobody"});
            std::cout<<Network::Activitypub::Activity_ToString(nActivty)<<std::endl;
            delete[] nActivty->object;
            delete[] nActivty;
            std::cout<<"?TEST?"<<std::endl;
        //Test

        return Ret;
    }
};