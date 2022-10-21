#include<Format.hpp>


namespace Format{
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