#include "functions.hpp"

bool main( int argcount, char** argvar )
{
	/* variables */
	const char* file_name = "settings.ini";
	int region = 0;

	if ( !start( ) )
		return false;

	if ( argcount == 2 && ( std::string )argvar[ 1 ] == "--reset" )
	{
		first_time( file_name, region, true );
	}
	
	{
		first_time( file_name, region, false );
	}
	
	create_tunnel( file_name );

	std::cout << ( "feche o software usando a tecla END do teclado" ) << std::endl;
	
	while ( !GetAsyncKeyState( VK_END ) )
		std::this_thread::sleep_for( std::chrono::nanoseconds( 5000 ) );
		
}
