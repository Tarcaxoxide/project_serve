#include <Activitypub.hpp>
namespace Format{
    namespace Activitypub{
        namespace ObjectDefinitions{
            Object_st::Object_st(std::string site,std::string name,Identifier_st Identifier){
                langauge=std::string("en");
                type=std::string("Object");
                this->Identifier=Identifier;
                this->name=name;
                this->site=site;
            }
            std::string Object_st::Object_Json(bool last){
                std::string Result;
                Result+=std::string("{");
                Result+=std::string("\"@context\": [\"https://www.w3.org/ns/activitystreams\",{\"@language\": \"")+langauge+std::string("\"}],");
                Result+=std::string("\"type\": \"")+type+std::string("\",");

                std::string ID_Type = Format::NumberToHex(Identifier.Type);
                std::string ID_Number = Format::NumberToHex(Identifier.Number);
                IdentifierString=std::string("https://")+site+std::string("/Article/")+ID_Type+std::string("/")+ID_Number;
                Result+=std::string("\"id\": \"")+IdentifierString+std::string("\",");

                Result+=std::string("\"name\": \"")+name+std::string("\"");
                if(last){Result+=std::string("}");}else{Result+=std::string(",");}
                return Result;
            }
            std::string Object_st::_Json(){
                if(Identifier.Type == Identifier_Type::IT_OBJECT)return ((Object_st*)this)->Object_Json(true);
                if(Identifier.Type == Identifier_Type::IT_LINK)return ((Link_st*)this)->Link_Json();
                if(Identifier.Type == Identifier_Type::IT_PERSON)return ((Actor_st*)this)->Actor_Json();
                if(Identifier.Type == Identifier_Type::IT_NOTE)return ((Note_st*)this)->Note_Json();
                if(Identifier.Type == Identifier_Type::IT_CREATE_NOTE)return ((Activity_st*)this)->Activity_Json();
                if(Identifier.Type == Identifier_Type::IT_iCREATE_NOTE)return ((IntransitiveActivity_st*)this)->IntransitiveActivity_Json();
                if(Identifier.Type == Identifier_Type::IT_COLLECTION)return ((Collection_st*)this)->Collection_Json();
                if(Identifier.Type == Identifier_Type::IT_CREATE_COLLECTION)return ((Activity_st*)this)->Activity_Json();
                if(Identifier.Type == Identifier_Type::IT_iCREATE_COLLECTION)return ((IntransitiveActivity_st*)this)->IntransitiveActivity_Json();
                return std::string("{}");
            }
            Link_st::Link_st(std::string site,std::string name,Identifier_st Identifier):Object_st(site,name,Identifier){
                langauge=std::string("en");
                type=std::string("Link");
                this->name=name;
            }
            std::string Link_st::Link_Json(){
                std::string Result=Object_Json(false);
                Result+=std::string("\"href\": \"")+link+std::string("\",");
                Result+=std::string("\"hreflang\": \"")+linkLangauge+std::string("\",");
                Result+=std::string("\"mediaType\": \"")+mediaType+std::string("\"}");
                return Result;
            }
            Actor_st::Actor_st(std::string site,std::string name,Identifier_st Identifier):Object_st(site,name,Identifier){
                langauge=std::string("en");
                type=Identifier_TypeString[Identifier.Type];
                username=name;
                this->name=name;
            }
            std::string Actor_st::Actor_Json(){
                std::string Result=Object_Json(false);
                Result+=std::string("\"subject\": \"acct:")+username+std::string("@")+site+std::string("\",");
                Result+=std::string("\"links\": [{\"rel\": \"self\",\"type\": \"application/activity+json\",\"href\": \"")+IdentifierString+std::string("\"}],");
                Result+=std::string("\"preferredUsername\": \"")+username+std::string("\",");
                Result+=std::string("\"inbox\": \"")+IdentifierString+std::string("/inbox")+std::string("\",");
                Result+=std::string("\"outbox\": \"")+IdentifierString+std::string("/outbox")+std::string("\"}\n");
                return Result;
            }
            Note_st::Note_st(std::string site,std::string name,std::string content,std::string published,std::string attributedTo,std::string to,Identifier_st Identifier):Object_st(site,name,Identifier){
                langauge=std::string("en");
                type=std::string("Note");
                this->name=name;
                this->published=published;
                this->attributedTo=attributedTo;
                inReplyTo=std::string("");
                this->content=content;
                this->to=to;
                to=std::string("https://www.w3.org/ns/activitystreams#Public");
            }
            std::string Note_st::Note_Json(){
                std::string Result=Object_Json(false);
                Result+=std::string("\"published\": \"")+published+std::string("\",");
                Result+=std::string("\"attributedTo\": \"")+attributedTo+std::string("\",");
                if(inReplyTo != "")Result+=std::string("\"inReplyTo\": \"")+inReplyTo+std::string("\",");
                Result+=std::string("\"content\": \"")+content+std::string("\",");
                Result+=std::string("\"to\": \"")+to+std::string("\"}");

                return Result;
            }
            Activity_st::Activity_st(std::string site,std::string name,Actor_st* actor,std::deque<Actor_st*> audience,Identifier_st Identifier):Object_st(site,name,Identifier){
                langauge=std::string("en");
                type=Identifier_TypeString[Identifier.Type];
                this->name=name;
                this->actor=actor;
                std::string _to="[";
                for(size_t i=0;i<audience.size();i++){
                    _to+=std::string("\"")+audience[i]->IdentifierString+std::string("\"");
                    if(i<audience.size()-1)_to+=std::string(",");
                }
                _to+="]";
                switch(Identifier.Type){
                    case Identifier_Type::IT_CREATE_NOTE:{
                        object = new Note_st(site,Identifier_TypeString[Identifier.Type-1],name,Format::currentDateAndTime(),actor->IdentifierString,_to,{Identifier_Type::IT_NOTE,Identifier.Number});
                    }break;
                    default:{}break;
                }
            }
            std::string Activity_st::Activity_Json(){
                std::string Result=Object_Json(false);
                
                //  {
                //  	"@context": "https://www.w3.org/ns/activitystreams",
                //  
                //  	"id": "https://my-example.com/create-hello-world",
                //  	"type": "Create",
                //  	"actor": "https://my-example.com/actor",
                Result+=std::string("\"actor\": \"")+actor->IdentifierString+std::string("\",");
                //  
                //  	"object": {
                //  		"id": "https://my-example.com/hello-world",
                //  		"type": "Note",
                //  		"published": "2018-06-23T17:17:11Z",
                //  		"attributedTo": "https://my-example.com/actor",
                //  		"inReplyTo": "https://mastodon.social/@Gargron/100254678717223630",
                //  		"content": "<p>Hello world</p>",
                //  		"to": "https://www.w3.org/ns/activitystreams#Public"
                //  	}
                Result+=object->_Json();
                //  }
                Result+=std::string("}");

                return Result;
            }
            IntransitiveActivity_st::IntransitiveActivity_st(std::string site,std::string name,Actor_st* actor,std::deque<Actor_st*> audience,Identifier_st Identifier):Activity_st(site,name,actor,audience,Identifier){
                langauge=std::string("en");
                type=std::string("IntransitiveActivity");
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
            Collection_st::Collection_st(std::string site,std::string name,std::deque<Object_st*> items,Identifier_st Identifier):Object_st(site,name,Identifier){
                langauge=std::string("en");
                type=std::string("Collection");
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