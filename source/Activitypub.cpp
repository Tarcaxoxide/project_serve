#include <Activitypub.hpp>
namespace Format{
    namespace Activitypub{
        namespace ObjectDefinitions{
            Object_st::Object_st(std::string site,std::string name){
                langauge=std::string("en");
                type=std::string("Object");
                id=std::string("http://")+site+std::string("/")+type+std::string("/")+name;
                this->name=name;
                this->site=site;
            }
            std::string Object_st::Object_Json(bool last){
                std::string Result;
                Result+=std::string("{");
                Result+=std::string("\"@context\": [\"https://www.w3.org/ns/activitystreams\",{\"@language\": \"")+langauge+std::string("\"}],");
                Result+=std::string("\"type\": \"")+type+std::string("\",");
                Result+=std::string("\"id\": \"")+id+std::string("\",");
                Result+=std::string("\"name\": \"")+name+std::string("\"");
                if(last){Result+=std::string("}");}else{Result+=std::string(",");}
                return Result;
            }
            std::string Object_st::_Json(){
                if(type == "Object")return ((Object_st*)this)->Object_Json(true);
                if(type == "Link")return ((Link_st*)this)->Link_Json();
                if(type == "Actor")return ((Actor_st*)this)->Actor_Json();
                if(type == "Note")return ((Note_st*)this)->Note_Json();
                if(type == "Activity")return ((Activity_st*)this)->Activity_Json();
                if(type == "IntransitiveActivity")return ((IntransitiveActivity_st*)this)->IntransitiveActivity_Json();
                if(type == "Collection")return ((Collection_st*)this)->Collection_Json();
                return std::string("{}");
            }
            Link_st::Link_st(std::string site,std::string name):Object_st(site,name){
                langauge=std::string("en");
                type=std::string("Link");
                id=std::string("http://")+site+std::string("/")+type+std::string("/")+name;
                this->name=name;
            }
            std::string Link_st::Link_Json(){
                std::string Result=Object_Json(false);
                Result+=std::string("\"href\": \"")+link+std::string("\",");
                Result+=std::string("\"hreflang\": \"")+linkLangauge+std::string("\",");
                Result+=std::string("\"mediaType\": \"")+mediaType+std::string("\"}");
                return Result;
            }
            Actor_st::Actor_st(std::string site,std::string name,std::string actorType):Object_st(site,name){
                langauge=std::string("en");
                type=actorType;
                id=std::string("http://")+site+std::string("/")+type+std::string("/")+name;
                inbox=std::string("https://")+site+std::string("/")+type+std::string("/")+name+std::string("/inbox");
                outbox=std::string("https://")+site+std::string("/")+type+std::string("/")+name+std::string("/outbox");
                username=name;
                this->name=name;
            }
            std::string Actor_st::Actor_Json(){
                std::string Result=Object_Json(false);
                Result+=std::string("\"subject\": \"acct:")+username+std::string("@")+site+std::string("\",");
                Result+=std::string("\"links\": [{\"rel\": \"self\",\"type\": \"application/activity+json\",\"href\": \"https://")+site+std::string("/")+type+std::string("/")+username+std::string("\"}],");
                Result+=std::string("\"preferredUsername\": \"")+username+std::string("\",");
                Result+=std::string("\"inbox\": \"")+inbox+std::string("\",");
                Result+=std::string("\"outbox\": \"")+outbox+std::string("\"}\n");
                return Result;
            }
            Note_st::Note_st(std::string site,std::string name):Object_st(site,name){
                langauge=std::string("en");
                type=std::string("Note");
                id=std::string("http://")+site+std::string("/")+type+std::string("/")+name;
                this->name=name;
            }
            std::string Note_st::Note_Json(){
                std::string Result=Object_Json(false);
                //TODO: Fill in with stuff XD
                return Result;
            }
            Activity_st::Activity_st(std::string site,std::string name):Object_st(site,name){
                langauge=std::string("en");
                type=std::string("Activity");
                id=std::string("http://")+site+std::string("/")+type+std::string("/")+name;
                this->name=name;
            }
            std::string Activity_st::Activity_Json(){
                std::string Result=Object_Json(false);
                Result+=std::string("\"summary\": \"")+summary+std::string("\",");
                Result+=std::string("\"actor\": ")+actor->Actor_Json()+std::string(",");
                Result+=std::string("\"object\": ");
                if(object->type == "Object"){Result+=((Object_st*)object)->Object_Json(true);}
                else if(object->type == "Link"){Result+=((Link_st*)object)->Link_Json();}
                else if(object->type == "Actor"){Result+=((Actor_st*)object)->Actor_Json();}
                else if(object->type == "Note"){Result+=((Note_st*)object)->Note_Json();}
                else{Result+=std::string("{}");}
                return Result;
            }
            IntransitiveActivity_st::IntransitiveActivity_st(std::string site,std::string name):Activity_st(site,name){
                langauge=std::string("en");
                type=std::string("IntransitiveActivity");
                id=std::string("http://")+site+std::string("/")+type+std::string("/")+name;
                this->name=name;
            }
            std::string IntransitiveActivity_st::IntransitiveActivity_Json(){
                std::string Result=Object_Json(false);
                Result+=std::string("\"summary\": \"")+summary+std::string("\",");
                Result+=std::string("\"actor\": ")+actor->Actor_Json()+std::string(",");
                Result+=std::string("\"target\": ");
                Result+=object->_Json();
                return Result;
            }
            Collection_st::Collection_st(std::string site,std::string name):Object_st(site,name){
                langauge=std::string("en");
                type=std::string("Collection");
                id=std::string("http://")+site+std::string("/")+type+std::string("/")+name;
                this->name=name;
            }
            std::string Collection_st::Collection_Json(){
                std::string Result=Object_Json(false);
                Result+=std::string("\"summary\": \"")+summary+std::string("\",");
                Result+=std::string("\"items\": [");
                for(size_t i=0;i<items.size();i++){
                    Result+=items[i]->_Json();
                    if(i<items.size()-1)Result+=",";
                }
                Result+=std::string("]}");
                return Result;
            }
        };
    };
};