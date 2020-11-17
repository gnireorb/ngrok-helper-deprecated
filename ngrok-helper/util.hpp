#pragma once
#include <Windows.h>

namespace util
{
	std::string read_file( std::string file_name );

	bool create_file( std::string file_name );
	bool write_to_file( std::string file_name, std::string json );
	bool file_exists( const char* string );
	void to_clipboard( HWND hwnd, const std::string& s );
	void to_lowercase( std::string& string );
	void to_uppercase( std::string& string );
};