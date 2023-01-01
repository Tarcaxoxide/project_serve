#include<Database.hpp>

namespace Database{
	Manager_cl::Manager_cl(std::string FileName){
		DatabaseFile.FileName=FileName+".Sbin";
		AddEntry(DET_e::DET_END,(uint8_t[7]){'H','E','L','L','O',0,0});
		Sync(!Sync(false));
	}
	bool Manager_cl::Sync(bool RamToDisk){
		if(RamToDisk){
			std::ofstream oFile(DatabaseFile.FileName.c_str());
			Entry_st Entry{DET_e::DET_NULL,0,0,0,0,0,0,0};
			for(size_t z=0;z<DatabaseFile.Data.size();z++){
				Entry = DatabaseFile.Data[z];
				for(size_t i=0;i<sizeof(Entry_st);i++){
					oFile << ((uint8_t*)&Entry)[i];
				}
			}
			oFile.close();
			return Sync(false);
		}else{
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
				return true;
			}
			iFile.close();
			return false;
		}
	}
	void Manager_cl::AddEntry(const DET_e type,const uint8_t dataToWrite[7]){
		Entry_st Entry{type,0,0,0,0,0,0,0};
		Entry.Data[0] = dataToWrite[0];
		Entry.Data[1] = dataToWrite[1];
		Entry.Data[2] = dataToWrite[2];
		Entry.Data[3] = dataToWrite[3];
		Entry.Data[4] = dataToWrite[4];
		Entry.Data[5] = dataToWrite[5];
		Entry.Data[6] = dataToWrite[6];
		DatabaseFile.Data.push_back(Entry);
	}
	void Manager_cl::AddFile(std::deque<uint8_t> Data){
		size_t size=Data.size()/8;
		for(size_t a=0;a<size;a++){
			uint8_t tmp[8]{0x00};
			for(size_t b=0;b<8;b++){
				tmp[b]=Data[(a*8)+b];
			}
			AddEntry((DET_e)tmp[0],(uint8_t[7]){tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7]});
		}
		AddEntry(DET_e::DET_END,(uint8_t[7]){0,0,0,0,0,0,0});
	}
};
