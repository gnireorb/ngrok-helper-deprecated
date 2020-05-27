#include <Windows.h>
#include <future>
#include <iostream>
#include <fstream>
#include <string>

/*
i can't run through memory so it sucks.
a lot of bad code practices
*/

bool create_file( const char* name );
bool write_to_file( const char* name, int choice );
int region_selector( );
bool authtoken( );
bool tunnel( const char* file_name );

bool main( )
{
	/* variables */
	const char* file_name = "settings.ini";
	int region = 0;

	/* file section */
	std::ifstream file( file_name );

	/* can be improved, checking ngrok.yml file */
	if ( !file.good( ) )
	{
		std::cout << ( "seems like it's your first time using this software.." ) << std::endl << std::endl;

		if ( !create_file( file_name ) )
			return false;

		region = region_selector( );

		if ( !write_to_file( file_name, region ) )
			return false;

		if ( !authtoken( ) )
			return false;
	}

	file.close( );

	tunnel( file_name );

	std::cout << ( "now you can press END to fully close the application" ) << std::endl;

	while ( !GetAsyncKeyState( VK_END ) )
		std::this_thread::sleep_for( std::chrono::nanoseconds( 5000 ) );
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
	std::cout << ( "please select your region:" ) << std::endl << std::endl;
	std::cout << ( "[0] south america" ) << std::endl << "[1] north america" << std::endl << "[2] europe" << std::endl;
	std::cout << ( "> " );
	std::cin >> region;
	std::cout << std::endl;

	if ( region > 6 || region < 0 )
		return false;

	return region;
}

bool authtoken( )
{
	std::string authtoken;

	std::cout << ( "enter your authtoken:" ) << std::endl;
	std::cout << ( "> " );
	std::cin >> authtoken;
	std::cout << std::endl;

	if ( !authtoken.empty( ) && authtoken.length( ) < 50 )
	{
		authtoken = "ngrok authtoken " + authtoken;
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

	std::cout << ( "enter the port that you want to open" ) << std::endl;
	std::cout << ( "> " );
	std::cin >> port;
	std::cout << std::endl;

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
		MessageBox( NULL, "please delete your settings.ini", "error", MB_OK | MB_ICONWARNING );
	}

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
