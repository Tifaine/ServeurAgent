// file auto generated

#include "./sharedMem/sharedMem.h"
#include "./log/log.h"
#include "./freeOnExit/freeOnExit.h"
#include "serveurTCP/serveurTCP.h"

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
	// INIT_LOG
	#ifdef __CONFIG_DATA_H__
	logSetVerbose ( flags.verbose );
	logSetDebug ( flags.debug );
	logSetColor ( flags.color );
	logSetQuiet ( flags.quiet );
	logSetOutput ( flags.term, flags.file );
	logSetFileName ( logFileName );
	#else
	logSetVerbose ( 1 );
	logSetDebug ( 1 );
	logSetColor ( 1 );
	logSetQuiet ( 0 );
	logSetOutput ( 1, 1 );
	logSetFileName ( "log.txt" );
	#endif
	// END_LOG
	
	// END_CORE
	// INIT_FREEONEXIT
	if ( initFreeOnExit ( ) )
	{ // failure case
	}
	// END_FREEONEXIT

	printf("Hello world\n");
	launch_Server ( &serverParams );

	while(1){};
	
	return ( 0 );
}

