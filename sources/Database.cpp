#include<Database.hpp>

namespace Database{
	Manager_cl::Manager_cl(std::string FileName){
		DatabaseFile.FileName=FileName;
		std::ifstream iFile(DatabaseFile.FileName.c_str());
		if(iFile){
			uint8_t rawData;
			Entry_st Entry{DatabaseEntryType_e::DET_NULL,0,0,0,0,0,0,0};
			for(size_t i=0;iFile>>rawData;i++){
				if(!(i%8)){
					if(Entry.DatabaseEntryType != DatabaseEntryType_e::DET_NULL){
						DatabaseFile.Data.push_back(Entry);
						Entry.DatabaseEntryType = DatabaseEntryType_e::DET_NULL;
					}
					Entry.DatabaseEntryType=(DatabaseEntryType_e)rawData;
				}else{
					Entry.Data[(i%8)-1]=rawData;
				}
			}	
			iFile.close();
		}else{
			iFile.close();
			std::ofstream oFile(DatabaseFile.FileName.c_str());
			for(size_t i=0;i<8;i++){
				oFile << 0x00;
			}
			oFile.close();
		}
	}
};
