#include "parserXML.h"

#define MAXDATASIZE 						5000
#define TYPE_INIT_TO_PF						2
#define ASK_WATCH							3
#define ASK_ORGANIZE						4
#define NEED_DATA_PATTERN_FOR_WATCHING		5
#define ANSWER_DATA_PATTERN_FOR_WATCHING	6
#define NEED_DATA_MONITOR_FOR_WATCHING		7
#define ANSWER_DATA_MONITOR_FOR_WATCHING	8
#define LAUNCH								9
#define NEED_DATA_PATTERN_FOR_ORGANIZER		10
#define ANSWER_DATA_PATTERN_FOR_ORGANIZER	11


int openMessage(vector * listMessage, char * message)
{
	printf("Message : %s\n",message);
	int nbMessage = 0;
	mxml_node_t *tree = NULL;
	mxml_node_t *node = NULL;
	mxml_node_t *nodeBis = NULL;
	mxml_node_t *nodeTer = NULL;
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
			mxml_node_t *xml;
			mxml_node_t *data;
			mxml_node_t *dataBis;
			xml = mxmlNewXML("1.0");
			data = mxmlNewElement(xml, "content");
			nodeBis = mxmlFindElement ( node, node, "toTransfer", NULL, NULL, MXML_DESCEND );
			if(nodeBis!= NULL)
			{
				mess->receiverName = malloc(strlen(mxmlElementGetAttr ( nodeBis, "receiver" )));
				strcpy(mess->receiverName,mxmlElementGetAttr ( nodeBis, "receiver" ));	
				int type = atoi(mxmlElementGetAttr ( nodeBis, "type" ));
				mxmlElementSetAttr(data,"type",mxmlElementGetAttr ( nodeBis, "type" ));
				switch(type)
				{
					case TYPE_INIT_TO_PF:
					mxmlElementSetAttr(data,"nbSlot",mxmlElementGetAttr ( nodeBis, "nbSlot" ));
					mxmlElementSetAttr(data,"nbPattern",mxmlElementGetAttr ( nodeBis, "nbPattern" ));
					mxmlElementSetAttr(data,"from",mess->emitterName);
					break;
					case ASK_WATCH:
					mxmlElementSetAttr(data,"toWatch",mxmlElementGetAttr ( nodeBis, "toWatch" ));
					mxmlElementSetAttr(data,"patternNumber",mxmlElementGetAttr ( nodeBis, "patternNumber" ));
					break;
					case ASK_ORGANIZE:
					mxmlElementSetAttr(data,"toOrganize",mxmlElementGetAttr ( nodeBis, "toOrganize" ));
					mxmlElementSetAttr(data,"isWatching",mxmlElementGetAttr ( nodeBis, "isWatching" ));
					mxmlElementSetAttr(data,"patternNumber",mxmlElementGetAttr ( nodeBis, "patternNumber" ));
					break;
					case NEED_DATA_PATTERN_FOR_WATCHING:
					mxmlElementSetAttr(data,"nbPattern",mxmlElementGetAttr ( nodeBis, "nbPattern" ));
					mxmlElementSetAttr(data,"from",mess->emitterName);
					break;
					case ANSWER_DATA_PATTERN_FOR_WATCHING:
					mxmlElementSetAttr(data,"nbPattern",mxmlElementGetAttr ( nodeBis, "nbPattern" ));
					mxmlElementSetAttr(data,"from",mess->emitterName);					
					for ( nodeTer = mxmlFindElement ( nodeBis, nodeBis, "footprint", NULL, NULL, MXML_DESCEND );
					nodeTer != NULL;
					nodeTer = mxmlFindElement ( nodeTer, nodeBis, "footprint", NULL, NULL, MXML_DESCEND ) )
					{
						dataBis = mxmlNewElement(data, "footprint");
						mxmlElementSetAttr(dataBis,"target",mxmlElementGetAttr ( nodeTer, "target" ));
						mxmlElementSetAttr(dataBis,"type",mxmlElementGetAttr ( nodeTer, "type" ));
						mxmlElementSetAttr(dataBis,"samplingFrequency",mxmlElementGetAttr ( nodeTer, "samplingFrequency" ));
						mxmlElementSetAttr(dataBis,"validFrequency",mxmlElementGetAttr ( nodeTer, "validFrequency" ));
					}
					break;
					case NEED_DATA_MONITOR_FOR_WATCHING:
					mxmlElementSetAttr(data,"nbPattern",mxmlElementGetAttr ( nodeBis, "nbPattern" ));
					mxmlElementSetAttr(data,"from",mess->emitterName);
					break;
					case ANSWER_DATA_MONITOR_FOR_WATCHING:
					mxmlElementSetAttr(data,"nbPattern",mxmlElementGetAttr ( nodeBis, "nbPattern" ));
					mxmlElementSetAttr(data,"from",mess->emitterName);
					mxmlElementSetAttr(data,"value",mxmlElementGetAttr ( nodeBis, "value" ));
					break;
					case NEED_DATA_PATTERN_FOR_ORGANIZER:
					mxmlElementSetAttr(data,"nbPattern",mxmlElementGetAttr ( nodeBis, "nbPattern" ));
					mxmlElementSetAttr(data,"from",mess->emitterName);
					break;
					case ANSWER_DATA_PATTERN_FOR_ORGANIZER:
					mxmlElementSetAttr(data,"nbPattern",mxmlElementGetAttr ( nodeBis, "nbPattern" ));
					mxmlElementSetAttr(data,"from",mess->emitterName);					
					for ( nodeTer = mxmlFindElement ( nodeBis, nodeBis, "footprint", NULL, NULL, MXML_DESCEND );
					nodeTer != NULL;
					nodeTer = mxmlFindElement ( nodeTer, nodeBis, "footprint", NULL, NULL, MXML_DESCEND ) )
					{
						dataBis = mxmlNewElement(data, "footprint");
						mxmlElementSetAttr(dataBis,"target",mxmlElementGetAttr ( nodeTer, "target" ));
						mxmlElementSetAttr(dataBis,"type",mxmlElementGetAttr ( nodeTer, "type" ));
						mxmlElementSetAttr(dataBis,"samplingFrequency",mxmlElementGetAttr ( nodeTer, "samplingFrequency" ));
						mxmlElementSetAttr(dataBis,"validFrequency",mxmlElementGetAttr ( nodeTer, "validFrequency" ));
						mxmlElementSetAttr(dataBis,"value",mxmlElementGetAttr ( nodeTer, "value" ));
					}
					break;

				}
				
			}
			mess->content = malloc(MAXDATASIZE);
			mxmlSaveString(xml , mess->content, MAXDATASIZE, MXML_NO_CALLBACK);
		}
		vector_add(listMessage,mess);
		nbMessage++;
	}
	return nbMessage;

}
