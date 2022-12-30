#pragma once

#include<string>
#include<deque>
#include<fstream>

typedef std::deque<std::string> RamFile_t;

namespace Database{
    struct DatabaseEntry_st{
        std::string Name;
        RamFile_t RamFile;
    };
    class Manager_cl{
        std::string BaseDirectory;
        std::deque<DatabaseEntry_st> Database;
        public:
            Manager_cl(std::string BaseDirectory);
            bool Load_File(std::string FileName);
            bool Save_File(std::string FileName);
            bool Create_File(std::string FileName);
            
            RamFile_t* Find_File(std::string FileName);
    };
};