#pragma once

#include<iostream>
#include<string>
#include<deque>
#include<Format.hpp>

namespace Data{
    struct File_st{
        std::string Name;
        std::string Type;// ('application/json','text/html')
        std::string Contents;
    };
    struct Folder_st{
        std::string Name;
        std::deque<File_st*> SubFiles;
        std::deque<Folder_st*> SubFolders;
        Folder_st(std::string name);
        ~Folder_st();
        void CreateFolder(std::string FolderName);
        void DeleteFolder(std::string FolderName);
        size_t FindFolder(std::string FolderName);
        void CreateFile(std::string FileName);
        void DeleteFile(std::string FileName);
        size_t FindFile(std::string FileName);
    };
    class ArtificialFileSystem_cl{
        Folder_st* KernelFolder;
        public:
        ArtificialFileSystem_cl();
        ~ArtificialFileSystem_cl();
        size_t Search(std::string Path,Folder_st*& Folder);
        size_t Search(std::string Path,File_st*& Folder);
        void Create(std::string Path);
        void Delete(std::string Path);
    };
    extern Data::ArtificialFileSystem_cl ArtificialFileSystem;
};