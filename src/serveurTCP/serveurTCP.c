#include <arpa/inet.h>	//close
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>   //strlen
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>   //close
#include <stdbool.h>
#include "../Pub_Sub_Managment/PS_Managment.h"
#include "serveurTCP.h"

#define NB_ELEMENT_MESSAGE_INIT			5
#define NB_ELEMENT_MESSAGE_TO_TRANSMIT 	6

static void * server_TCP ( serverTcpParams_t * arg );

int launch_Server ( serverTcpParams_t* arg )
{
	initRegistration();
	PS_TCP_init(); //Init pub-sub managment
	arg->errorLine = 0; //no error 

	//Un thread est initialisé avec comme fonction de le serveurTCP et en paramètre les paramètres qu'il doit utiliser
	if ( pthread_create ( &arg->threadServerTCP, NULL, (void * (*)(void *))server_TCP, arg ) == -1 )
	{

  		//En cas d'erreur, un message est affiché, un bit d'actitivé du serveur indique qu'il n'est pas fonctionnel et la ligne fautive est renvoyée.
		perror ( "pthread_create" );
		arg->flag.work = false;
		return ( __LINE__ );
	}

  	//Si le lancement se passe normalement, le bit d'activité du serveur indique que celui-ci est en fonctionnement.
	arg->flag.work = true;


	//Return 0 => Pas d'erreur.
	return ( 0 );
}

void * server_TCP ( serverTcpParams_t * arg )
{
	int opt = true;

	uint16_t max_clients = arg->nbMaxClients;

	int master_socket, addrlen, new_socket, client_socket[ max_clients ], activity, valread, sd;
	int max_sd;
	struct sockaddr_in address;
	char* buffer;  //data buffer of 1K
	buffer = malloc(1025);
	//set of socket descriptors
	fd_set readfds;

	memset ( client_socket, 0, sizeof ( *client_socket ) * max_clients ); //initialise all client_socket[] to 0 so not checked

	//create a master socket
	master_socket = socket ( AF_INET, SOCK_STREAM, 0 );
	if ( !master_socket ) 
	{
		perror ( "socket failed" );
		arg->flag.work = false;
		arg->errorLine = __LINE__;
		return ( (void *)&arg->errorLine );
	}

	if ( setsockopt ( master_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt) ) < 0 )
	{
		perror("setsockopt");
		arg->flag.work = false;
		arg->errorLine = __LINE__;
		return ( (void *)&arg->errorLine );
	}

	//type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( arg->port );

	//bind the socket to localhost on the specified port
	if ( bind ( master_socket, (struct sockaddr *)&address, sizeof ( address ) ) < 0) 
	{
		perror("bind failed");
		arg->flag.work = false;
		arg->errorLine = __LINE__;
		return ( (void *)&arg->errorLine );
	}
	
	if (listen(master_socket, arg->nbMaxClients) < 0)
	{
		perror("listen");
		arg->flag.work = false;
		arg->errorLine = __LINE__;
		return ( (void *)&arg->errorLine );
	}
	
	while ( true ) 
	{
		//clear the socket set
		FD_ZERO ( &readfds );

		//add master socket to set
		FD_SET ( master_socket, &readfds );
		max_sd = master_socket;
		
		//add child sockets to set
		for ( uint8_t i = 0; i < max_clients; i++ ) 
		{
			//socket descriptor
			sd = client_socket[ i ];
			
			//if valid socket descriptor then add to read list
			if ( sd > 0 )
			{
				FD_SET( sd , &readfds );
			}
			
			//highest file descriptor number, need it for the select function
			if ( sd > max_sd )
			{
				max_sd = sd;
			}
		}

		//wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
		activity = select ( max_sd + 1 , &readfds , NULL , NULL , NULL );

		if ( ( activity < 0 ) &&
			( errno != EINTR ) ) 
		{
			printf("select error");
		}

		// If something happened on the master socket , then its an incoming connection
		if ( FD_ISSET ( master_socket, &readfds ) ) 
		{
			addrlen = sizeof ( address );
			new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
			if ( new_socket < 0 )
			{
				perror("accept");
				arg->flag.work = false;
				arg->errorLine = __LINE__;
				return ( (void *)&arg->errorLine );
			}

			//inform user of socket number - used in send and receive commands
			//printf ( "New connection , socket fd is %d , ip is : %s , port : %d \n",
			//	new_socket,
			//	inet_ntoa ( address.sin_addr ),
			//	ntohs ( address.sin_port ) );

			//add new socket to array of sockets
			for ( uint8_t i = 0; i < max_clients; i++) 
			{
				//if position is empty
				if( client_socket[ i ] == 0 )
				{
					client_socket[ i ] = new_socket;
					//printf ( "Adding to list of sockets as %d\n" , i );
					break;
				}
			}
		}

		// else its some IO operation on some other socket :)
		for ( uint8_t i = 0; i < max_clients; i++) 
		{
			sd = client_socket[ i ];

			if ( FD_ISSET ( sd , &readfds ) ) 
			{
				//Check if it was for closing , and also read the incoming message
				valread = read ( sd, buffer, 1024 );
				if ( valread == 0 )
				{ //Somebody disconnected 
					getpeername ( sd , (struct sockaddr*)&address, (socklen_t*)&addrlen );
					PS_TCP_suppimerSubscriber(sd);
					//Close the socket and mark as 0 in list for reuse
					close( sd );
					client_socket[ i ] = 0;
				}
				else
				{ 
					send ( sd , "01ABsuccess" , strlen ( "01ABsuccess" ), MSG_CONFIRM );
					char** result;
					result = malloc(sizeof(char*)); 
					int nbMess = findSubstring ( buffer, "01AB", &result );
					for ( int id = nbMess-1; id >= 0; id-- )
					{
						result[ id ][ valread ] = '\0';
						// contains
						char** delimiters;
						int nbElement;
						delimiters = str_split ( result[id], '|', &nbElement );

						if(nbElement == NB_ELEMENT_MESSAGE_INIT)
						{
							if(atoi(delimiters[2]) == INIT)
							{
								//Initialisation d'un nouvel objet, création d'un socket pour pouvoir lui parler.
								char* nomUnique;
								nomUnique = malloc(strlen(delimiters[1])+1+sizeof(int));
								sprintf(nomUnique,"%s_%d",delimiters[1],sd);
								PS_TCP_ajoutSubscriber(nomUnique, sd);

								char* toSend;
								toSend = gestionNouvelArrivant(nomUnique,delimiters[4],sd);
								send(sd,toSend,strlen(toSend),MSG_CONFIRM);

								free(toSend);
								free(nomUnique);
							}
						}else if(nbElement == NB_ELEMENT_MESSAGE_TO_TRANSMIT)
						{
							if(atoi(delimiters[2]) == MESSAGE)
							{
								PS_TCP_publish(delimiters[4], delimiters[5]);
							}
						}


						
						free ( result[ id ] );
						free ( buffer );
						buffer = calloc ( 1025, sizeof( char ) );
						buffer[ 0 ]= '\0';
					}
				}
			}
		}
	}
}

