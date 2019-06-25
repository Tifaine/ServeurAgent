// file auto generated

#include "./sharedMem/sharedMem.h"
#include "./log/log.h"
#include "./freeOnExit/freeOnExit.h"
#include <stdint.h>
#include "serveurTCP/serveurTCP.h"
#include "./config/config_arg.h"
#include "./config/config_file.h"

// INIT_FUNCTION

int main ( int argc, char ** argv )
{
	// INIT_VAR
	serverTcpParams_t serverParams = {
		.port = 12345,
		.flag.work = true,
		.nbMaxClients = 30
	};
	struct
	{
		#ifdef __LOG_H__
		uint8_t help:1,
		quiet:1,
		verbose:1,
			#ifndef RELEASE_MODE
				color:1, // only available for debug mode
				debug:1, // only available for debug mode
			#else
				unused:2,
			#endif
				term:1,
				file:1;
		#else
				uint8_t help:1,
			unused:6; // to have same allignement with and without debug/color/quiet flags
		#endif
		}
		flags = { 0 };
	#ifdef __LOG_H__
		char logFileName[ 512 ] = "";
	#endif

		param_el param[] =
		{
			{ "--help", "-h", 0x01, cT ( bool ), &flags, "help" },
		#ifdef __LOG_H__
			{ "--quiet", "-q", 0x02, cT ( bool ), &flags, "quiet" },
			{ "--verbose", "-v", 0x04, cT ( bool ), &flags, "verbose" },
			#ifndef RELEASE_MODE
			{ "--color", "-c", 0x08, cT ( bool ), &flags, "color" },
			{ "--debug", "-d", 0x10, cT ( bool ), &flags, "debug" },
			#endif
			{ "--term", "-lT", 0x20, cT ( bool ), &flags, "log on term" },
			{ "--file", "-lF", 0x40, cT ( bool ), &flags, "log in file" },
			{ "--logFileName", "-lFN", 1, cT ( str ), logFileName, "log file name" },
		#endif
		};

	// INIT_CORE
	// INIT_FREEONEXIT
		if ( initFreeOnExit ( ) )
	{ // failure case
	}
	// END_FREEONEXIT
	
	// INIT_CONFIG
	flags.help = 0;
	if( readParamArgs ( argc, argv, param ) )
	{ // failure case
	}


	if ( flags.help )
	{// configFile read successfully
		helpParamArgs ( param );
	}
	// END_CONFIG
	// INIT_LOG
	#ifdef __CONFIG_DATA_H__
	logSetVerbose ( flags.verbose );
		#ifndef RELEASE_MODE
	logSetDebug ( flags.debug );
	logSetColor ( flags.color );
		#endif
	logSetQuiet ( flags.quiet );
	logSetOutput ( flags.term | !flags.file, flags.file );
	logSetFileName ( logFileName );
	#endif
	// END_LOG
	flags.debug = 1;
	printf ( "main start mode: %s\n", (flags.debug)?"debug":(flags.verbose)?"verbose":"normal" );
	printf ( "print: %s\n", (flags.file)?"file":"terminal" );
	logVerbose ( "Verbose Activated\n" );
	logDebug ( "Debug Activated\n" );

	launch_Server ( &serverParams );

	while(1){};
	
	// END_CORE
	return ( 0 );
}

