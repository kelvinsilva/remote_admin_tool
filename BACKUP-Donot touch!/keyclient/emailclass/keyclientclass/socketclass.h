//Created by kelvin silva.
//You may use, modify, and distribute any part of this source code in any way shape or form
//when anything here is modified, tampered, criticized or distributed in any way shape or form, i also request it to be shared alike.
//This class is for client type sockets to connect to servers.
//May need some cleaning up and add UDP support.

#ifndef SOCKETCLASS_H_INCLUDED
#define SOCKETCLASS_H_INCLUDED
#define _WIN32_WINNT 0x501


#include <windows.h>
#include <winsock2.h>
#include <string>
#include <sstream>
#include <iostream>
#include <ws2tcpip.h>


using std::string;

class socketclass{

    private:

        struct sockaddr_in server;
        WSADATA wsa; //winsock
        SOCKET sa; //handle to socket

        string ipaddress;

        int buffersize;
        int portnumber;
        char* server_reply;




    public:

        socketclass(string, int, int, int);
        socketclass();
        ~socketclass();

        char* read_socket();


        void nametoip(string www, int portno);

        int send_socket(string);
        int connectsocket();
        int initsocket(int udp_tcp);
        int initwinsk();


        int endsocket();
        int shutsocket();

        void setsocket(string, int, int);
        void changebuffersize(int);



};
#endif // SOCKETCLASS_H_INCLUDED
