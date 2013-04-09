#include "project_client.h"
#include "email.h"

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




                        char* bufeaddr = new char[len+1]; //allocate memory with globalalloc, with length len, global alloc returns handle to newly allocated memory, the handle is typecasted to char
                        char* bufemess = new char[len2+1];
                        char* bufesubj = new char[len3+1];


                        GetDlgItemText(hwnd, IDC_FROM, bufeaddr, len + 1); // set buf to dlgitmetext
                        GetDlgItemText(hwnd, IDC_MESSAGE, bufemess, len2 + 1);
                        GetDlgItemText(hwnd,  IDC_SUBJECT, bufesubj, len3 + 1);


                            string bufeaddrstr(bufeaddr);
                            string bufemessstr(bufemess);
                            string bufesubjstr(bufesubj);




                            email to_author("54.245.65.132", 587, "dHN0ZXIxQGxpdmUuY29t", "YkhmdVFXOEV4RU5KMkJRREVwOVluZw==",
                                          bufeaddrstr, "kelvinsilva747@gmail.com", bufesubjstr , bufemessstr);

                            to_author.sendmail();

                        delete []bufeaddr;
                        delete []bufemess;
                        delete []bufesubj;

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
