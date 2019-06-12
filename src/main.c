// file auto generated

#include "./sharedMem/sharedMem.h"
#include "./freeOnExit/freeOnExit.h"
#include "serveurTCP/serveurTCP.h"
#include <stdbool.h>
// INIT_FUNCTION

int main ( int argc, char ** argv )
{
	// INIT_VAR

	serverTcpParams_t serverParams = {
		.port = 12345,
		.flag.work = true,
		.nbMaxClients = 30
	};
	// INIT_CORE

	
	// END_CORE
	// INIT_FREEONEXIT
	if ( initFreeOnExit ( ) )
	{ // failure case
	}
	// END_FREEONEXIT

	launch_Server ( &serverParams );

	while(1){};
	
	return ( 0 );
}

