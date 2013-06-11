#include "project_client.h"

//Some helper functions not worth including in main file.

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
