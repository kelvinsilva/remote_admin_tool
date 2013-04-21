#include "project_client.h"


extern SOCKET sa;
BOOL CALLBACK MsgBoxTrigger(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){

switch(Message){

    case WM_INITDIALOG:{

        SetDlgItemText(hwnd, ID_TITLEMSGBOXTRIG, "Enter Title Of MessageBox");
        SetDlgItemText(hwnd, ID_MESSAGEMSGBOXTRIG, "Enter Message of MessageBox");
                HWND handletocombo = GetDlgItem(hwnd, ID_MSGBOXTRIGSTYLE);

                SendMessage(handletocombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>((LPCTSTR)"Abort Retry Ignore"));
                SendMessage(handletocombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>((LPCTSTR)"Help MsgBox"));
                SendMessage(handletocombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>((LPCSTR)"Ok Cancel"));
                SendMessage(handletocombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>((LPCSTR)"Yes No"));

                handletocombo = GetDlgItem(hwnd, ID_MSGBOXICONCOMBO);

                SendMessage(handletocombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>((LPCSTR)"Exclamation Icon"));
                SendMessage(handletocombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>((LPCSTR)"Warning Icon"));
                SendMessage(handletocombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>((LPCSTR)"Error Icon"));
                SendMessage(handletocombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>((LPCSTR)"Stop Icon"));

//                    MSGBOX_ICONS["Exclamation Icon"]      = MB_ICONEXCLAMATION;
    //MSGBOX_ICONS["Warning Icon"]          = MB_ICONWARNING;
    //MSGBOX_ICONS["Error Icon"]            = MB_ICONINFORMATION;
    //MSGBOX_ICONS["Stop Icon"]             = MB_ICONSTOP;




    }

    break;
    case WM_DESTROY:

    case WM_COMMAND:
        switch(LOWORD(wParam)){


            case ID_TRIGBOXSEND: {

                //Calling GetDlgItem twice is better than calling if 4 times.
                HWND Hcomboboxstyle = GetDlgItem(hwnd, ID_MSGBOXTRIGSTYLE);
                HWND Hcomboboxicon = GetDlgItem(hwnd, ID_MSGBOXICONCOMBO);

                int lentitle = GetWindowTextLength(GetDlgItem(hwnd, ID_TITLEMSGBOXTRIG));
                int lenmessage = GetWindowTextLength(GetDlgItem(hwnd, ID_MESSAGEMSGBOXTRIG));
                int lencomboboxstyle = GetWindowTextLength(Hcomboboxstyle);
                int lencomboboxicon = GetWindowTextLength(Hcomboboxicon);

                      char* msgboxtitle = new char[lentitle+1];
                      char* msgboxmsg = new char[lenmessage+1];
                      char* MBSTYLE = new char[lencomboboxstyle+1];
                      char* MBICON = new char[lencomboboxicon+1];


                            ComboBox_GetText(Hcomboboxstyle, MBSTYLE , lencomboboxstyle+1);
                            ComboBox_GetText(Hcomboboxicon, MBICON, lencomboboxicon+1);
                            GetDlgItemText(hwnd, ID_TITLEMSGBOXTRIG, msgboxtitle, lentitle+1);
                            GetDlgItemText(hwnd, ID_MESSAGEMSGBOXTRIG, msgboxmsg, lenmessage+1);

                            stringstream ss;
                            ss << "TRIMSGBOX>" << msgboxtitle << "/" << msgboxmsg << "-" << MBSTYLE << "_" << MBICON <<"<";

                            HandleError(send(sa, ss.str().c_str(), ss.str().size(), NULL));


                               delete []msgboxtitle;
                               delete []msgboxmsg;
                               delete []MBSTYLE;
                               delete []MBICON;
                          //commented out on purpose-no use, but may need to go back again?
                        //const UINT type[8] = {0x002L, 0x006L, 0x00004000L, 0x00L, 0x001L, 0x005L, 0x004L, 0x003L};
                        //const UINT icons[8] = {0x0030L, 0x0030L, 0x0040L, 0x0040L, 0x0020L, 0x0010L, 0x0010L, 0x0010L};


            }
            break;

            case ID_MSGBOXTRIGCANCEL: {

                EndDialog(hwnd, IDCANCEL);

            }break;



        }
        break;

        case WM_CLOSE:{
                EndDialog(hwnd, IDCANCEL);
            }

    default: return false;
}

return true;
}

