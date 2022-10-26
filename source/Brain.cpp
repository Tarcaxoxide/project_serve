#include<Brain.hpp>


// KEY: `Accept`, VALUE: {`text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8`}
// KEY: `Accept-Encoding`, VALUE: {`gzip,`,`deflate,`,`br`}
// KEY: `Accept-Language`, VALUE: {`en-US,en;q=0.5`}
// KEY: `Connection`, VALUE: {`keep-alive`}
// KEY: `Cookie`, VALUE: {`__Host-pleroma_key=SFMyNTY.g3QAAAABbQAAAAtvYXV0aF90b2tlbm0AAAArY1NuTHh1OWhhS2tsNWhkQTdrb0NPZlBPVTFtVGR3YzAxLV96WkhsU1JXUQ.KnaUN18o8rMfukn0Cy0riHzBboIe6H0lPyjNtUhlFpQ`}
// KEY: `GET`, VALUE: {`/test.html`,`HTTP/1.1`}
// KEY: `Host`, VALUE: {`localhost:8083`}
// KEY: `Sec-Fetch-Dest`, VALUE: {`document`}
// KEY: `Sec-Fetch-Mode`, VALUE: {`navigate`}
// KEY: `Sec-Fetch-Site`, VALUE: {`none`}
// KEY: `Sec-Fetch-User`, VALUE: {`?1`}
// KEY: `Upgrade-Insecure-Requests`, VALUE: {`1`}
// KEY: `User-Agent`, VALUE: {`Mozilla/5.0`,`(X11;`,`Linux`,`x86_64;`,`rv:104.0)`,`Gecko/20100101`,`Firefox/104.0`}


// KEY: `Accept`, VALUE: {`text/html,`,`application/xhtml+xml,`,`application/xml;q=0.9,`,`image/avif,`,`image/webp,`,`*/*;q=0.8`}
// KEY: `Accept-Encoding`, VALUE: {`gzip,`,``,`deflate,`,``,`br`}
// KEY: `Accept-Language`, VALUE: {`en-US,`,`en;q=0.5`}
// KEY: `Connection`, VALUE: {`keep-alive`}
// KEY: `Cookie`, VALUE: {`__Host-pleroma_key=SFMyNTY.g3QAAAABbQAAAAtvYXV0aF90b2tlbm0AAAArY1NuTHh1OWhhS2tsNWhkQTdrb0NPZlBPVTFtVGR3YzAxLV96WkhsU1JXUQ.KnaUN18o8rMfukn0Cy0riHzBboIe6H0lPyjNtUhlFpQ`}
// KEY: `GET`, VALUE: {`/test.html`,`HTTP/1.1`}
// KEY: `Host`, VALUE: {`localhost:8083`}
// KEY: `Sec-Fetch-Dest`, VALUE: {`document`}
// KEY: `Sec-Fetch-Mode`, VALUE: {`navigate`}
// KEY: `Sec-Fetch-Site`, VALUE: {`none`}
// KEY: `Sec-Fetch-User`, VALUE: {`?1`}
// KEY: `Upgrade-Insecure-Requests`, VALUE: {`1`}
// KEY: `User-Agent`, VALUE: {`Mozilla/5.0`,`(X11;`,`Linux`,`x86_64;`,`rv:104.0)`,`Gecko/20100101`,`Firefox/104.0`}





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
                    std::cout<<"\t"<<ib<<":`"<<RequestHeader.HeaderProperties[ia].Values[ib]<<"`"<<std::endl;
                }
            }
            std::cout<<"GET? "<<GetText<<std::endl;
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