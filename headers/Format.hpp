#pragma once

#include <deque>
#include <string>
#include <jsoncpp/json/json.h>
#include <Variables.hpp>
#include <map>
#include <boost/algorithm/string.hpp>

namespace Format{
    namespace Activitypub{
        namespace ObjectDefinitions{
            struct Object_st{
                std::string context;
                std::string type;
                std::string id;
                std::string name;
            };
            struct Note_st : public Object_st{
                std::deque<std::string> to;
                std::string attributedTo;
                std::string content;
            };
            struct Activity_st : public Object_st{
                std::deque<std::string> to;
                std::string actor;
                Object_st* object;
            };
        };
        ObjectDefinitions::Activity_st* Create_CreateNote_Activity(std::string site,std::string actor,std::string ActivityID,std::string NoteID,std::string message,std::deque<std::string> SendTo);
        std::string Activity_ToString(ObjectDefinitions::Activity_st* Activity);
        std::string Note_ToString(ObjectDefinitions::Note_st* cNote);
        std::string GenerateProfile(std::string site,std::string username);
    };
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
};