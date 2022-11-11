#include<Format.hpp>


namespace Format{
    
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
    std::string RequestHeader_st::AllProperties(){
        std::string Result;
        for(size_t ia=0;ia<HeaderProperties.size();ia++){
            Result+=std::string("\t")+std::to_string(ia)+std::string(": ")+HeaderProperties[ia].Name+std::string("\n");
            for(size_t ib=0;ib<HeaderProperties[ia].Values.size();ib++){
                Result+=std::string("\t\t")+std::to_string(ib)+std::string(": ")+HeaderProperties[ia].Values[ib]+std::string("\n");
            }
        }
        return Result;
    }
    void AddHeader(std::string& Message,std::string type,HeaderCodes code){
        size_t messageSize=Message.size();
        std::string Header;
        std::string loc=Message;
        
        Header+="HTTP/1.0 ";
        Header+=std::to_string(code)+std::string(" ");
        if(code == HeaderCodes::NOT_FOUND)Header+=std::string("file not found");
        if(code == HeaderCodes::OK)Header+=std::string("ok");
        if(code == HeaderCodes::FOUND)Header+=std::string("location:")+loc;
        if(code == HeaderCodes::REDIRECT)Header+=std::string("location: ")+loc;
        Header+=std::string("\r\n");
        Header+=std::string("Content-type: ")+type+std::string("\r\n");
        Header+=std::string("Content-Length: ")+std::to_string(messageSize)+std::string("\r\n");
        Header+=std::string("Connection: close");
        Header+=std::string("\r\n\r\n");
        if(code == HeaderCodes::FOUND || code == HeaderCodes::REDIRECT){
            Message=std::string("Redirecting to ")+loc;
        }else{
            Message=Header+Message;
        }
    }
    std::deque<std::string> split(std::string str, std::string token){
        std::deque<std::string> result;
        while(str.size()){
            int index = str.find(token);
            if(index!=std::string::npos){
                result.push_back(str.substr(0,index));
                str = str.substr(index+token.size());
                if(str.size()==0)result.push_back(str);
            }else{
                result.push_back(str);
                str = "";
            }
        }
        return result;
    }
    std::string urlEncode(std::string str,bool EncodeSpaces){
        std::string new_str = "";
        char c;
        int ic;
        const char* chars = str.c_str();
        char bufHex[10];
        int len = strlen(chars);
        for(int i=0;i<len;i++){
            c = chars[i];
            ic = c;
            if (c==' ' && EncodeSpaces) new_str += '+';   
            else if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') new_str += c;
            else {
                sprintf(bufHex,"%X",c);
                if(ic < 16) 
                    new_str += "%0"; 
                else
                    new_str += "%";
                new_str += bufHex;
            }
        }
        return new_str;
     }
    std::string urlDecode(std::string str){
        std::string ret;
        char ch;
        int i, ii, len = str.length();
        for (i=0; i < len; i++){
            if(str[i] != '%'){
                if(str[i] == '+')
                    ret += ' ';
                else
                    ret += str[i];
            }else{
                sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
                ch = static_cast<char>(ii);
                ret += ch;
                i = i + 2;
            }
        }
        return ret;
    }
};