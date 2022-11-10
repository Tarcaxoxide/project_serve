#include<Format.hpp>


namespace Format{
    namespace Activitypub{
        ObjectDefinitions::Activity_st* Create_CreateNote_Activity(std::string site,std::string actor,std::string ActivityID,std::string NoteID,std::string message,std::deque<std::string> SendTo){
            ObjectDefinitions::Activity_st* New_CreateNote_Activity=new ObjectDefinitions::Activity_st;
            New_CreateNote_Activity->context=std::string("https://www.w3.org/ns/activitystreams");
            New_CreateNote_Activity->type=std::string("Create");
            New_CreateNote_Activity->id=std::string("https://")+site+std::string("/")+actor+std::string("/posts/")+ActivityID;
            for(size_t i=0;i<SendTo.size();i++){
                New_CreateNote_Activity->to.push_back(SendTo[i]);
            }
            New_CreateNote_Activity->actor=std::string("https://")+site+std::string("/")+actor+std::string("/");

            New_CreateNote_Activity->object=new ObjectDefinitions::Note_st;
            ((ObjectDefinitions::Note_st*)New_CreateNote_Activity->object)->context=std::string("https://www.w3.org/ns/activitystreams");
            ((ObjectDefinitions::Note_st*)New_CreateNote_Activity->object)->type=std::string("Note");
            ((ObjectDefinitions::Note_st*)New_CreateNote_Activity->object)->id=std::string("https://")+site+std::string("/")+actor+std::string("/posts/")+NoteID;
            for(size_t i=0;i<SendTo.size();i++){
                ((ObjectDefinitions::Note_st*)New_CreateNote_Activity->object)->to.push_back(SendTo[i]);
            }
            ((ObjectDefinitions::Note_st*)New_CreateNote_Activity->object)->attributedTo=std::string("https://")+site+std::string("/")+actor+std::string("/");
            ((ObjectDefinitions::Note_st*)New_CreateNote_Activity->object)->content=message;
            return New_CreateNote_Activity;
        }
        std::string Activity_ToString(ObjectDefinitions::Activity_st* Activity){
            std::string Result="{";

            Result+=std::string("\"@context\": \"")+Activity->context+std::string("\",\n");
            Result+=std::string("\"type\": \"")+Activity->type+std::string("\",\n");
            Result+=std::string("\"id\": \"")+Activity->id+std::string("\",\n");
            std::string SendToString=std::string("\"to\": [");
            for(size_t i=0;i<Activity->to.size();i++){
                SendToString+=std::string("\"")+Activity->to[i]+std::string("\"");
                if(i<Activity->to.size()-1)SendToString+=std::string(",");
            }
            SendToString+=std::string("],\n");
            Result+=SendToString;
            Result+=std::string("\"actor\": \"")+Activity->actor+std::string("\",\n");
            Result+=std::string("\"object\": {");
            if(Activity->type == "Create"){
                ObjectDefinitions::Object_st* cObject=Activity->object;
                Result+=std::string("\"@context\": \"")+cObject->context+std::string("\",\n");
                Result+=std::string("\"type\": \"")+cObject->type+std::string("\",\n");
                Result+=std::string("\"id\": \"")+cObject->id+std::string("\",\n");
                Result+=SendToString;

                if(cObject->type == "Note"){
                    ObjectDefinitions::Note_st* cNote=(ObjectDefinitions::Note_st*)cObject;
                    Result+=std::string("\"attributedTo\": \"")+cNote->attributedTo+std::string("\",\n");
                    Result+=std::string("\"content\": \"")+cNote->content+std::string("\"\n");
                }
            }
            Result+=std::string("}\n");
            Result+="}";
            return Result;
        }
    };
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
    void AddHeader(std::string& Message,std::string type,bool good){
        size_t messageSize=Message.size();
        std::string Header;
        if(good){
            Header+="HTTP/1.0 200 OK\r\n";
            Header+="Content-type: "+type+"\r\n";
            Header+="Content-Length: "+std::to_string(messageSize)+"\r\n";
            Header+="Connection: close";
            Header+="\r\n\r\n";
            Message=Header+Message;
        }else{
            Message="404 file not found";
            size_t messageSize=Message.size();
            Header+="HTTP/1.0 404 File not found\r\n";
            Header+="Content-type: "+type+"\r\n";
            Header+="Content-Length: "+std::to_string(messageSize)+"\r\n";
            Header+="Connection: close";
            Header+="\r\n\r\n";
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