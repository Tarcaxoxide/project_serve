#include<iostream>
#include<string>
#include<deque>

#include<Network/Server.hpp>
#include<Format.hpp>
#include<Brain.hpp>
#include<Data/ArtificialFileSystem.hpp>

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
    std::thread ServerThread(RunServer,8083);
    std::string UserInput;

    while(Running){
        std::getline(std::cin,UserInput);
        
        std::deque<std::string> args=Format::split(UserInput," ");
        
        
        if(args[0] == "Exit"){
            Running=false;
        }
        else if(args[0] == "ArtificialFileSystem"||args[0] == "AFS"){
            if(args.size() < 2){
                std::cout<<"please provide a SubCommand, Type 'Commands' as the SubCommand for a list of SubCommands."<<std::endl;
            }
            else if(args[1] == "Commands"){
                std::cout<<"Create\t"<<"Creates a File or Folder (path taken as argument)."<<std::endl;
                std::cout<<"Delete\t"<<"Deletes a File or Folder (path taken as argument)."<<std::endl;
                std::cout<<"Find\t"<<"Finds if a File or Folder exists (path taken as argument)."<<std::endl;

            }
            else if(args[1] == "Find"){
                if(args.size() < 3){
                    std::cout<<"No path specified."<<std::endl;
                    continue;
                }
                std::deque<std::string> TokenizedPath=Format::split(args[2],"/");
                if(TokenizedPath[TokenizedPath.size()-1].size() > 0){
                    Data::File_st* File;
                    size_t FileIndex=Data::ArtificialFileSystem.Search(args[2],File);
                    if(FileIndex){
                        std::cout<<"File found"<<std::endl;
                    }else{
                        std::cout<<"File not found."<<std::endl;
                    }
                }else{
                    Data::Folder_st* Folder;
                    size_t FolderIndex=Data::ArtificialFileSystem.Search(args[2],Folder);
                    if(FolderIndex){
                        std::cout<<"Folder found."<<std::endl;
                    }else{
                        std::cout<<"Folder not found."<<std::endl;
                    }
                }
            }
            else if(args[1] == "Create"){
                if(args.size() < 3){
                    std::cout<<"No path specified."<<std::endl;
                    continue;
                }
                std::deque<std::string> TokenizedPath=Format::split(args[2],"/");
                if(TokenizedPath[TokenizedPath.size()-1].size() >0){
                    Data::File_st* File;
                    size_t FileIndex=Data::ArtificialFileSystem.Search(args[2],File);
                    if(FileIndex){
                        std::cout<<"File not created."<<std::endl;
                    }else{
                        Data::ArtificialFileSystem.Create(args[2]);
                        std::cout<<"File created."<<std::endl;
                    }
                }else{
                    Data::File_st* Folder;
                    size_t FolderIndex=Data::ArtificialFileSystem.Search(args[2],Folder);
                    if(FolderIndex){
                        std::cout<<"Folder not created."<<std::endl;
                    }else{
                        Data::ArtificialFileSystem.Create(args[2]);
                        std::cout<<"Folder created."<<std::endl;
                    }
                }
            }
            else if(args[1] == "Delete"){
                if(args.size() < 3){
                    std::cout<<"No path specified."<<std::endl;
                    continue;
                }
                std::deque<std::string> TokenizedPath=Format::split(args[2],"/");
                if(TokenizedPath[TokenizedPath.size()-1].size() >0){
                    Data::File_st* File;
                    size_t FileIndex=Data::ArtificialFileSystem.Search(args[2],File);
                    if(FileIndex){
                        std::cout<<"File deleted."<<std::endl;
                        Data::ArtificialFileSystem.Delete(args[2]);
                    }else{
                        std::cout<<"File not deleted."<<std::endl;
                    }
                }else{
                    Data::File_st* Folder;
                    size_t FolderIndex=Data::ArtificialFileSystem.Search(args[2],Folder);
                    if(FolderIndex){
                        std::cout<<"Folder deleted."<<std::endl;
                        Data::ArtificialFileSystem.Delete(args[2]);
                    }else{
                        std::cout<<"Folder not deleted."<<std::endl;
                    }
                }
            }
            else{
                
            }
        }
        else{std::cout<<"Uknown Command:\'"<<args[0]<<"\'"<<std::endl;}
    }
    sleep(30);
    exit(0);
}