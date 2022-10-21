#pragma once

#include<iostream>
#include<string>
#include<deque>
#include<Network/Server.hpp>
#include<Format.hpp>
#include<Brain.hpp>
#include<Data/ArtificialFileSystem.hpp>

std::string Command(std::deque<std::string> args,bool& KeepGoing);