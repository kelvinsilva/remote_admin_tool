#ifndef EMAIL_H
#define EMAIL_H

#include "socketclass.h"
#include <string>
#include <windows.h>
#include <sstream>

using std::string;

class email : public socketclass
{
    public:

        email();
        email(string, string); //set email user and password
        email(string, string, string, string, string, string); // email user, password, from, to, subject, message
        email(string, int, string, string, string, string, string, string); //set ip and port, then email info
        ~email();

        int  sendmail();
        void setemailuser(string);
        void setemailpasswd(string);
        void setemailfrom(string);
        void setemailto(string);
        void setemailsubj(string);
        void setemailmessage(string);


    private:

            string ehlo;
            string authloginuser;
            string passwd;
            string mailfrom;
            string rcptto;
            string datacmd;
            string data;
            string quit;

        string emailuser;
        string emailpasswd;
        string emailfrom;
        string emailto;
        string emailsubj;
        string emailmessage;

};

#endif // EMAIL_H
