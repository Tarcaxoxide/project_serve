#include<Data/ArtificialFileSystem.hpp>

namespace Data{
    Folder_st::Folder_st(std::string name){Name=name;}
    Folder_st::~Folder_st(){
        for(size_t i=0;i<SubFiles.size();i++){
            delete[] SubFiles[i];
        }
        for(size_t i=0;i<SubFolders.size();i++){
            delete[] SubFolders[i];
        }
        SubFiles.clear();
        SubFolders.clear();
    }
    void Folder_st::CreateFolder(std::string FolderName){
        size_t FolderIndex=FindFolder(FolderName);
        if(FolderIndex)return;
        SubFolders.push_back(new Folder_st(FolderName));
    }
    void Folder_st::DeleteFolder(std::string FolderName){
        size_t FolderIndex=FindFolder(FolderName);
        if(FolderIndex){
            delete[] SubFolders[FolderIndex-1];
            SubFolders.erase(SubFolders.begin()+FolderIndex-1);
        }
    }
    size_t Folder_st::FindFolder(std::string FolderName){
        for(size_t index=0;index<SubFolders.size();index++){
            if(FolderName == SubFolders[index]->Name)return index+1;
        }
        return 0;
    }
    void Folder_st::CreateFile(std::string FileName){
        size_t FileIndex=FindFile(FileName);
        if(FileIndex)return;
        SubFiles.push_back(new File_st({FileName,"text/html",""}));
    }
    void Folder_st::DeleteFile(std::string FileName){
        size_t FileIndex=FindFile(FileName);
        if(FileIndex){
            delete[] SubFiles[FileIndex-1];
            SubFiles.erase(SubFiles.begin()+FileIndex-1);
        }
    }
    size_t Folder_st::FindFile(std::string FileName){
        for(size_t index=0;index<SubFiles.size();index++){
            if(FileName == SubFiles[index]->Name)return index+1;
        }
        return 0;
    }
    ArtificialFileSystem_cl::ArtificialFileSystem_cl(){
        KernelFolder = new Folder_st("KernelFolder");
    }
    ArtificialFileSystem_cl::~ArtificialFileSystem_cl(){
        delete[] KernelFolder;
    }
    size_t ArtificialFileSystem_cl::Search(std::string Path,Folder_st*& Folder){
        std::deque<std::string> TokenizedPath=Format::split(Path,"/");
        Folder_st* CurrentFolder=KernelFolder;
        size_t FolderIndex;
        for(size_t i=0;i<TokenizedPath.size()-1/*the last one will always be blank or a file*/;i++){
            FolderIndex=CurrentFolder->FindFolder(TokenizedPath[i]);
            if(FolderIndex){
                CurrentFolder=CurrentFolder->SubFolders[FolderIndex-1];
                Folder=CurrentFolder;
            }
        }
        return FolderIndex;
    }
    size_t ArtificialFileSystem_cl::Search(std::string Path,File_st*& File){
        Folder_st* Folder=KernelFolder;
        std::deque<std::string> TokenizedPath=Format::split(Path,"/");
        size_t FolderIndex=Search(Path,Folder);
        if(FolderIndex == 0)return 0;

        size_t FileIndex=Folder->FindFile(TokenizedPath[TokenizedPath.size()-1]);
        if(FileIndex){
            File=Folder->SubFiles[FileIndex-1];
            return FileIndex;
        }
        return 0;
    }
    void ArtificialFileSystem_cl::Create(std::string Path){
        std::deque<std::string> TokenizedPath=Format::split(Path,"/");
        Folder_st* Folder=KernelFolder;
        size_t FolderIndex=Search(Path,Folder);
        for(size_t i=FolderIndex;i<TokenizedPath.size()-1;i++){
            Folder->CreateFolder(TokenizedPath[i]);
            Folder=Folder->SubFolders.back();
        }
        if(TokenizedPath[TokenizedPath.size()-1].size() >0){
            Folder->CreateFile(TokenizedPath[TokenizedPath.size()-1]);
        }
    }
    void ArtificialFileSystem_cl::Delete(std::string Path){}
    Data::ArtificialFileSystem_cl ArtificialFileSystem;
};