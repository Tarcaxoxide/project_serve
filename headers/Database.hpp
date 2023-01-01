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
		bool Sync(bool RamToDisk);
		void AddEntry(const DET_e type,const uint8_t dataToWrite[7]);
		void AddFile(std::deque<uint8_t> Data);
	};
};


/*
	first entry is always a DET_END so if your traversing the data backwards
	  you wont fall off the front.
	Each file ends in a DET_END entry.
	DET_NULL is used for padding.
	######################
		DET_END,H,E,L,L,O,\0,\0
		DET_END,G,O,O,D,B,Y,E
	######################
*/