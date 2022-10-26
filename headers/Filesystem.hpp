#pragma once

#include <string>
#include <deque>
#include <Format.hpp>
#include <Variables.hpp>

namespace Filesystem{
    struct File_st{
        std::string Name;
        std::string ContentType;
        std::string Contents;
    };
    struct Folder_st{
        std::string Name;
        std::deque<File_st*> SubFiles;
        std::deque<Folder_st*> SubFolders;
        Folder_st(std::string nName);
        ~Folder_st();
        Folder_st* FindSubFolder(std::string tName);
        File_st* FindSubFile(std::string tName);
        File_st* CreateFile(std::string tName);
        Folder_st* CreateFolder(std::string tName);
        bool DeleteFile(std::string tName);
        bool DeleteFolder(std::string tName);
    };
    class FilesystemManager_cl{
        Folder_st* RootFolder;
        public:
        FilesystemManager_cl();
        ~FilesystemManager_cl();
        File_st* FileCreate(std::string Path);
        Folder_st* FolderCreate(std::string Path);
        File_st* FileSearch(std::string Path);
        Folder_st* FolderSearch(std::string Path);
        bool FileDelete(std::string Path);
        bool FolderDelete(std::string Path);
    };
    extern FilesystemManager_cl FilesystemManager;
};