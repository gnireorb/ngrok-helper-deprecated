#pragma once
#include <string>

namespace settings
{
    const char* ip_address = "tunnel not yet created.";
    static char authtoken[ 50 ];
    static int port;
    static int region = 0;
    const char* const regions[]
    {
        "south america",
        "north america",
        "europe",
        "asia",
        "australia"
    };
}
