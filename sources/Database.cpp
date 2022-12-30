#include<Database.hpp>

namespace Database{
	Manager_cl::Manager_cl(std::string FileName){
		DatabaseFile.FileName=FileName;
		Sync(false);
	}
	void Manager_cl::Sync(bool RamToDisk){
		if(RamToDisk){}else{
			std::ifstream iFile(DatabaseFile.FileName.c_str());
			if(iFile){
				if(DatabaseFile.Data.size())DatabaseFile.Data.clear();
				uint8_t rawData;
				Entry_st Entry{DET_e::DET_NULL,0,0,0,0,0,0,0};
				for(size_t i=0;iFile>>rawData;i++){
					if(!(i%8)){
						if(Entry.DatabaseEntryType != DET_e::DET_NULL){
							DatabaseFile.Data.push_back(Entry);
							Entry.DatabaseEntryType = DET_e::DET_NULL;
						}
						Entry.DatabaseEntryType=(DET_e)rawData;
					}else{
						Entry.Data[(i%8)-1]=rawData;
					}
				}	
				iFile.close();
			}else{
				iFile.close();
				std::ofstream oFile(DatabaseFile.FileName.c_str());
				oFile << DET_e::DET_END;
				oFile << 0x00;
				oFile << 0x00;
				oFile << 0x00;
				oFile << 0x00;
				oFile << 0x00;
				oFile << 0x00;
				oFile << 0x00;
				oFile.close();
				Sync(false);
			}
		}
	}
	void Manager_cl::AddEntry(DET_e type,std::deque<uint8_t> dataToWrite){}
};
