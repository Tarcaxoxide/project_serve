#pragma once

#include <deque>
#include <string>
#include<Format.hpp>

namespace Format{
    namespace Activitypub{
        namespace ObjectDefinitions{
            enum Identifier_Type: size_t{
                IT_NULL=0,
                IT_OBJECT,
                IT_PERSON,
                IT_NOTE,
                IT_CREATE_NOTE,
                IT_iCREATE_NOTE,
                IT_COLLECTION,
                IT_CREATE_COLLECTION,
                IT_iCREATE_COLLECTION,
                IT_LINK,
                IT_SIZE
            };
            static const std::string Identifier_TypeString[Identifier_Type::IT_SIZE+1]={
                "Null",//IT_NULL
                "Object",//IT_OBJECT
                "Person",//IT_PERSON
                "Note",//IT_NOTE
                "Create",//IT_CREATE_NOTE
                "Create",//IT_iCREATE_NOTE
                "Collection",//IT_COLLECTION
                "Create",//IT_CREATE_COLLECTION
                "Create",//IT_iCREATE_COLLECTION
                "Link",//IT_LINK
                "Size"//IT_SIZE
            };
            struct Identifier_st{
                Identifier_Type Type;
                size_t Number;
            };
            struct Object_st{
                std::string langauge;
                std::string type;
                std::string name;
                std::string site;
                Identifier_st Identifier;
                std::string IdentifierString;
                Object_st(std::string site,std::string name,Identifier_st Identifier);
                std::string Object_Json(bool last);
                std::string _Json();
            };
            struct Link_st : public Object_st{
                std::string link;
                std::string linkLangauge;
                std::string mediaType;
                Link_st(std::string site,std::string name,Identifier_st Identifier);
                std::string Link_Json();
            };
            struct Actor_st : public Object_st{
                std::string username;
                Actor_st(std::string site,std::string name,Identifier_st Identifier);
                std::string Actor_Json();
            };
            struct Note_st : public Object_st{
                std::string published;
                std::string attributedTo;
                std::string inReplyTo;
                std::string content;
                std::string to;
                Note_st(std::string site,std::string name,std::string content,std::string published,std::string attributedTo,std::string to,Identifier_st Identifier);
                std::string Note_Json();
            };
            struct Activity_st : public Object_st{
                std::string summary;
                Actor_st* actor=nullptr;
                Object_st* object=nullptr;
                Activity_st(std::string site,std::string name,Actor_st* actor,std::deque<Actor_st*> audience,Identifier_st Identifier);
                std::string Activity_Json();
            };
            struct IntransitiveActivity_st : public Activity_st{
                IntransitiveActivity_st(std::string site,std::string name,Actor_st* actor,std::deque<Actor_st*> audience,Identifier_st Identifier);
                std::string IntransitiveActivity_Json();
            };
            struct Collection_st : public Object_st{
                std::string summary;
                std::deque<Object_st*> items;
                Collection_st(std::string site,std::string name,std::deque<Object_st*> items,Identifier_st Identifier);
                std::string Collection_Json();
            };
        };
    };
};