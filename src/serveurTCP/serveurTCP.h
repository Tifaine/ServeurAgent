/**
* 
*
* Copyright (c) 2019 STMicroelectronics - All Rights Reserved
* Copyright (c) 2019 CREN - Action de Recherche Tifaifai
*
* \file serveurTCP.h
* \brief serveur TCP permettant le lien entre entités d'un système.
* \author Guyot Tifaine <tifaine.guyot@univ-lemans.fr> <tifaine.guyot@st.com>
* \version 0.1
* \date 03/06/2019
*
* Programme permettant de lancer un serveur TCP. Il va ainsi permettre aux entités d'un même réseau de communiquer entre elles.
*
*/


#ifndef SERVEUR_TCP_H
#define SERVEUR_TCP_H

#include <stdint.h>
#include "../util/util.h"

/**
 * \struct serverTcpParams_t
 * \brief Objet regroupant les paramètres d'un serveur.
 *
 * La structure permet de regrouper l'ensemble des paramètres utiles du serveur
 * tel que le port utilisé, le nombre maximal de client autorisé ou la ligne fautive si une erreur apparaît.
 */
typedef struct
{
	pthread_t threadServerTCP; /*!< Thread du serveur, celui-ci sera dans un thread séparé du programme principal afin de pouvoir traiter les messages entrant le plus efficacement possible. */
	int errorLine; /*!< Numéro de la ligne fautive si le serveur renvoit une erreur. */
	struct
	{
		uint8_t work:1; /*!< Bit témoignant du status du serveur; en fonctionnement ou non. */
	}flag;
	
	uint16_t port; /*!< Port utilisé par le serveur. */
	uint16_t nbMaxClients; /*!< Nombre maximal de clients autorisés sur le serveur. */
}
serverTcpParams_t;

/**
 * \fn int launch_Server ( serverTcpParams_t * arg )
 * \brief Fonction de lancement du thread du serveur TCP
 *
 * \param arg Adresse de la structure contenant les paramètres à utiliser pour le serveur.
 * \return 0 si aucune erreur, l'adresse de la ligne fautive sinon.
 */
int launch_Server ( serverTcpParams_t * arg );





#endif //SERVEUR_TCP_H