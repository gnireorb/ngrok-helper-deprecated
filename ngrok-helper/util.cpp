#include <string>
#include <algorithm>

#include "util.hpp"

bool util::file_exists( const char* string )
{
	struct stat buffer;
	return ( stat( string, &buffer ) == 0 );
}

void util::to_clipboard( HWND hwnd, const std::string& s )
{
	OpenClipboard( hwnd );
	EmptyClipboard( );
	HGLOBAL hg = GlobalAlloc( GMEM_MOVEABLE, s.size( ) + 1 );
	if ( !hg ) { CloseClipboard( ); return; }
	memcpy( GlobalLock( hg ), s.c_str( ), s.size( ) + 1 );
	GlobalUnlock( hg );
	SetClipboardData( CF_TEXT, hg );
	CloseClipboard( );
	GlobalFree( hg );
}

void util::to_lowercase( std::string& string )
{
	std::transform( string.begin( ), string.end( ), string.begin( ), ::tolower );
}

void util::to_uppercase( std::string& string )
{
	std::transform( string.begin( ), string.end( ), string.begin( ), ::toupper );
}