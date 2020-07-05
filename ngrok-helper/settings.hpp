#pragma once
#include <string>

namespace settings
{
    std::string ip_address = "tunnel not created yet.";
    static const char* file_name = "settings.ini";
    static char authtoken[ 50 ];
    static int port;
    static int region = 0;
    const char* const regions[]
    {
        "south america",
        "north america",
        "europe",
    };
}
