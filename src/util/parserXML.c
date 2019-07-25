#include "parserXML.h"
#define MAXDATASIZE 	5000

int openMessage(vector * listMessage, char * message)
{
	printf("Message : %s\n",message);
	int nbMessage = 0;
	mxml_node_t *tree = NULL;
	mxml_node_t *node = NULL;
	mxml_node_t *nodeBis = NULL;
	char* content;
	tree = mxmlLoadString(NULL, message, MXML_OPAQUE_CALLBACK);
	if(tree == NULL)
	{
		return 0;
	}
	for ( node = mxmlFindElement ( tree, tree, "message", NULL, NULL, MXML_DESCEND );
	node != NULL;
	node = mxmlFindElement ( node, tree, "message", NULL, NULL, MXML_DESCEND ) )
	{
		_message* mess;
		mess = malloc(sizeof(*(mess)));
		mess->emitterName = malloc(strlen(mxmlElementGetAttr ( node, "name" )));
		strcpy(mess->emitterName,mxmlElementGetAttr ( node, "name" ));		
		mess->type = atoi(mxmlElementGetAttr ( node, "type" ));
		if(mess->type == 1)
		{
			mess->content = malloc(strlen(mxmlElementGetAttr ( node, "content" )));
			strcpy(mess->content,mxmlElementGetAttr ( node, "content" ));
			
		}else
		{
			printf("Ici\n");
			mxml_node_t *xml;
			mxml_node_t *data;
			xml = mxmlNewXML("1.0");
			data = mxmlNewElement(xml, "toPublish");
			nodeBis = mxmlFindElement ( node, node, "toTransfer", NULL, NULL, MXML_DESCEND );
			if(nodeBis!= NULL)
			{
				mess->receiverName = malloc(strlen(mxmlElementGetAttr ( nodeBis, "receiver" )));
				strcpy(mess->receiverName,mxmlElementGetAttr ( nodeBis, "receiver" ));	
				mxmlElementSetAttr(data,"type",mxmlElementGetAttr ( nodeBis, "type" ));
				mxmlElementSetAttr(data,"nbSlot",mxmlElementGetAttr ( nodeBis, "nbSlot" ));
				mxmlElementSetAttr(data,"from",mess->emitterName);
			}
			mess->content = malloc(MAXDATASIZE);
			mxmlSaveString(xml , mess->content, MAXDATASIZE, MXML_NO_CALLBACK);

		}
		vector_add(listMessage,mess);
		nbMessage++;
	}
	return nbMessage;

}
