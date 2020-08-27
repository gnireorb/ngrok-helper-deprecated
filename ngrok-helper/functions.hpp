#pragma once
#include "../dependencies/HTTPRequest/HTTPRequest.hpp"
#include "../dependencies/rapidjson/document.h"
#include <Windows.h>
#include <string>
#include <future>
#include <iostream>
#include <fstream>
#include <string>
#include <tchar.h>

#define _CRT_SECURE_NO_WARNINGS

using namespace rapidjson;

bool init( );
bool create_file( const char* file_name );
bool write_to_file( const char* file_name, int integer );
bool create_tunnel( const char* file_name, int port, int region );
bool get_public_url( );
void to_clipboard( HWND hwnd, const std::string& s );

bool init( )
{
	SetConsoleTitle( ( "ngrok" ) );

	std::ifstream file( settings::file_name );

	if ( !file.good( ) )
	{
		if ( !create_file( settings::file_name ) )
			return false;

		if ( !write_to_file( settings::file_name, settings::region ) )
			return false;
	}

	return true;
}

bool create_file( const char* file_name )
{
	std::ofstream file( file_name, std::ios::out | std::ios::trunc );
	file.close( );
	return file.good( );
}

bool write_to_file( const char* file_name, int integer )
{
	std::ofstream file( file_name, std::ios::out | std::ios::trunc );
	std::string str = std::to_string( integer );
	file << str;
	file.close( );

	return true;
}

bool create_tunnel( const char* file_name, int port, int region )
{
	std::string str_port;
	std::string str_string;
	const char* execute;

	if ( region > 2 || region < 0 )
		return false;

	str_port = std::to_string( port );

	switch ( region )
	{
	case 0:
		str_string = "ngrok tcp " + str_port + " --region sa";
		execute = str_string.c_str();
		WinExec( execute, SW_HIDE );
		get_public_url( );
		break;
	case 1:
		str_string = "ngrok tcp " + str_port + " --region us";
		execute = str_string.c_str( );
		WinExec( execute, SW_HIDE );
		get_public_url( );		
		break;
	case 2:
		str_string = "ngrok tcp " + str_port + " --region eu";
		execute = str_string.c_str( );
		WinExec( execute, SW_HIDE );
		get_public_url( );
		break;
	}

	return true;
}

bool get_public_url( )
{
	std::string srcreq; 
	try
	{
		http::Request request( "http://127.0.0.1:4040/api/tunnels" );
		const http::Response response = request.send( "GET" );
		srcreq = std::string( response.body.begin( ), response.body.end( ) );
	}
	catch ( const std::exception& e )
	{
		std::cerr << "Request failed, error: " << e.what( ) << '\n';
	}
	const char* json = srcreq.c_str( );
	Document d;
	d.Parse( json );
	Value const& tunnels = d[ "tunnels" ];
	for ( auto& tunnel_url : tunnels.GetArray( ) )
	{
		std::string public_url = tunnel_url[ "public_url" ].GetString( );
		public_url.erase( 0, 6 );

		settings::ip_address = public_url;
	}

	return true;
}

/* https://www.cplusplus.com/forum/general/48837/#msg266980 */
void to_clipboard( HWND hwnd, const std::string& s ) {
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
