#pragma once
#include <string>

namespace ngrok
{
	bool init( );
	bool load_ngrok_settings( );
	bool create_tunnel( int port, int region );
	std::string get_public_url( );
}