#include<Database.hpp>

namespace Database{
    Manager_cl::Manager_cl(std::string BaseDirectory){
        this->BaseDirectory=BaseDirectory;

    }
    bool Manager_cl::Load_File(std::string FileName){
        RamFile_t* RamFile = Find_File(FileName);
        if(RamFile == nullptr)return false;
        FileName=BaseDirectory+FileName;
        std::ifstream DiskFile(FileName.c_str());
        std::string Line;while(DiskFile >> Line){
            (*RamFile).push_back(Line);
        }
        DiskFile.close();
        return true;
    }
    bool Manager_cl::Save_File(std::string FileName){
        RamFile_t* RamFile = Find_File(FileName);
        if(RamFile == nullptr)return false;
        FileName=BaseDirectory+FileName;
        std::ofstream DiskFile(FileName.c_str());
        for(size_t i=0;i<(*RamFile).size();i++){
            DiskFile << (*RamFile)[i] << std::endl;
        }
        DiskFile.close();
        return true;
    }
    RamFile_t* Manager_cl::Find_File(std::string FileName){
        for(size_t i=0;i<Database.size();i++){
            if(Database[i].Name == FileName)return &Database[i].RamFile;
        }
        return nullptr;
    }
    bool Manager_cl::Create_File(std::string FileName){
        RamFile_t* RamFile = Find_File(FileName);
        if(RamFile != nullptr)return false;
        Database.push_back({FileName,{}});
        return true;
    }
};