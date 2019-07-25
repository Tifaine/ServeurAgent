#include "PS_Managment.h"
#include "../log/log.h"
#define MAXDATASIZE 	5000

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
	mxml_node_t *xml;
	mxml_node_t *data;
	xml = mxmlNewXML("1.0");
	
	data = mxmlNewElement(xml, "publish");
    mxmlElementSetAttr(data,"topic",topic);
    if(type == TOPIC)
    {
    	
    	mxmlElementSetAttr(data,"type","2");
    }else
    {    	
    	mxmlElementSetAttr(data,"type","1");
   	}
    
   	mxml_node_t *tree = NULL;
    tree = mxmlLoadString(NULL, message, MXML_OPAQUE_CALLBACK);
    if(tree == NULL)
	{
		mxmlElementSetAttr(data,"frame",message);
	}else
	{
		mxmlAdd(data,MXML_ADD_AFTER,MXML_ADD_TO_PARENT,tree);
	}
        
   	
   	messageToSend = malloc(MAXDATASIZE);
    mxmlSaveString(xml , messageToSend, MAXDATASIZE, MXML_NO_CALLBACK);
    
    send(socket , messageToSend , strlen(messageToSend) , MSG_CONFIRM );

	free(messageToSend);
	
}