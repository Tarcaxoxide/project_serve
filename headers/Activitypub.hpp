#pragma once

#include <string>
#include <deque>

namespace Network{
    namespace Activitypub{
        namespace ObjectDefinitions{
            struct Object_st{
                
                std::string context;
                std::string type;
                std::string id;
                std::string name;
                //virtual ~Object_st(){}
            };
            struct Actor_st : public Object_st{
                std::string preferredUsername;
                std::string summary;
                std::string inbox;
                std::string outbox;
                std::string followers;
                std::string following;
                std::string liked;
                //virtual ~Actor_st(){}
            };
            struct Note_st : public Object_st{
                std::deque<std::string> to;
                std::string attributedTo;
                std::string content;
                //virtual ~Note_st(){}
            };
            struct Activity_st : public Object_st{
                std::deque<std::string> to;
                std::string actor;
                Object_st* object;
                //virtual ~Activity_st(){}
            };
        };

        ObjectDefinitions::Activity_st* Create_CreateNote_Activity(std::string site,std::string actor,std::string ActivityID,std::string NoteID,std::string message,std::deque<std::string> SendTo);
        std::string Activity_ToString(ObjectDefinitions::Activity_st* Activity);
    };
};