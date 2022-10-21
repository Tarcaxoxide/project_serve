#include<Command.hpp>

std::string Command(std::deque<std::string> args,bool& KeepGoing){
    std::string Ret="";
    KeepGoing=true;
    if(args[0] == "Exit"){
            KeepGoing=false;
        }
        else if(args[0] == "ArtificialFileSystem"||args[0] == "AFS"){
            if(args.size() < 2){
                Ret+=std::string("please provide a SubCommand, Type 'Commands' as the SubCommand for a list of SubCommands.");
            }
            else if(args[1] == "Commands"){
                Ret+=std::string("Create\tCreates a File or Folder (path taken as argument).\n");
                Ret+=std::string("Delete\tDeletes a File or Folder (path taken as argument).\n");
                Ret+=std::string("Find\tFinds if a File or Folder exists (path taken as argument).\n");
                Ret+=std::string("Commands\tDisplays this message.");
            }
            else if(args[1] == "Find"){
                if(args.size() < 3){
                    Ret+=std::string("No path specified.");
                    //continue;
                }
                std::deque<std::string> TokenizedPath=Format::split(args[2],"/");
                if(TokenizedPath[TokenizedPath.size()-1].size() > 0){
                    Data::File_st* File;
                    size_t FileIndex=Data::ArtificialFileSystem.Search(args[2],File);
                    if(FileIndex){
                        Ret+=std::string("File found");
                    }else{
                        Ret+=std::string("File not found.");
                    }
                }else{
                    Data::Folder_st* Folder;
                    size_t FolderIndex=Data::ArtificialFileSystem.Search(args[2],Folder);
                    if(FolderIndex){
                        Ret+=std::string("Folder found.");
                    }else{
                        Ret+=std::string("Folder not found.");
                    }
                }
            }
            else if(args[1] == "Create"){
                if(args.size() < 3){
                    Ret+=std::string("No path specified.");
                    //continue;
                }
                std::deque<std::string> TokenizedPath=Format::split(args[2],"/");
                if(TokenizedPath[TokenizedPath.size()-1].size() >0){
                    Data::File_st* File;
                    size_t FileIndex=Data::ArtificialFileSystem.Search(args[2],File);
                    if(FileIndex){
                        Ret+=std::string("File not created.");
                    }else{
                        Data::ArtificialFileSystem.Create(args[2]);
                        Ret+=std::string("File created.");
                    }
                }else{
                    Data::File_st* Folder;
                    size_t FolderIndex=Data::ArtificialFileSystem.Search(args[2],Folder);
                    if(FolderIndex){
                        Ret+=std::string("Folder not created.");
                    }else{
                        Data::ArtificialFileSystem.Create(args[2]);
                        Ret+=std::string("Folder created.");
                    }
                }
            }
            else if(args[1] == "Delete"){
                if(args.size() < 3){
                    Ret+=std::string("No path specified.");
                    //continue;
                }
                std::deque<std::string> TokenizedPath=Format::split(args[2],"/");
                if(TokenizedPath[TokenizedPath.size()-1].size() >0){
                    Data::File_st* File;
                    size_t FileIndex=Data::ArtificialFileSystem.Search(args[2],File);
                    if(FileIndex){
                        Ret+=std::string("File deleted.");
                        Data::ArtificialFileSystem.Delete(args[2]);
                    }else{
                        Ret+=std::string("File not deleted.");
                    }
                }else{
                    Data::File_st* Folder;
                    size_t FolderIndex=Data::ArtificialFileSystem.Search(args[2],Folder);
                    if(FolderIndex){
                        Ret+=std::string("Folder deleted.");
                        Data::ArtificialFileSystem.Delete(args[2]);
                    }else{
                        Ret+=std::string("Folder not deleted.");
                    }
                }
            }
            else{
                
            }
        }
        else{Ret+=std::string("Uknown Command:\'")+args[0]+std::string("\'");}
        return Ret;
}