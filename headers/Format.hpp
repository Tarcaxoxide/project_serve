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
            struct Actor_st : public Object_st{
                std::string preferredUsername;
                std::string summary;
                std::string inbox;
                std::string outbox;
                std::string followers;
                std::string following;
                std::string liked;
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
    void AddHeader(std::string& Message,std::string type,bool good);
    std::deque<std::string> split(std::string str, std::string token);
    std::string urlEncode(std::string str,bool EncodeSpaces=false);
    std::string urlDecode(std::string str);
};