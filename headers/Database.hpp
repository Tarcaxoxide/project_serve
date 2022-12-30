#pragma once

#include<string>
#include<deque>
#include<fstream>

namespace Database{
	enum DET_e:uint8_t{
		DET_NULL=0x00,
		DET_END=0x01,
		DET_TEXT=0x02,
		DET_HTML=0x03,
		DET_PROFILE=0x04,
		DET_LINK=0x05,
		DET_APP=0x06
	};
    struct Entry_st{
		DET_e DatabaseEntryType;
		uint8_t Data[7];
	}__attribute__((packed));
	struct DatabaseFile_st{
		std::string FileName;
		std::deque<Entry_st> Data;
	};
	class Manager_cl{
		DatabaseFile_st DatabaseFile;
		public:
		Manager_cl(std::string FileName);
		void Sync(bool RamToDisk);
		void AddEntry(DET_e type,std::deque<uint8_t> dataToWrite);
    };
};
