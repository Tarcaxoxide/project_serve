#pragma once

#include <Server.hpp>
#include <Format.hpp>
#include <iostream>
#include <string>
#include <string.h>
#include <Filesystem.hpp>
#include <Variables.hpp>



// GET /test.html HTTP/1.1
// Host: localhost:8083
// User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:104.0) Gecko/20100101 Firefox/104.0
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
// Accept-Language: en-US,en;q=0.5
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive
// Cookie: __Host-pleroma_key=[insert some long string of random letters]
// Upgrade-Insecure-Requests: 1
// Sec-Fetch-Dest: document
// Sec-Fetch-Mode: navigate
// Sec-Fetch-Site: none
// Sec-Fetch-User: ?1


namespace Brain{
    

    void Initialize();
    std::string Think(std::string Input,Network::Socket_st* Client,Network::Server_cl& Server);
};