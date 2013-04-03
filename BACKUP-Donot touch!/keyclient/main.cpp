#include <windows.h>
#include <string>
#include <winsock2.h>
#include <sstream>
#include "resource.h"
#include  "email.h"

#define WIN32_LEAN_AND_MEAN




/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM , LPARAM);
BOOL CALLBACK EmailMeDlg(HWND , UINT, WPARAM , LPARAM);
BOOL CALLBACK ESpamDlg(HWND, UINT, WPARAM, LPARAM);

//convert a string to float
float GetDlgItemFloat(HWND, int);

using namespace std;


/*  Make the class name into a global variable  */
char szClassName[ ] = "KeyClient";
HWND g_hToolbar = NULL;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{

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
    wincl.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);                 /* No menu */
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
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           600,                 /* The programs width */
           400,                 /* and height in pixels */
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
        }
        break;

        case WM_COMMAND:

            switch(LOWORD(wParam)){

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

BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){

        switch(Message){

            case WM_INITDIALOG:
            return TRUE;

            case WM_COMMAND:
                switch(LOWORD(wParam)){

                    case IDOK:
                        EndDialog(hwnd, IDOK);
                        break;

                    case IDCANCEL:
                        EndDialog(hwnd, IDCANCEL);
                        break;

                }
            break;
            default:
            return FALSE;
    }
    return TRUE;

        }

BOOL CALLBACK EmailMeDlg(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){


    switch(Message){
        case WM_INITDIALOG:

        SetDlgItemText(hwnd, IDC_DEST, "kelvinsilva747@gmail.com");
        SetDlgItemText(hwnd, IDC_FROM, "Enter your email address");
        SetDlgItemText(hwnd, IDC_MESSAGE, "Enter your message");
        SetDlgItemText(hwnd, IDC_SUBJECT, "Enter your subject");
        break;

        case WM_COMMAND:

            switch(LOWORD(wParam)){

                case IDC_PRESS:{


                    int len = GetWindowTextLength(GetDlgItem(hwnd, IDC_FROM));
                    int len2 = GetWindowTextLength(GetDlgItem(hwnd, IDC_MESSAGE));
                    int len3 = GetWindowTextLength(GetDlgItem(hwnd, IDC_SUBJECT));


                    if (len > 0 && len2 > 0){




                        char* bufeaddr = (char*)GlobalAlloc(GPTR, len+1); //allocate memory with globalalloc, with length len, global alloc returns handle to newly allocated memory, the handle is typecasted to char
                        char* bufemess = (char*)GlobalAlloc(GPTR, len2+1);
                        char* bufesubj = (char*)GlobalAlloc(GPTR, len3+1);


                        GetDlgItemText(hwnd, IDC_FROM, bufeaddr, len + 1); // set buf to dlgitmetext
                        GetDlgItemText(hwnd, IDC_MESSAGE, bufemess, len2 + 1);
                        GetDlgItemText(hwnd,  IDC_SUBJECT, bufesubj, len3 + 1);


                            string bufeaddrstr(bufeaddr);
                            string bufemessstr(bufemess);
                            string bufesubjstr(bufesubj);




                            email to_author("54.245.65.132", 587, "dHN0ZXIxQGxpdmUuY29t", "YkhmdVFXOEV4RU5KMkJRREVwOVluZw==",
                                          bufeaddrstr, "kelvinsilva747@gmail.com", bufesubjstr , bufemessstr);

                            to_author.sendmail();

                        GlobalFree((HANDLE)bufeaddr); //free memory
                        GlobalFree((HANDLE)bufemess);
                        GlobalFree((HANDLE)bufesubj);

                    }



                }
                break;
                case IDC_CANCELMAIL:
                    EndDialog(hwnd, IDC_CANCELMAIL);
                break;

            }
            break;

            default:
                return false;

    }
    return true;


}

