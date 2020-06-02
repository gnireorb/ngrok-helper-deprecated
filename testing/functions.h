#pragma once
#include <Windows.h>
#include <future>
#include <iostream>
#include <fstream>
#include <string>
#include <ShlObj.h>
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib") 

#define _CRT_SECURE_NO_WARNINGS

bool create_file( const char* name );
bool write_to_file( const char* name, int choice );
int region_selector( );
bool authtoken( );
bool tunnel( const char* file_name );
bool ngrok_yml( );
bool first_time( const char* file_name, int region );
bool start( );

bool start( )
{
	SetConsoleTitle( ( "ngrok" ) );
	std::cout << ( "seja bem-vindo, eu acho?" ) << std::endl << std::endl;

	return true;
}

bool first_time( const char* file_name, int region )
{
	bool file_exists = ngrok_yml( );
#if _DEBUG
	std::cout << ( "[file_exists]: " ) << file_exists << std::endl;
#endif
	if ( !file_exists )
	{
		std::cout << ( "parece ser a sua primeira vez usando essa aplicação.." ) << std::endl << std::endl;

		if ( !create_file( file_name ) )
			return false;

		region = region_selector( );

		if ( !write_to_file( file_name, region ) )
			return false;

		if ( !authtoken( ) )
			return false;
	}

	return true;
}

bool ngrok_yml( )
{
	TCHAR user_path[MAX_PATH];
	char ngrok_path[MAX_PATH] = "\\.ngrok2\\ngrok.yml";

	HRESULT hr = SHGetFolderPath( NULL, CSIDL_PROFILE, NULL, SHGFP_TYPE_CURRENT, user_path );
	if ( hr != NULL )
		return false;

#if _DEBUG
	std::cout << ( "[user_path]: " ) << user_path << std::endl;
#endif

	bool append = PathAppend( user_path, ngrok_path );
	if ( !append )
		return false;

#if _DEBUG
	std::cout << ( "[append]: " ) << append << std::endl;
#endif

#if _DEBUG
	std::cout << ( "[user_path]: " ) << user_path << std::endl;
#endif

	bool file_exists = PathFileExists( user_path );
	if ( !file_exists )
		return false;

#if _DEBUG
	std::cout << ( "[file_exists]: " ) << file_exists << std::endl;
#endif

	return file_exists;
}

bool create_file( const char* file_name )
{
	std::ofstream file( file_name, std::ios::out | std::ios::trunc );
	file.close( );
	return file.good( );
}

bool write_to_file( const char* file_name, int choice )
{
	std::ofstream file( file_name, std::ios::out | std::ios::trunc );
	std::string str = std::to_string( choice );
	file << str;
	file.close( );

	return true;
}

int region_selector( )
{
	int region;
	std::cout << ( "selecione sua região favorita:" ) << std::endl << std::endl;
	std::cout << ( "[0] america do sul" ) << std::endl << "[1] america do norte" << std::endl << "[2] europa" << std::endl;
	std::cout << ( "> " );
	std::cin >> region;
	std::cout << std::endl;

#if _DEBUG
	std::cout << ( "[region]: " ) << region << std::endl;
#endif

	if ( region > 6 || region < 0 )
		return false;

	return region;
}

bool authtoken( )
{
	std::string authtoken;

	std::cout << ( "digite o seu authtoken:" ) << std::endl;
	std::cout << ( "> " );
	std::cin >> authtoken;
	std::cout << std::endl;

#if _DEBUG
	std::cout << ( "[authtoken]: " ) << authtoken << std::endl;
#endif

	if ( !authtoken.empty( ) && authtoken.length( ) < 50 )
	{
		authtoken = "ngrok authtoken " + authtoken;
#if _DEBUG
		std::cout << ( "[authtoken]: " ) << authtoken << std::endl;
#endif
		const char* execute = authtoken.c_str( );
		std::system( execute );
		std::cout << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}

bool tunnel( const char* file_name )
{
	/* wtf */
	int region;
	std::string strregion;
	std::string port;
	const char* execute;

	std::cout << ( "coloque a porta que deseja abrir:" ) << std::endl;
	std::cout << ( "> " );
	std::cin >> port;
	std::cout << std::endl;

#if _DEBUG
	std::cout << ( "[port]: " ) << port << std::endl;
#endif

	/* ifstream is for read-only */
	std::ifstream file;
	file.open( file_name, std::ios::in );
	if ( !file.is_open( ) )
		return false;
	std::string strchoice;
	std::getline( file, strchoice, '\n' );
	file.close( );
	try
	{
		region = std::stoi( strchoice );
	}
	catch ( const std::exception& )
	{
		MessageBox( NULL, "por favor delete sua settings.ini", "erro", MB_OK | MB_ICONWARNING );
	}

#if _DEBUG
	std::cout << ( "[region]: " ) << region << std::endl;
#endif

	if ( region > 6 || region < 0 )
		return false;

	switch ( region )
	{
	case 0:
		port = "ngrok tcp " + port + " --region sa";
		execute = port.c_str( );
		std::system( execute );
		break;
	case 1:
		port = "ngrok tcp " + port + " --region na";
		execute = port.c_str( );
		std::system( execute );
		break;
	case 2:
		port = "ngrok tcp " + port + " --region eu";
		execute = port.c_str( );
		std::system( execute );
		break;
	}

	return true;
}
