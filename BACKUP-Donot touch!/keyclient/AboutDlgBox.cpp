#include "project_client.h"
//About dialog. Made by me :3 Modify it as you want.

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
