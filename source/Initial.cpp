#include<iostream>
#include<string>
#include<deque>

#include<Server.hpp>
#include<Format.hpp>
#include<Brain.hpp>
#include<Command.hpp>

static bool Running{true};

void RunServer(int PortNumber){
    Network::Server_cl Server(PortNumber);
    Network::Socket_st* Client=nullptr;

    while(Running){
        Server.AcceptConnection(Client);
        Client->Buffer->SetSize(1024*4);
        Server.ReadConnection(Client);

        if(Client->Buffer->BytesOfContent){
            std::string msg=Brain::Think(std::string(Client->Buffer->ContentsOfBuffer),Client,Server);
            Client->Buffer->SetMessage(msg);
        }
        Server.WriteConnection(Client);
        Server.CloseConnection(Client);
    }
    exit(0);
}


int main(){
    Brain::Initialize();
    Shell::Initialize();
    std::thread ServerThread(RunServer,8083);
    std::string UserInput;

    while(Running){
        std::getline(std::cin,UserInput);
        
        std::deque<std::string> args=Format::split(UserInput," ");
        std::string r=Shell::Command(args,Running);
        std::cout<<r<<std::endl;
    }
    sleep(30);
    exit(0);
}