#include "registration.h"

list_objet listObjet;
list_PF listPF;

#define DEPART_OBJET	8
#define INIT 			1
#define MESSAGE			2
#define MAXDATASIZE 	5000

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
		/*char* messageToReturn;
		messageToReturn = malloc(strlen(nom_Unique)+2+sizeof(int));
		sprintf(messageToReturn,"%d-%s",DEPART_OBJET,nom_Unique);
		PS_TCP_publish(listPF.nomUnique, messageToReturn);*/

		char* messageToReturn;
		mxml_node_t *xml;
		mxml_node_t *data;
		xml = mxmlNewXML("1.0");

		data = mxmlNewElement(xml, "content");
		mxmlElementSetAttr(data,"type","8");
		
		mxmlElementSetAttr(data,"unique_name",nom_Unique);    

		messageToReturn = malloc(MAXDATASIZE);
		mxmlSaveString(xml , messageToReturn, MAXDATASIZE, MXML_NO_CALLBACK);
		PS_TCP_publish(listPF.nomUnique, messageToReturn);


	}
}

int gestionNouvelArrivant(char* nom_Unique, char* type, int socket, char* messageToReturn)
{
	if(nom_Unique!=NULL)
	{
		mxml_node_t *xml;
		mxml_node_t *data;
		xml = mxmlNewXML("1.0");
		data = mxmlNewElement(xml, "message");
		mxmlElementSetAttr(data,"name","server");
		mxmlElementSetAttr(data,"type","1");
		mxmlElementSetAttr(data,"privateName",nom_Unique);
		switch(atoi(type))
		{
			case TYPE_PF:
			listPF.socket = socket;			
			listPF.nomUnique =  malloc(strlen(nom_Unique)+1);
			strcpy(listPF.nomUnique,nom_Unique);
			break;
			case TYPE_OBJET:
			listObjet.nbObjet++;
			listObjet.tabSocket = realloc(listObjet.tabSocket,listObjet.nbObjet*sizeof(int));
			listObjet.tabSocket[listObjet.nbObjet-1]= socket;
			if(listPF.socket != -1)
			{
				mxmlElementSetAttr(data,"PFName",listPF.nomUnique);				
			}		
			break;
			default:
			break;
		}
		mxmlSaveString(xml , messageToReturn, MAXDATASIZE, MXML_NO_CALLBACK);
		return 1;
	}else return 0;
	
}