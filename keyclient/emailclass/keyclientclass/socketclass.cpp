//Created by kelvin silva.
//You may use, modify, and distribute any part of this source code in any way shape or form
//when anything here is modified, tampered, criticized or distributed in any way shape or form, i also request it to be shared alike.
//This class is for client type sockets to connect to servers.
//May need some cleaning up and add UDP support.
#include "socketclass.h"

using namespace std;

socketclass::socketclass(){

    ipaddress = "127.0.0.1";
    portnumber = 80;
    buffersize = 5000;
    server_reply = new char[buffersize];
    initwinsk();
    initsocket(SOCK_STREAM);

}

socketclass::socketclass(string ipsock, int portnom, int buff, int udp_tcp){

    ipaddress = ipsock;
    portnumber = portnom;
    buffersize = buff;
    server_reply = new char[buffersize];
    initwinsk();
    initsocket(udp_tcp);

}



int socketclass::initwinsk(){


    if (WSAStartup(MAKEWORD(2,2), &wsa)!=0)  {
              //more error checking
            int x = WSAGetLastError();

            stringstream ss;

                    ss <<"Error: " << x << " From WSAGetLastError()";

            MessageBox(NULL, "Winsock Error!", ss.str().c_str(), MB_OK | MB_ICONINFORMATION);
       }


}


void socketclass::changebuffersize(int bufsz){

    buffersize = bufsz;

}



int socketclass::initsocket(int udp_tcp){

    if ((sa = socket(AF_INET, udp_tcp, IPPROTO_TCP)) == INVALID_SOCKET){
             int x = WSAGetLastError();

            stringstream ss;

                    ss <<"Error: " << x << " From WSAGetLastError()";

            MessageBox(NULL, "Winsock Error!", ss.str().c_str(), MB_OK | MB_ICONINFORMATION);
            return -1;
    }

    server.sin_addr.s_addr = inet_addr (ipaddress.c_str());      //initialize members of server structure
    server.sin_family = AF_INET;
    server.sin_port = htons(portnumber);

    return 1;
}

int socketclass::connectsocket(){

     if (connect(sa, (struct sockaddr *)&server, sizeof(server)) < 0){ //connect to gmx server. must change structure values and add tls to conect gmail.

         int x = WSAGetLastError();

         stringstream ss;

         ss <<"Error: " << x << " From WSAGetLastError()";

         MessageBox(NULL, "Winsock Error!", ss.str().c_str(), MB_OK | MB_ICONINFORMATION);
        return -1;

    }

    return 1;
}



char* socketclass::read_socket(){

        int counter = 0;

                while (counter < buffersize){ //make sure read buffer is clear!
                    server_reply[counter] = '\0';
                    counter++;

                }

            if ((recv(sa, server_reply, buffersize, 0)) == SOCKET_ERROR){     //error checking combined with recv function

                int x = WSAGetLastError();

                stringstream ss;


                    ss <<"Error: " << x << " From WSAGetLastError()";

                MessageBox(NULL, "Winsock Error!", ss.str().c_str(), MB_OK | MB_ICONINFORMATION);

                server_reply = NULL;
                return server_reply;

            }


        return server_reply;

}

int socketclass::send_socket(string mess){

        if (send(sa, mess.c_str(), mess.size(), 0 ) < 0){

            int x = WSAGetLastError();

            stringstream ss;

                ss <<"Error: " << x << " From WSAGetLastError()";

            MessageBox(NULL, "Winsock Error!", ss.str().c_str(), MB_OK | MB_ICONINFORMATION);

            return -1;

        }


        return 0;


}

void socketclass::setsocket(string ip, int port, int udp_tcp){

    portnumber = port;
    ipaddress = ip;

    initsocket(udp_tcp);

}

int socketclass::shutsocket(){

    if (shutdown(sa, 2) == SOCKET_ERROR){

        int x = WSAGetLastError();

            stringstream ss;

                ss <<"Error: " << x << " From WSAGetLastError()";

            MessageBox(NULL, "Winsock Error!", ss.str().c_str(), MB_OK | MB_ICONINFORMATION);

            return -1;

    }

    return 0;

}

int socketclass::endsocket(){


        if (closesocket(sa) == SOCKET_ERROR){

                    int x = WSAGetLastError();

            stringstream ss;

                ss <<"Error: " << x << " From WSAGetLastError()";

            MessageBox(NULL, "Winsock Error!", ss.str().c_str(), MB_OK | MB_ICONINFORMATION);

            return -1;

        }
        return 0;
}

void socketclass::nametoip(string www, int portno){

//needs improvement...

    char* hostname = new char[www.size()+1];
    copy(www.begin(), www.end(), hostname);
    hostname[www.size()] = '\0';

    struct addrinfo *res;
    struct in_addr addr;

    getaddrinfo(hostname, NULL, 0, &res);

    addr.S_un = ((struct sockaddr_in *)(res->ai_addr))->sin_addr.S_un;

    server.sin_addr.s_addr = inet_addr(inet_ntoa(addr));
    server.sin_port = htons(portno);

    freeaddrinfo(res);
    delete []hostname;


}

socketclass::~socketclass(){

    delete[] server_reply;
    endsocket();
    WSACleanup();



    //dtor
}




