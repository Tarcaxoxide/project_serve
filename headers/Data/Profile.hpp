#pragma once

#include<Network/Server.hpp>
#include<Format.hpp>

namespace Data{
    struct Profile_st{
        struct UserData_st{
            std::string Name;
            std::string ID;
        }UserData;
        struct ServerData_st{
            std::string SubDomain;
            std::string Domain;
            std::string TopLevelDomain;
        }ServerData;
        Profile_st(std::string SubDomain,std::string Domain,std::string TopLevelDomain,std::string Name,std::string ID){
            UserData.Name=Name;
            UserData.ID=ID;
            ServerData.SubDomain=SubDomain;
            ServerData.Domain=Domain;
            ServerData.TopLevelDomain=TopLevelDomain;
        }
        std::string operator()(){
            return std::string("{\"subject\":\"acct:"+UserData.Name+"@"+ServerData.SubDomain+"."+ServerData.Domain+"."+ServerData.TopLevelDomain+"\",\"links\":[{\"rel\":\"self\",\"type\":\"application/activity+json\",\"href\":\"https://"+ServerData.SubDomain+"."+ServerData.Domain+"."+ServerData.TopLevelDomain+"/users/"+UserData.ID+"\"},{\"rel\":\"http://webfinger.net/rel/profile-page\",\"type\":\"text/html\",\"href\":\"https://"+ServerData.SubDomain+"."+ServerData.Domain+"."+ServerData.TopLevelDomain+"/@"+UserData.Name+"\"},{\"rel\":\"http://ostatus.org/schema/1.0/subscribe\",\"template\":\"https://"+ServerData.SubDomain+"."+ServerData.Domain+"."+ServerData.TopLevelDomain+"/authorize-follow?acct={uri}\"}]");
        }
    };
};


/*
    {
        "subject":"acct:logan@www.loganjohndarylgraham.xyz",
        "links":[
                    {"rel":"self","type":"application/activity+json","href":"https://www.loganjohndarylgraham.xyz/users/8yhruqg7cl"},
                    {"rel":"http://webfinger.net/rel/profile-page","type":"text/html","href":"https://www.loganjohndarylgraham.xyz/@logan"},
                    {"rel":"http://ostatus.org/schema/1.0/subscribe","template":"https://www.loganjohndarylgraham.xyz/authorize-follow?acct={uri}"}
                 ]
    }
*/
