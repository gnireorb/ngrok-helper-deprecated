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
	int choice = 1;
	const char* execute;
	std::string authtoken;
	std::string port;
	std::ifstream file( "settings.ini" );

	SetConsoleTitle( ( "a software that uses ngrok as base" ) );
	std::cout << ( "welcome, my sir" ) << std::endl;
	std::cout << ( "big thanks to ngrok.com" ) << std::endl;
	std::cout << ( "CTRL + C to close the ngrok application" ) << std::endl << std::endl;

	/* wtf is this code */
	/* can be improved, checking ngrok.yml file */
	if ( !file.good( ) )
	{
		/* create the suspicious file */
		std::ofstream file( "settings.ini", std::ios::out | std::ios::trunc );
		file.close( );

		std::cout << ( "seems like it's your first time using this software.." ) << std::endl << std::endl;

		std::cout << ( "please select your region:" ) << std::endl << std::endl;
		std::cout << ( "[0] south america" ) << std::endl;
		std::cout << ( "[1] north america" ) << std::endl;
		std::cout << ( "[2] asia" ) << std::endl;
		std::cout << ( "[3] europe" ) << std::endl;
		std::cout << ( "[4] australia" ) << std::endl;
		std::cout << ( "[5] japan" ) << std::endl;
		std::cout << ( "[6] india" ) << std::endl;

		std::cout << ( "> " );
		std::cin >> choice;
		std::cout << std::endl;

		if ( choice > 6 || choice < 0 )
			return false;

		file.open( "settings.ini", std::ios::in );

		if ( !file.is_open( ) )
			return false;

		file << choice;

		file.close( );

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
	std::cout << ( "> " );
	std::cin >> port;
	std::cout << std::endl;

	switch ( choice )
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
		port = "ngrok tcp " + port + " --region ap";
		execute = port.c_str( );
		std::system( execute );
		break;
	case 3:
		port = "ngrok tcp " + port + " --region eu";
		execute = port.c_str( );
		std::system( execute );
		break;
	case 4:
		port = "ngrok tcp " + port + " --region au";
		execute = port.c_str( );
		std::system( execute );
		break;
	case 5:
		port = "ngrok tcp " + port + " --region jp";
		execute = port.c_str( );
		std::system( execute );
		break;
	case 6:
		port = "ngrok tcp " + port + " --region in";
		execute = port.c_str( );
		std::system( execute );
		break;
	}

	std::cout << ( "now you can press END to fully close the application" ) << std::endl;

	while ( !GetAsyncKeyState( VK_END ) )
		std::this_thread::sleep_for( std::chrono::nanoseconds( 5000 ) );
}
