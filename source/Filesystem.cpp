#include<Filesystem.hpp>



namespace Filesystem{//Folder_st
    Folder_st::Folder_st(std::string nName){
        Name=nName;
    }
    Folder_st::~Folder_st(){
        SubFiles.clear();
        SubFolders.clear();
    }
    Folder_st* Folder_st::FindSubFolder(std::string tName){
        for(size_t i=0;i<SubFolders.size();i++){
            if(SubFolders[i]->Name == tName)return SubFolders[i];
        }
        return nullptr;
    }
    File_st* Folder_st::FindSubFile(std::string tName){
        for(size_t i=0;i<SubFiles.size();i++){
            if(SubFiles[i]->Name == tName)return SubFiles[i];
        }
        return nullptr;
    }
    File_st* Folder_st::CreateFile(std::string tName){
        File_st* File = FindSubFile(tName);
        if(File != nullptr)return nullptr;//don't create a file if it exist already.
        if(tName.size() < 1)return nullptr;//don't create a file with a name of 0 length.
        SubFiles.push_back(new File_st{tName,"text/html",""});
        return SubFiles[SubFiles.size()-1];
    }
    Folder_st* Folder_st::CreateFolder(std::string tName){
        Folder_st* Folder = FindSubFolder(tName);
        if(Folder != nullptr)return nullptr;//don't create a folder if it exists already.
        if(tName.size() < 1)return nullptr;//don't create a folder with a name of 0 length.
        SubFolders.push_back(new Folder_st(tName));
        return SubFolders[SubFolders.size()-1];
    }
    bool Folder_st::DeleteFile(std::string tName){
        for(size_t i=0;i<SubFiles.size();i++){
            if(SubFiles[i]->Name == tName){
                SubFiles.erase(SubFiles.begin()+i);
                return true;
            }
        }
        return false;
    }
    bool Folder_st::DeleteFolder(std::string tName){
        for(size_t i=0;i<SubFolders.size();i++){
            if(SubFolders[i]->Name == tName){
                SubFolders.erase(SubFolders.begin()+i);
                return true;
            }
        }
        return false;
    }
};

namespace Filesystem{//FilesystemManager_cl
    FilesystemManager_cl::FilesystemManager_cl(){
        RootFolder=new Folder_st("/");
    }
    FilesystemManager_cl::~FilesystemManager_cl(){
        delete[] RootFolder;
    }
    File_st* FilesystemManager_cl::FileCreate(std::string Path){
        std::deque<std::string> TokenizedPath=Format::split(Path,"/");
        Folder_st* CurrentFolder=RootFolder;
        Folder_st* PreviousFolder=RootFolder;
        for(size_t i=0;i<TokenizedPath.size()-1/*last one is blank or a file*/;i++){
            PreviousFolder=CurrentFolder;
            CurrentFolder->FindSubFolder(TokenizedPath[i]);
            if(CurrentFolder == nullptr){
                CurrentFolder=PreviousFolder->CreateFolder(TokenizedPath[i]);
            }
        }
        return CurrentFolder->CreateFile(TokenizedPath[TokenizedPath.size()-1]);
    }
    Folder_st* FilesystemManager_cl::FolderCreate(std::string Path){
        std::deque<std::string> TokenizedPath=Format::split(Path,"/");
        bool Created=false;
        Folder_st* CurrentFolder=RootFolder;
        Folder_st* PreviousFolder=RootFolder;
        for(size_t i=0;i<TokenizedPath.size()-1/*last one is blank or a file*/;i++){
            PreviousFolder=CurrentFolder;
            CurrentFolder->FindSubFolder(TokenizedPath[i]);
            if(CurrentFolder == nullptr){
                Created=true;
                CurrentFolder=PreviousFolder->CreateFolder(TokenizedPath[i]);
            }
        }
        return (Created? CurrentFolder : nullptr);
    }
    File_st* FilesystemManager_cl::FileSearch(std::string Path){
        std::deque<std::string> TokenizedPath=Format::split(Path,"/");
        Folder_st* CurrentFolder=RootFolder;
        Folder_st* PreviousFolder=RootFolder;
        for(size_t i=0;i<TokenizedPath.size()-1/*last one is blank or a file*/;i++){
            PreviousFolder=CurrentFolder;
            CurrentFolder->FindSubFolder(TokenizedPath[i]);
            if(CurrentFolder == nullptr)return nullptr;
        }
        return CurrentFolder->FindSubFile(TokenizedPath[TokenizedPath.size()-1]);
    }
    Folder_st* FilesystemManager_cl::FolderSearch(std::string Path){
        std::deque<std::string> TokenizedPath=Format::split(Path,"/");
        Folder_st* CurrentFolder=RootFolder;
        Folder_st* PreviousFolder=RootFolder;
        for(size_t i=0;i<TokenizedPath.size()-1/*last one is blank or a file*/;i++){
            PreviousFolder=CurrentFolder;
            CurrentFolder->FindSubFolder(TokenizedPath[i]);
            if(CurrentFolder == nullptr)return nullptr;
        }
        return CurrentFolder;
    }
    bool FilesystemManager_cl::FileDelete(std::string Path){
        std::deque<std::string> TokenizedPath=Format::split(Path,"/");
        Folder_st* CurrentFolder=RootFolder;
        Folder_st* PreviousFolder=RootFolder;
        for(size_t i=0;i<TokenizedPath.size()-1/*last one is blank or a file*/;i++){
            PreviousFolder=CurrentFolder;
            CurrentFolder->FindSubFolder(TokenizedPath[i]);
            if(CurrentFolder == nullptr)return false;
        }
        return CurrentFolder->DeleteFile(TokenizedPath[TokenizedPath.size()-1]);
    }
    bool FilesystemManager_cl::FolderDelete(std::string Path){
        std::deque<std::string> TokenizedPath=Format::split(Path,"/");
        Folder_st* CurrentFolder=RootFolder;
        Folder_st* PreviousFolder=RootFolder;
        for(size_t i=0;i<TokenizedPath.size()-2/*last one is blank or a file,2nd from last is the target*/;i++){
            PreviousFolder=CurrentFolder;
            CurrentFolder->FindSubFolder(TokenizedPath[i]);
            if(CurrentFolder == nullptr)return false;
        }
        return CurrentFolder->DeleteFolder(TokenizedPath[TokenizedPath.size()-2]);
    }
};

namespace Filesystem{
    FilesystemManager_cl FilesystemManager;
};