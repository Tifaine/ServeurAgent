/**
* 
*
* Copyright (c) 2019 STMicroelectronics - All Rights Reserved
* Copyright (c) 2019 CREN - Action de Recherche Tifaifai
*
* \file registration.h
* \brief API permettant la gestion d'enregistrement et de validation d'objet arrivant dans le réseau
* \author Guyot Tifaine <tifaine.guyot@univ-lemans.fr> <tifaine.guyot@st.com>
* \version 0.1
* \date 03/06/2019
*
*
*/

#ifndef REGISTRATION_H
#define REGISTRATION_H

#define TYPE_PF 	1
#define TYPE_OBJET 	2

#include <assert.h>
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>


typedef struct list_objet
{
	int nbObjet;
	int* tabSocket;
}list_objet;

typedef struct list_PF
{
	int socket;
	char* nomUnique;
}list_PF;

void initRegistration();

char* gestionNouvelArrivant(char* nom_Unique, char* type, int socket);

#endif //REGISTRATION_H