#include "email.h"
#include "project_client.h"


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

                             char* mandrilluser = new char[len1+1];
                             char* mandrillapi = new char[len2+1];
                             char* emaildestination = new char[len3+1];
                             char* emailhdrfrom = new char[len4+1];
                             char* subjectheader = new char[len5+1];
                             char* emailmessage = new char[len6+1];

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
                                     }

                            delete []mandrilluser;
                            delete []mandrillapi;
                            delete []emaildestination;
                            delete []emailhdrfrom;
                            delete []subjectheader;
                            delete []emailmessage;

                        }else
                            MessageBox(NULL, "Make sure you fill in all required fields!", "Error!", NULL);




            }

            break;
            case IDC_BUTTON3:  MessageBox(NULL, "This email spammer uses mandrill.com transactional email service to spam.\nYou must go to mandrillapp.com or search on google and open up a mandrill account.\nThen you make a mandrill API key and encode it in base 64, that will be your password for this spammer\nYour email username must also be encoded in base 64.\n", "Info", MB_ICONASTERISK);
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

