#pragma once

#include <thread>
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <deque>
#include <string>

namespace Network{
  struct Socket_st{
        int FileDescriptor;
        sockaddr_in SocketAddress;
        size_t SocketAddressLength;
        int Port;
        int Domain;
        int Type;
        int Protocol;
        int Backlog;
        struct Buffer_st{
          size_t SizeOfBuffer{0},BytesOfContent{0};
          char* ContentsOfBuffer{nullptr};
          void SetSize(size_t nSize);
          void Erase();
          void SetMessage(std::string nMessage);
        };
        Buffer_st* Buffer{nullptr};
    };
    class Server_cl{
      Socket_st Socket;
      void _Socket();
      void _Bind();
      void _Listen();
      public:
      void AcceptConnection(Socket_st*& Client);
      void ReadConnection(Socket_st*& Client);
      void WriteConnection(Socket_st*& Client);
      void CloseConnection(Socket_st*& Client);
      Server_cl(int port=9999,int backlog=10,int domain=AF_INET, int type=SOCK_STREAM, int protocol=0);
      ~Server_cl();
    };  
};