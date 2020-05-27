#include <Windows.h>
#include <future>
#include <iostream>
#include <fstream>
#include <string>

/*
I can't run through memory so it sucks.
*/

bool main( )
{
	std::string authtoken;
	std::string port;
	std::ifstream file( "settings.ini" );

	SetConsoleTitle( ( "a software that uses ngrok as base" ) );
	std::cout << ( "welcome, my sir" ) << std::endl;
	std::cout << ( "big thanks to ngrok.com" ) << std::endl << std::endl;
	std::cout << ( "CTRL + C to close the ngrok application" ) << std::endl;

	/* wtf is this code */
	/* can be improved, checking ngrok.yml file */
	if ( !file.good( ) )
	{
		/* create the suspicious file */
		std::ofstream file( "settings.ini", std::ios::out | std::ios::trunc );
		file << "\n";
		file.close( );

		std::cout << ( "seems like it's your first time using this software.." ) << std::endl;

		file.open( "settings.ini", std::ios::in );

		if ( !file.is_open( ) )
			return false;

		std::cout << ( "enter your authtoken:" ) << std::endl;
		std::cout << ( "> " );
		std::cin >> authtoken;
		std::cout << std::endl;

		if ( !authtoken.empty( ) )
		{
			authtoken = "ngrok authtoken " + authtoken;
			const char* execute = authtoken.c_str( );
			std::system( execute );
			std::cout << std::endl;
		}
	}

	/* std::cin lul */
	std::cout << ( "enter the port that you want to open" ) << std::endl;
	std::cout << ("> ");
	std::cin >> port;
	std::cout << std::endl;

	port = "ngrok tcp " + port;

	const char* execute = port.c_str();

	// just gay people use system
	std::system( execute );

	while ( !GetAsyncKeyState( VK_END ) )
		std::this_thread::sleep_for( std::chrono::nanoseconds( 5000 ) );
}
