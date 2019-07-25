/**
* 
*
* Copyright (c) 2019 STMicroelectronics - All Rights Reserved
* Copyright (c) 2019 CREN - Action de Recherche Tifaifai
*
* \file PS_Managment.h
* \brief Ensemble de fonction permettant de souscrire et de publier à différents topics
* \author Guyot Tifaine <tifaine.guyot@univ-lemans.fr> <tifaine.guyot@st.com>
* \version 0.1
* \date 03/06/2019
*
*/

#ifndef PS_MANAGMENT_H
#define PS_MANAGMENT_H
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include "../mxml/mxml.h"
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>

#define REPONSE 	1
#define TOPIC 		2

typedef struct map_topic
{
	char* nom;
	int nbSubscriber;
	int* tabSocket;
}map_topic;

/**
 * \fn void PS_TCP_init()
 * \brief Fonction permettant d'initialiser la gestion des topics et les divers variables allant être utilisées
 *
 */
void PS_TCP_init();

/*
 * \fn void PS_TCP_ajoutSubscriber(char* nom, int socket)
 * \param nom Nom du topic a créer ou à ajouter un socket
 * \param socket Numéro de la socket à ajouter au topic
 * \brief Fonction d'ajout d'un subscriber à un topic, si le topic n'existe pas déjà, il est créé, chaque agent est identifié par sa socket
*/
void PS_TCP_ajoutSubscriber(char* nom, int socket);

/*
 * \fn void PS_TCP_suppimerSubscriber(int socket)
 * \param socket : Numéro de la socket à supprimer
 * \brief Fonction permettant de supprimer une socket à tous les topics crées
*/
char* PS_TCP_suppimerSubscriber(int socket);


/*
 * \fn void PS_TCP_publish(char* nomTopic, char* data)
 * \param nomTopic : Nom du topic où publier des données
 * \param data : Chaïne de caractères à envoyer à toutes les sockets souscrivant au topic passé en paramètre
 * \brief Fonction permettant d'envoyer un message à toutes les entitées ayant subscribe au topic passé en paramètre'
*/

void PS_TCP_publish(char* nomTopic, char* data);


/*
	Fonction d'envoie de message à un client, prend le message en paramètre ainsi
	que la socket et le type du message. On prend aussi en paramètre le topic
	qui peut être NULL si le type n'est pas le bon
	Dans la fonction : on construit le message
	type-topic-message 
	puis on envoie
*/
void PS_TCP_envoiMessage(char* message, char* topic, int type, int socket);


#endif