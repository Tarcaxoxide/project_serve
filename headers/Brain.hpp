#pragma once

#include<Network/Server.hpp>
#include<Format.hpp>
#include<Data/Profile.hpp>
#include<iostream>
#include<string>
#include<string.h>
#include<Data/ArtificialFileSystem.hpp>

namespace Brain{
    void Initialize();
    std::string Think(std::string Input,Network::Socket_st* Client,Network::Server_cl& Server);
};