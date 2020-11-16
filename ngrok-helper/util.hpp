#pragma once
#include <Windows.h>

namespace util
{
	bool file_exists( const char* string );
	void to_clipboard( HWND hwnd, const std::string& s );
	void to_lowercase( std::string& string );
	void to_uppercase( std::string& string );
};