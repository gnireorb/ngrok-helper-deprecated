#pragma once
#include "../dependencies/HTTPRequest/HTTPRequest.hpp"
#include "../dependencies/rapidjson/document.h"
#include "../dependencies/rapidjson/stringbuffer.h"
#include "../dependencies/rapidjson/writer.h"
#include "../dependencies/rapidjson/document.h"
#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <tchar.h>
#include <future>

using namespace rapidjson;

inline bool init( );
inline bool get_public_url( );
inline bool load_settings( );
inline bool create_file( const char* file_name );
inline bool write_to_file( const char* file_name, const char* json );
inline bool create_tunnel( int port, int region );
inline bool file_exists( const std::string& file_name );
inline void to_clipboard( HWND hwnd, const std::string& s );
inline const char* read_file( const char* file_name );

inline bool init( )
{
	SetConsoleTitle( ( "ngrok" ) );

	if ( !file_exists( "ngrok.exe" ) )
	{
		MessageBox( NULL, "ngrok.exe é inexistente, baixe-o em 'ngrok.com'.", "erro", MB_ICONERROR );
		exit( -1 );
	}

	if ( !file_exists( "settings.json" ) )
	{
		if ( !create_file( "settings.json" ) )
			return false;

		if ( !write_to_file( "settings.json", "{\"version\":\"0.4\",\"last_port\":0,\"ngrok_region\":0}" ) )
			return false;

		if ( !load_settings( ) )
			return false;
	}

	if ( !load_settings( ) )
		return false;

	return true;
}

inline bool load_settings( )
{
	Document doc;
	doc.Parse( read_file( "settings.json" ) );

	std::cout << "[version]: " << doc[ "version" ].GetString( ) << std::endl;

	settings::region = doc[ "ngrok_region" ].GetInt( );
	settings::port = doc[ "last_port" ].GetInt( );

	return true;
}

inline bool create_file( const char* file_name )
{
	std::ofstream file( file_name, std::ios::out | std::ios::trunc );
	file.close( );
	return file.good( );
}

inline bool write_to_file( const char* file_name, const char* json )
{
	std::ofstream file( file_name, std::ios::out | std::ios::trunc );
	std::string str = json;
	file << str;
	file.close( );
	return true;
}

inline const char* read_file( const char* file_name )
{
	std::string output;
	std::ifstream file;
	file.open( file_name, std::ios::in );
	if ( !file.is_open( ) )
	{
		MessageBox( NULL, "ocorreu uma falha ao abrir 'settings.json'.", "erro", MB_ICONERROR );
		exit( -1 );
	}
	std::getline( file, output );
	file.close( );
	return output.c_str( );
}

inline bool create_tunnel( int port, int region )
{
	std::string commandline;

	if ( region >= 4 || region < 0 )
		return false;

	switch ( region )
	{
	case 0:
		commandline = "ngrok tcp " + std::to_string( port ) + " --region sa";
		if ( WinExec( commandline.c_str( ), SW_HIDE ) >= 31 )
			std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
		get_public_url( );
		break;
	case 1:
		commandline = "ngrok tcp " + std::to_string( port ) + " --region us";
		if ( WinExec( commandline.c_str( ), SW_HIDE ) >= 31 )
			std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
		get_public_url( );
		break;
	case 2:
		commandline = "ngrok tcp " + std::to_string( port ) + " --region eu";
		if ( WinExec( commandline.c_str( ), SW_HIDE ) >= 31 )
			std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
		get_public_url( );
		break;
	case 3:
		commandline = "ngrok tcp " + std::to_string( port ) + " --region ap";
		if ( WinExec( commandline.c_str( ), SW_HIDE ) >= 31 )
			std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
		get_public_url( );
		break;
	case 4:
		commandline = "ngrok tcp " + std::to_string( port ) + " --region au";
		if ( WinExec( commandline.c_str( ), SW_HIDE ) >= 31 )
			std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
		get_public_url( );
		break;
	}

	return true;
}

inline bool get_public_url( )
{
	std::string api_request;
	try
	{
		http::Request request( "http://127.0.0.1:4040/api/tunnels" );
		const http::Response response = request.send( "GET" );
		api_request = std::string( response.body.begin( ), response.body.end( ) );
	}
	catch ( const std::exception& e )
	{
		std::cerr << "request failed, error: " << e.what( ) << '\n';
	}
	const char* json = api_request.c_str( );
	Document d;
	d.Parse( json );
	Value const& tunnels = d[ "tunnels" ];
	for ( auto& tunnel_url : tunnels.GetArray( ) )
	{
		std::string public_url = tunnel_url[ "public_url" ].GetString( );
		public_url.erase( 0, 6 );

		settings::ip_address = public_url.c_str( );
	}

	return true;
}

/* https://www.cplusplus.com/forum/general/48837/#msg266980 */
inline void to_clipboard( HWND hwnd, const std::string& s ) {
	OpenClipboard( hwnd );
	EmptyClipboard( );
	HGLOBAL hg = GlobalAlloc( GMEM_MOVEABLE, s.size( ) + 1 );
	if ( !hg )
	{
		CloseClipboard( );
		return;
	}
	memcpy( GlobalLock( hg ), s.c_str( ), s.size( ) + 1 );
	GlobalUnlock( hg );
	SetClipboardData( CF_TEXT, hg );
	CloseClipboard( );
	GlobalFree( hg );
}

inline bool file_exists( const std::string& file_name ) {
	struct stat buffer;
	return ( stat( file_name.c_str( ), &buffer ) == 0 );
}
