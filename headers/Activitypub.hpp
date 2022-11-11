#pragma once

#include <deque>
#include <string>

namespace Format{
    namespace Activitypub{
        namespace ObjectDefinitions{
            struct Object_st{
                std::string langauge;
                std::string type;
                std::string id;
                std::string name;
                std::string site;
                Object_st(std::string site,std::string name);
                std::string Object_Json(bool last);
                std::string _Json();
            };
            struct Link_st : public Object_st{
                std::string link;
                std::string linkLangauge;
                std::string mediaType;
                Link_st(std::string site,std::string name);
                std::string Link_Json();
            };
            struct Actor_st : public Object_st{
                std::string username;
                std::string inbox;
                std::string outbox;
                Actor_st(std::string site,std::string name,std::string actorType);
                std::string Actor_Json();
            };
            struct Note_st : public Object_st{
                Note_st(std::string site,std::string name);
                std::string Note_Json();
            };
            struct Activity_st : public Object_st{
                std::string summary;
                Actor_st* actor=nullptr;
                Object_st* object=nullptr;
                Activity_st(std::string site,std::string name);
                std::string Activity_Json();
            };
            struct IntransitiveActivity_st : public Activity_st{
                IntransitiveActivity_st(std::string site,std::string name);
                std::string IntransitiveActivity_Json();
            };
            struct Collection_st : public Object_st{
                std::string summary;
                std::deque<Object_st*> items;
                Collection_st(std::string site,std::string name);
                std::string Collection_Json();
            };
        };
    };
};