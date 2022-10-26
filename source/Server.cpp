#include<Server.hpp>

namespace Network{
    void Socket_st::Buffer_st::Erase(){
        delete[] ContentsOfBuffer;
        ContentsOfBuffer=nullptr;
        BytesOfContent=0;
        SizeOfBuffer=0;
    }
    void Socket_st::Buffer_st::SetSize(size_t nSize){
        if(ContentsOfBuffer != nullptr)Erase();
        ContentsOfBuffer=new char[nSize+1]{0};
        SizeOfBuffer=nSize;
    }
    void Socket_st::Buffer_st::SetMessage(std::string Message){
        SetSize(Message.size());
        for(;BytesOfContent<Message.size();BytesOfContent++){
            ContentsOfBuffer[BytesOfContent] = Message[BytesOfContent];
        }
    }
    Server_cl::Server_cl(int port,int backlog,int domain, int type, int protocol){
        Socket.Port=port;
        Socket.Domain=domain;
        Socket.Type=type;
        Socket.Protocol=protocol;
        Socket.Backlog=backlog;

        _Socket();
        _Bind();
        _Listen();
    }
    void Server_cl::_Socket(){
        Socket.FileDescriptor=socket(Socket.Domain, Socket.Type, Socket.Protocol);
        if (Socket.FileDescriptor == -1) {
          perror( std::string("Failed to create socket.").c_str());
          exit(EXIT_FAILURE);
        }
        std::cout << "Created socket." << std::endl;
        Socket.SocketAddress.sin_family = Socket.Domain;
        Socket.SocketAddress.sin_addr.s_addr = INADDR_ANY;
        Socket.SocketAddress.sin_port = htons(Socket.Port);
        Socket.SocketAddressLength = sizeof(Socket.SocketAddress);
    }
    void Server_cl::_Bind(){
        while(bind(Socket.FileDescriptor, (struct sockaddr*)&Socket.SocketAddress, Socket.SocketAddressLength) < 0){
            perror( std::string(std::string("Failed to bind to port ")+std::to_string(Socket.Port)).c_str() );
            sleep(1);
        }
        std::cout << "Bound to port "<<Socket.Port<< std::endl;
    }
    void Server_cl::_Listen(){
        if (listen(Socket.FileDescriptor, Socket.Backlog) < 0) {
          perror( std::string("Failed to listen on socket.").c_str());
          exit(EXIT_FAILURE);
        }
        std::cout << "Listen on socket." << std::endl;
    }
    void Server_cl::AcceptConnection(Socket_st*& Client){
        if(Client != nullptr){CloseConnection(Client);}

        Client=new Socket_st;

        Client->FileDescriptor = accept(Socket.FileDescriptor, (struct sockaddr*)&Client->SocketAddress, (socklen_t*)&Client->SocketAddressLength);
        if (Client->FileDescriptor < 0) {
          std::cout << "Failed to grab connection. errno: " << errno << std::endl;
          perror( std::string("Failed to grab connection.").c_str());
          exit(EXIT_FAILURE);
        }

        Client->Port = ntohs(Client->SocketAddress.sin_port);
        Client->SocketAddress.sin_addr.s_addr = Socket.SocketAddress.sin_addr.s_addr;
        Client->SocketAddress.sin_family = Socket.SocketAddress.sin_family;
        Client->SocketAddressLength = sizeof(Client->SocketAddress);
        Client->Buffer = new Socket_st::Buffer_st;

    }
    void Server_cl::ReadConnection(Socket_st*& Client){
        Client->Buffer->BytesOfContent = read(Client->FileDescriptor, Client->Buffer->ContentsOfBuffer, Client->Buffer->SizeOfBuffer);
    }
    void Server_cl::WriteConnection(Socket_st*& Client){
        send(Client->FileDescriptor, Client->Buffer->ContentsOfBuffer, Client->Buffer->BytesOfContent, 0);
    }
    void Server_cl::CloseConnection(Socket_st*& Client){
        close(Client->FileDescriptor);
        delete[] Client;
        Client=nullptr;
    }
    Server_cl::~Server_cl(){
        close(Socket.FileDescriptor);
    }
};