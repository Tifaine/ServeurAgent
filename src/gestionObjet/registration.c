#include "registration.h"

list_objet listObjet;
list_PF listPF;

#define DEPART_OBJET	8

void initRegistration()
{
	listObjet.nbObjet = 0;
	listObjet.tabSocket = (int *)malloc(sizeof(int));

	listPF.socket = -1;
	listPF.nomUnique = malloc(1);
}

void gestionDepartObjet(char* nom_Unique)
{
	if(nom_Unique!=NULL)
	{
		char* messageToReturn;
		messageToReturn = malloc(strlen(nom_Unique)+2+sizeof(int));
		sprintf(messageToReturn,"%d/%s",DEPART_OBJET,nom_Unique);
		PS_TCP_publish(listPF.nomUnique, messageToReturn);
	}
}

char* gestionNouvelArrivant(char* nom_Unique, char* type, int socket)
{
	if(nom_Unique!=NULL)
	{
		char* messageToReturn;
		switch(atoi(type))
		{
			case TYPE_PF:
			listPF.socket = socket;			
			listPF.nomUnique =  malloc(strlen(nom_Unique)+1);
			messageToReturn = malloc(strlen(nom_Unique)+5);
			sprintf(messageToReturn,"01AB%s",nom_Unique);
			strcpy(listPF.nomUnique,nom_Unique);
			break;
			case TYPE_OBJET:
			listObjet.nbObjet++;
			listObjet.tabSocket = realloc(listObjet.tabSocket,listObjet.nbObjet*sizeof(int));
			listObjet.tabSocket[listObjet.nbObjet-1]= socket;
			if(listPF.socket != -1)
			{
				messageToReturn = malloc(strlen(nom_Unique)+6+strlen(listPF.nomUnique));
				sprintf(messageToReturn,"01AB%s/%s",nom_Unique,listPF.nomUnique);
			}			
			break;
			default:
			break;
		}
		return messageToReturn;
	}else return NULL;
	
}