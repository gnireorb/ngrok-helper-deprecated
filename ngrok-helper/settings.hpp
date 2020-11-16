#pragma once
#include <string>

namespace settings
{
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
