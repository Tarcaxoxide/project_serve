#pragma once

#include <string>
#include <deque>

namespace Network{
    namespace Activitypub{
        namespace ObjectDefinitions{
            struct Object_st{
                // {
                //   "@context": "https://www.w3.org/ns/activitystreams",
                std::string context;
                //   "type": "Object",
                std::string type;
                //   "id": "http://www.test.example/object/1",
                std::string id;
                //   "name": "A Simple, non-specific object"
                std::string name;
                // }
            };
            struct Actor_st : public Object_st{
                // {
                //     "@context": "https://www.w3.org/ns/activitystreams",
                //     "type": "Person",
                //     "id": "https://social.example/alyssa/",
                //     "name": "Alyssa P. Hacker",
                //     "preferredUsername": "alyssa",
                std::string preferredUsername;
                //     "summary": "Lisp enthusiast hailing from MIT",
                std::string summary;
                //     "inbox": "https://social.example/alyssa/inbox/",
                std::string inbox;
                //     "outbox": "https://social.example/alyssa/outbox/",
                std::string outbox;
                //     "followers": "https://social.example/alyssa/followers/",
                std::string followers;
                //     "following": "https://social.example/alyssa/following/",
                std::string following;
                //     "liked": "https://social.example/alyssa/liked/"
                std::string liked;
                // }
            };
            struct Note_st : public Object_st{
                // {
                //     "@context": "https://www.w3.org/ns/activitystreams",
                //     "type": "Note",
                //     "to": ["https://chatty.example/ben/"],
                std::deque<std::string> to;
                //     "attributedTo": "https://social.example/alyssa/",
                std::string attributedTo;
                //     "content": "Say, did you finish reading that book I lent you?"
                std::string content;
                // }
            };
            struct Activity_st : public Object_st{
                // {
                //     "@context": "https://www.w3.org/ns/activitystreams",
                //     "type": "Create",
                //     "id": "https://social.example/alyssa/posts/a29a6843-9feb-4c74-a7f7-081b9c9201d3",
                //     "to": ["https://chatty.example/ben/"],
                std::deque<std::string> to;
                //     "actor": "https://social.example/alyssa/",
                std::string actor;
                //     "object": {
                //         "type": "Note",
                //         "id": "https://social.example/alyssa/posts/49e2d03d-b53a-4c4c-a95c-94a6abf45a19",
                //         "attributedTo": "https://social.example/alyssa/",
                //         "to": ["https://chatty.example/ben/"],
                //         "content": "Say, did you finish reading that book I lent you?"
                //     }
                Object_st* object;
                // }
            };
        };

        ObjectDefinitions::Activity_st* Create_CreateNote_Activity(std::string site,std::string actor,std::string ActivityID,std::string NoteID,std::string message,std::deque<std::string> SendTo);
        std::string Activity_ToString(ObjectDefinitions::Activity_st* Activity);
    };
};