#include<Brain.hpp>

namespace Brain{

    void Initialize(){
        
    }
    std::string Think(std::string Input,Network::Socket_st* Client,Network::Server_cl& Server){
        std::string Ret{""},GetText{""},Get_UserName{""},Local_Profile_Location{""};
        
        // Compile the header into an object for easier handling.
        Format::RequestHeader_st RequestHeader(Input);
        
        
        return Ret;
    }
};