#include <Activitypub.hpp>

namespace Network{
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
};