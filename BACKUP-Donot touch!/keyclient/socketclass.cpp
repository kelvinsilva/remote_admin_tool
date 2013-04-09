
#include "socketclass.h"
#include <windows.h>
using namespace std;

socketclass::socketclass(){

    setsocket(string("127.0.0.1"), 80);
    buffersize = 5000;


}

socketclass::socketclass(string ipsock, int portnom, int buff){

    setsocket(ipsock, portnom);
    buffersize = buff;

}


void socketclass::changebuffersize(int bufsz){

    buffersize = bufsz;

}



void socketclass::initsocket(){

    if (WSAStartup(MAKEWORD(2,2), &wsa)!=0)  {
              //more error checking
            int x = WSAGetLastError();

            stringstream ss;

                    ss <<"Error: " << x << " From WSAGetLastError() From WSAStart() !";

            MessageBox(NULL, ss.str().c_str(), " Winsock Error! ", MB_OK | MB_ICONINFORMATION);
       }


    MessageBox(NULL, ipaddress.c_str(), "nigga", NULL);
    server.sin_addr.s_addr = inet_addr(ipaddress.c_str());      //initialize members of server structure
    server.sin_family = AF_INET;
    server.sin_port = htons(portnumber);

}

int socketclass::connectsocket(){

    if ((sa = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET){
             int x = WSAGetLastError();

            stringstream ss;

                    ss <<"Error: " << x << " From WSAGetLastError() from socket() function!";

            MessageBox(NULL, ss.str().c_str(), " Winsock Error! ", MB_OK | MB_ICONINFORMATION);
    }

     if (connect(sa, (struct sockaddr *)&server, sizeof(server)) < 0){

         int x = WSAGetLastError();

         stringstream ss;

         ss <<"Error: " << x << " From WSAGetLastError() From connect() function!";

         MessageBox(NULL, ss.str().c_str(), " Winsock Error! ", MB_OK | MB_ICONINFORMATION);
        return -1;

    }

    return 1;
}

char socketclass::read_socket(){

    char server_reply[buffersize];

        int counter = 0;

                while (counter < buffersize){ //make sure read buffer is clear!
                    server_reply[counter] = '\0';
                    counter++;

                }

            if ((recv(sa, server_reply, buffersize, 0)) == SOCKET_ERROR){     //error checking combined with recv function

                int x = WSAGetLastError();

                stringstream ss;


                    ss <<"Error: " << x << " From WSAGetLastError()" << " From recv() function! ";

                MessageBox(NULL, ss.str().c_str(), "Winsock Error! ", MB_OK | MB_ICONINFORMATION);


                return 0;

            }


            //again used for debugging:
            cout << server_reply;
        return *server_reply;

}

int socketclass::send_socket(string mess){

        if (send(sa, mess.c_str(), mess.size(), 0 ) < 0){

            int x = WSAGetLastError();

            stringstream ss;

                ss <<"Error: " << x << " From WSAGetLastError()" << " send() function!";

            MessageBox(NULL, ss.str().c_str(), "Winsock Error!", MB_OK | MB_ICONINFORMATION);

            return -1;

        }
        cout << mess;

        return 0;


}

void socketclass::setsocket(string ip, int port){

    portnumber = port;
    ipaddress = ip;

    initsocket();

}

void socketclass::endsocket(){

    closesocket(sa);

}

socketclass::~socketclass(){
    WSACleanup();
    MessageBox(NULL, "dtor socket", "dtor socket", NULL);
    //dtor
}


