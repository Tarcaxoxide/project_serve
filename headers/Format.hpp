#pragma once

#include <deque>
#include <string>
#include <jsoncpp/json/json.h>

namespace Format{
    
    void AddHeader(std::string& Message,std::string type,bool good);
    std::deque<std::string> split(std::string str, std::string token);
    std::string urlEncode(std::string str,bool EncodeSpaces=false);
    std::string urlDecode(std::string str);
};