#include "PS_Managment.h"
#include "../log/log.h"

map_topic* listTopics;
int nbTopic;

void PS_TCP_init()
{
	listTopics = malloc(sizeof(map_topic));
	nbTopic = 0;
}

void PS_TCP_ajoutSubscriber(char* nom, int socket)
{
	int topicDejaCree = 0;
	for(int i=0;i<nbTopic;i++)
	{
		if(strcmp(listTopics[i].nom,nom)==0)
		{
			topicDejaCree = 1;
			listTopics[i].nbSubscriber++;
			listTopics[i].tabSocket = realloc(listTopics[i].tabSocket,listTopics[i].nbSubscriber*sizeof(int));
			listTopics[i].tabSocket[listTopics[i].nbSubscriber-1]= socket;
		}
	}
	if(topicDejaCree==0)
	{
		listTopics = realloc(listTopics,(nbTopic+1)*sizeof(map_topic));
		listTopics[nbTopic].nom = malloc(strlen(nom)+1);
		strcpy(listTopics[nbTopic].nom,nom);
		listTopics[nbTopic].tabSocket = malloc(sizeof(int));
		listTopics[nbTopic].tabSocket[0] = socket;
		listTopics[nbTopic].nbSubscriber=1;
		nbTopic++;
	}
}

char* PS_TCP_suppimerSubscriber(int socket)
{
	//Client vient de se déconnecter, faut tout lui enlever !
	char* toReturn;
	for(int i=0;i<nbTopic;i++)
	{
		for(int j=0;j<listTopics[i].nbSubscriber;j++)
		{
			if(listTopics[i].tabSocket[j]==socket)
			{
				toReturn = malloc(strlen(listTopics[i].nom)+1);
				strcpy(toReturn,listTopics[i].nom);
				for(int k=j;k<listTopics[i].nbSubscriber-1;k++)
				{
					listTopics[i].tabSocket[k]=listTopics[i].tabSocket[k+1];
				}
				listTopics[i].nbSubscriber--;
				listTopics[i].tabSocket = realloc(listTopics[i].tabSocket,listTopics[i].nbSubscriber*sizeof(int));
			}
		}
	}
	return toReturn;
}

void PS_TCP_publish(char* nomTopic, char* data)
{
	for(int i=0;i<nbTopic;i++)
	{
		if(strcmp(listTopics[i].nom,nomTopic)==0)
		{
			for(int j=0;j<listTopics[i].nbSubscriber;j++)
			{
				PS_TCP_envoiMessage(data,nomTopic,TOPIC,listTopics[i].tabSocket[j]);
			}			
		}
	}
}

void PS_TCP_envoiMessage(char* message, char* topic, int type, int socket)
{
	char* messageToSend;
	messageToSend = malloc(strlen("01AB")+strlen(message)+1+sizeof(int)+1);
	sprintf(messageToSend,"01AB%d-%s",type,message);
	send(socket , messageToSend , strlen(messageToSend) , MSG_CONFIRM );
	free(messageToSend);
}