#pragma once

#include <deque>
#include <string>
#include <Variables.hpp>
#include <map>
#include <boost/algorithm/string.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <dirent.h>
#include <stdio.h>

namespace Format{
    struct RequestHeaderProperty_st{
        std::string Name;
        std::deque<std::string> Values;
    };
    struct RequestHeader_st{
        std::deque<RequestHeaderProperty_st> HeaderProperties;
        RequestHeader_st(std::string RequestString);
        std::deque<std::string> Property(std::string Name);
        std::string AllProperties();
    };
    enum HeaderCodes{
        OK=200,
        REDIRECT=301,
        FOUND=302,
        NOT_FOUND=404
    };
    void AddHeader(std::string& Message,std::string type,HeaderCodes code);
    std::deque<std::string> split(std::string str, std::string token);
    std::string urlEncode(std::string str,bool EncodeSpaces=false);
    std::string urlDecode(std::string str);
    std::string currentDateAndTime();
    std::string NumberToHex(size_t number);
    size_t HexToNumber(std::string hex);
    std::deque<std::string> listDirectories(std::string path);
};