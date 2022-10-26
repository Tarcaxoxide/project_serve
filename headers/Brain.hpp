#pragma once

#include<Server.hpp>
#include<Format.hpp>
#include<iostream>
#include<string>
#include<string.h>
#include <Filesystem.hpp>

namespace Brain{
    void Initialize();
    std::string Think(std::string Input,Network::Socket_st* Client,Network::Server_cl& Server);
};