BOOL CALLBACK ESpamDlg(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){

    switch(Message){

        case WM_INITDIALOG:{

            SetDlgItemText(hwnd, IDC_EDIT1, "YOUR MANDRILL USER ACCOUNT NAME IN BASE 64");
            SetDlgItemText(hwnd, IDC_EDIT2, "YOUR API KEY IN BASE 64");
            SetDlgItemText(hwnd, IDC_EDIT3, "To Whom?");
            SetDlgItemText(hwnd, IDC_EDIT7, "10");
            SetDlgItemText(hwnd, IDC_EDIT8, "100");
            SetDlgItemText(hwnd, IDC_EDIT4, "afakeemail@fakeemail.com");
            SetDlgItemText(hwnd, IDC_EDIT5, "Subject");
            SetDlgItemText(hwnd, IDC_EDIT6, "Message");



                return true;

        } break;

        case WM_COMMAND:
            switch(LOWORD(wParam)){

                case IDC_BUTTON2:{
                    int len1 = GetWindowTextLength(GetDlgItem(hwnd,IDC_EDIT1));
                    int len2 = GetWindowTextLength(GetDlgItem(hwnd,IDC_EDIT2));
                    int len3 = GetWindowTextLength(GetDlgItem(hwnd,IDC_EDIT3));
                    int len4 = GetWindowTextLength(GetDlgItem(hwnd,IDC_EDIT4));
                    int len5 = GetWindowTextLength(GetDlgItem(hwnd,IDC_EDIT5));
                    int len6 = GetWindowTextLength(GetDlgItem(hwnd,IDC_EDIT6));


                        if (len3 > 0 && len4 > 0 && len5 > 0){

                             char* mandrilluser = (char*)GlobalAlloc(GPTR, len1+1);
                             char* mandrillapi = (char*)GlobalAlloc(GPTR, len2+1);
                             char* emaildestination = (char*)GlobalAlloc(GPTR, len3+1);
                             char* emailhdrfrom = (char*)GlobalAlloc(GPTR, len4+1);
                             char* subjectheader = (char*)GlobalAlloc(GPTR, len5+1);
                             char* emailmessage = (char*)GlobalAlloc(GPTR, len6+1);

                                    GetDlgItemText(hwnd, IDC_EDIT1, mandrilluser, len1+1);
                                    GetDlgItemText(hwnd, IDC_EDIT2, mandrillapi, len2+1);
                                    GetDlgItemText(hwnd, IDC_EDIT3, emaildestination, len3+1);
                                    GetDlgItemText(hwnd, IDC_EDIT4, emailhdrfrom, len4+1);
                                    GetDlgItemText(hwnd, IDC_EDIT5, subjectheader, len5+1);
                                    GetDlgItemText(hwnd, IDC_EDIT6, emailmessage, len6+1);

                                    BOOL success;


                                int numberemails = GetDlgItemInt(hwnd, IDC_EDIT7, &success, TRUE);
                                double intervalnumber = GetDlgItemFloat(hwnd, IDC_EDIT8);

                                        if (!success || intervalnumber == 0 ){

                                            MessageBox(NULL, "Please type an appropriate value", "Error!", NULL);

                                        } else if (numberemails < 0 || intervalnumber < 0){

                                            MessageBox(NULL, "Please type a positive number", "Error!", NULL);

                                        } else {


                                        string mandrilluser64(mandrilluser);
                                        string mandrillapi64(mandrillapi);
                                        string emaildestinationstr(emaildestination);
                                        string emailhdrfromstr(emailhdrfrom);
                                        string subjectheaderstr(subjectheader);
                                        string emailmessagestr(emailmessage);

                                            email spam_mail("54.245.65.132", 587, mandrilluser, mandrillapi64,
                                                emailhdrfromstr, emaildestinationstr, subjectheaderstr , emailmessagestr);

                                        int interval = (60000)*intervalnumber;

                                        stringstream ss;

                                        ss << "Email Spammer- Emails sent: 0" ;
                                        SetWindowText(hwnd, ss.str().c_str() );

                                        for (int counter = 0; counter <= numberemails; counter++ ){

                                            Sleep(interval);
                                            spam_mail.sendmail();
                                            ss << "Email Spammer- Emails sent: " << counter;
                                            SetWindowText(hwnd, ss.str().c_str() );
                                            ss.str(std::string());

                                        }

                                            //spam_mail.sendmail();


                                                }

                            GlobalFree((HANDLE)mandrilluser);
                            GlobalFree((HANDLE)mandrillapi);
                            GlobalFree((HANDLE)emaildestination);
                            GlobalFree((HANDLE)emailhdrfrom);
                            GlobalFree((HANDLE)subjectheader);
                            GlobalFree((HANDLE)emailmessage);

                        }else
                            MessageBox(NULL, "Make sure you fill in all required fields!", "Error!", NULL);




            }

            break;

            case IDC_EMAILEXIT: {
                EndDialog(hwnd, 0);


                }
            }

            break;

        default:
        return false;



}
return true;

}








//http://us.generation-nt.com/answer/getdlgitemfloat-help-26637032.html

float GetDlgItemFloat(HWND hDlg, int id){

    TCHAR *pEnd;
    TCHAR szItemText[20];
    GetDlgItemText(hDlg, id, szItemText, 20);

    return (float)strtod(szItemText, &pEnd);
}



