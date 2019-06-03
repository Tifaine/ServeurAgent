/*
* serveurTCP.h
*
* Copyright (c) 2019 STMicroelectronics - All Rights Reserved
* Copyright (c) 2019 CREN - Action de Recherche Tifaifai
*
* Author: Guyot Tifaine <tifaine.guyot@univ-lemans.fr> <tifaine.guyot@st.com>
*/


#ifndef SERVEUR_TCP_H
#define SERVEUR_TCP_H

#include <stdint.h>

typedef struct
{
	pthread_t threadServerTCP;
	int errorLine;
	struct
	{
		uint8_t work:1;
	}flag;
	
	uint16_t port;
	uint16_t nbMaxClients;
}
serverTcpParams_t;

int launch_Server ( serverTcpParams_t * arg );

char** str_split(char* a_str, const char a_delim, int* nbItem);
int findSubstring(char* input, char* toFind, char*** result);


#endif //SERVEUR_TCP_H