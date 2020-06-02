#include "functions.h"

bool main( )
{
	/* variables */
	const char* file_name = "settings.ini";
	int region = 0;

	{
		start( );
		first_time( file_name, region );
	}
	
	tunnel( file_name );

	std::cout << ( "feche a aplicação usando o botão END do teclado" ) << std::endl;

	while ( !GetAsyncKeyState( VK_END ) )
		std::this_thread::sleep_for( std::chrono::nanoseconds( 5000 ) );
}
