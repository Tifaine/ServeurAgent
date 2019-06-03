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

#include "serveurTCP.h"
#include "../log/log.h"


static void * server_TCP ( serverTcpParams_t * arg );

int launch_Server ( serverTcpParams_t* arg )
{
	arg->errorLine = 0;
  	if ( pthread_create ( &arg->threadServerTCP, NULL, (void * (*)(void *))server_TCP, arg ) == -1 )
  	{
   		perror ( "pthread_create" );
   		arg->flag.work = false;
		return ( __LINE__ );
  	}
	arg->flag.work = true;
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

	//set master socket to allow multiple connections , this is just a good habit, it will work without this
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
	//printf("Listener on port %d \n", PORT);
	//try to specify maximum of 10 pending connections for the master socket
	if (listen(master_socket, 10) < 0)
	{
		perror("listen");
   		arg->flag.work = false;
   		arg->errorLine = __LINE__;
		return ( (void *)&arg->errorLine );
	}
	//mtp_init_client_tcp((agent*)ag,"127.0.0.1");
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
			printf ( "New connection , socket fd is %d , ip is : %s , port : %d \n",
				new_socket,
				inet_ntoa ( address.sin_addr ),
				ntohs ( address.sin_port ) );
			 
			//add new socket to array of sockets
			for ( uint8_t i = 0; i < max_clients; i++) 
			{
				//if position is empty
				if( client_socket[ i ] == 0 )
				{
					client_socket[ i ] = new_socket;
					logDebug ( "Adding to list of sockets as %d\n" , i );
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

						delimiters = str_split ( result[id], '-', &nbElement );
						
						for(int indice = 0;indice < nbElement ; indice++)
						{
							printf("Message : %s \n",delimiters[indice]);
						}
						
						free ( result[ id ] );
						free ( buffer );
						buffer = calloc ( 1025, sizeof( char ) );
						buffer[ 0 ]= '\0';
					}				   
					//usleep(100000);
					//set the string terminating NULL byte on the end of the data read
								  
				}
			}
		}
	}	 
}


char** str_split(char* a_str, const char a_delim, int* nbItem)
{
  char** result    = 0;
  size_t count     = 0;
  char* tmp        = a_str;
  char* last_comma = 0;
  char delim[2];
  delim[0] = a_delim;
  delim[1] = 0;

    /* Count how many elements will be extracted. */
  while (*tmp)
  {
    if (a_delim == *tmp)
    {
      count++;
      last_comma = tmp;
    }
    tmp++;
  }

    /* Add space for trailing token. */
  count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
  count++;

  result = malloc(sizeof(char*) * count);

  if (result)
  {
    size_t idx  = 0;
    char* token = strtok(a_str, delim);

    while (token)
    {
      assert(idx < count);
      *(result + idx++) = strdup(token);
      token = strtok(0, delim);
    }
    assert(idx == count - 1);
    *(result + idx) = 0;
  }
  *nbItem = count-1;
  return result;
}

int findSubstring(char* input, char* toFind, char*** result)
{
  char** toReturn;
  toReturn = malloc(sizeof(char*));
  int nbMessage = 0;
  int isOk = 1;

  while(isOk==1 && input!=NULL)
  {
    if(strlen(input)>strlen(toFind)+1)
    {
      for(int i=strlen(input)-1-strlen(toFind);i>=0;i--)
      {
        isOk=1;
        for(int j=0;j<strlen(toFind);j++)
        {
          if(input[i+j]!=toFind[j])
          {
            isOk = 0;
            break;
          }        
        }
        if(isOk==1)
        {
          toReturn[nbMessage] = input + i + strlen(toFind);
          (*result)[nbMessage] = malloc(500);
          memcpy((*result)[nbMessage],toReturn[nbMessage],strlen(toReturn[nbMessage]));
          (*result)[nbMessage][ strlen ( toReturn[nbMessage] ) ] = '\0';
          if(i > 0)
          {
            //memcpy(input, input, strlen(input)-(strlen(input)-i-1));
            input[i] = '\0';
          }else
          {
            input = NULL;
          }
          
          break;
        }
      }
    }
    nbMessage++;
  }
  return nbMessage;
}