#include "registration.h"

list_objet listObjet;
list_PF listPF;

void initRegistration()
{
	listObjet.nbObjet = 0;
	listObjet.tabSocket = (int *)malloc(sizeof(int));

	listPF.socket = -1;
	listPF.nomUnique = malloc(1);
}

char* gestionNouvelArrivant(char* nom_Unique, char* type, int socket)
{
	char* messageToReturn;

	switch(atoi(type))
	{
		case TYPE_PF:
			listPF.socket = socket;			
			messageToReturn = malloc(strlen(nom_Unique));
			listPF.nomUnique =  malloc(strlen(nom_Unique));
			messageToReturn = malloc(strlen(nom_Unique)+4);
			sprintf(messageToReturn,"01AB%s",nom_Unique);
			strcpy(listPF.nomUnique,nom_Unique);
		break;
		case TYPE_OBJET:
			listObjet.nbObjet++;
			listObjet.tabSocket = realloc(listObjet.tabSocket,listObjet.nbObjet*sizeof(int));
			listObjet.tabSocket[listObjet.nbObjet-1]= socket;
			if(listPF.socket != -1)
			{
				messageToReturn = malloc(strlen(nom_Unique)+5+strlen(listPF.nomUnique));
				sprintf(messageToReturn,"01AB%s/%s",nom_Unique,listPF.nomUnique);
			}			
		break;
		default:
		break;
	}
	return messageToReturn;
}