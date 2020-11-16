#pragma once
#include <string>

namespace ngrok
{
	bool init( );
	inline bool load_settings( );
	bool create_file( const char* file_name );
	bool write_to_file( const char* file_name, const char* json );
	const char* read_file( const char* file_name );
	bool create_tunnel( int port, int region );
	std::string get_public_url( );
}