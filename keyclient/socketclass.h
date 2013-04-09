#ifndef SOCKETCLASS_H_INCLUDED
#define SOCKETCLASS_H_INCLUDED

#include <windows.h>
#include <winsock2.h>
#include <string>
#include <sstream>
#include <iostream>
using std::string;

class socketclass{

    private:

        string ipaddress;
        int portnumber;
        WSADATA wsa; //winsock
        SOCKET sa; //handle to socket
        struct sockaddr_in server;
        int buffersize;


    public:

        socketclass(string, int, int);
        socketclass();
        ~socketclass();

        char read_socket();
        int send_socket(string);
        int connectsocket();

        void initsocket();
        void endsocket();
        void setsocket(string, int);
        void changebuffersize(int);



};
#endif // SOCKETCLASS_H_INCLUDED
