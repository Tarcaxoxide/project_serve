#include<Brain.hpp>

namespace Brain{

    void Initialize(){
        
    }
    std::string Think(std::string Input,Network::Socket_st* Client,Network::Server_cl& Server){
        std::string Ret{""},GetText{""},Get_UserName{""},Local_Profile_Location{""};
        
        // Compile the header into an object for easier handling.
        Format::RequestHeader_st RequestHeader(Input);
        
        // Get the requested url from the header object.
        GetText=Format::urlDecode(RequestHeader.Property("GET")[0]);
        // try to parse out a username.
        bool isUserName=false;
        for(size_t i=0;i<GetText.size();i++){
            if(GetText[i]=='@')isUserName=false;
            if(isUserName)Get_UserName+=GetText[i];
            if(GetText[i]=='a' && GetText[i+1]=='c' && GetText[i+2]=='c' && GetText[i+3]=='t' && GetText[i+4]==':'){i+=4;isUserName=true;}
            // /Article/0000000000000002/0000000000000001
            if(GetText[i]=='/' && GetText[i+1]=='A' && GetText[i+2]=='r' && GetText[i+3]=='t' && GetText[i+4]=='i' && GetText[i+5]=='c' && GetText[i+6]=='l' && GetText[i+7]=='e' && GetText[i+8]=='/'){
                for(size_t ib=i+1;ib<GetText.size();ib++){
                    Local_Profile_Location+=GetText[ib];
                }
                break;
            }
        }
        
        // See if the user exist
        std::ifstream MainRecord;
        MainRecord.open("Article/MainRecord");
        if(MainRecord.is_open()){
            std::string line;
            size_t iline=0;
            bool usernameFound=false;
            while(getline(MainRecord, line)) {
                std::cout<<iline<<":"<<((iline%2)? "location" : "username")<<":"<<line<<std::endl;

                if(usernameFound){
                    Local_Profile_Location=line;
                }
                usernameFound=(!(iline%2) && line == Get_UserName);
                iline++;
            }
            MainRecord.close();
        }
        // return profile if one exist?
        if(Local_Profile_Location != ""){
            std::ifstream Profile;
            Profile.open(std::string(Local_Profile_Location+std::string("/Profile.json")).c_str());
            if(Profile.is_open()){
                std::string line;
                Ret="";
                while(getline(Profile, line))Ret+=line;
                Format::AddHeader(Ret,"application/json",Format::HeaderCodes::OK);
                Profile.close();
            }
        }


        // logs
        std::cout<<"Time: "<<Format::currentDateAndTime()<<std::endl;
        std::cout<<"Request Header:{\n"<<Format::urlDecode(RequestHeader.AllProperties())<<"\n}"<<std::endl;
        std::cout<<"Get text{"<<GetText<<"}"<<std::endl;
        std::cout<<"Get user{"<<Get_UserName<<"}"<<std::endl;
        std::cout<<"Local Profile Location{"<<Local_Profile_Location<<"}"<<std::endl;
        std::cout<<"Ret{"<<Ret<<"}"<<std::endl;
        return Ret;
    }
};