//Created by kelvin silva.
//You may use, modify, and distribute any part of this source code in any way shape or form
//when anything here is modified, tampered, criticized or distributed in any way shape or form, i also request it to be shared alike.
//This class is used for email telnet smtp services. Specially tailored for MANDRILL transactional email.
//May need some cleaning up and add UDP support.

#include "email.h"

using namespace std;

email::email():socketclass(){



        emailuser = "";
        emailpasswd = "";
        emailfrom = "";
        emailto = "";
        emailsubj = "";
        emailmessage = "";

           ehlo = "EHLO MAIL\r\n";
           authloginuser = "AUTH LOGIN " + emailuser + "\r\n";
           passwd = emailpasswd + "\r\n";
           mailfrom = "MAIL FROM: <" + emailfrom + ">\r\n";
           rcptto = "RCPT TO: <" + emailto + ">\r\n";
           datacmd = "DATA\r\n";
           data = "From: " + emailfrom + "\r\nTo: " + emailto + "\r\nSubject: " + emailsubj + "\r\n\r\n" + emailmessage + "\r\n.\r\n";
           quit = "QUIT\r\n";



}

email::email(string usr, string pass, string from, string to, string subj, string message):socketclass(){


        emailuser = usr;
        emailpasswd = pass;
        emailfrom = from;
        emailto = to;
        emailsubj = subj;
        emailmessage = message;

           ehlo = "EHLO MAIL\r\n";
           authloginuser = "AUTH LOGIN " + emailuser + "\r\n";
           passwd = emailpasswd + "\r\n";
           mailfrom = "MAIL FROM: <" + emailfrom + ">\r\n";
           rcptto = "RCPT TO: <" + to + ">\r\n";
           datacmd = "DATA\r\n";
           data = "From: " + emailfrom + "\r\nTo: " + emailto + "\r\nSubject: " + emailsubj + "\r\n\r\n" + emailmessage + "\r\n.\r\n";
           quit = "QUIT";

}

email::email(string ip, int port, string usr, string pass, string from, string to, string subj, string message):socketclass(ip, port, 5000, SOCK_STREAM){

        changebuffersize(5000);

        emailuser = usr;
        emailpasswd = pass;
        emailfrom = from;
        emailto = to;
        emailsubj = subj;
        emailmessage = message;

           ehlo = "EHLO MAIL\r\n";
           authloginuser = "AUTH LOGIN " + emailuser + "\r\n";
           passwd = emailpasswd + "\r\n";
           mailfrom = "MAIL FROM: <" + emailfrom + ">\r\n";
           rcptto = "RCPT TO: <" + to + ">\r\n";
           datacmd = "DATA\r\n";
           data = "From: " + emailfrom + "\r\nTo: " + emailto + "\r\nSubject: " + emailsubj + "\r\n\r\n" + emailmessage + "\r\n.\r\n";
           quit = "QUIT";


}

void email::setemailuser(string emailusr){

    emailuser = emailusr;
    authloginuser = "AUTH LOGIN";

}

void email::setemailpasswd(string emailpsd){

    emailpasswd = emailpsd;
    passwd = emailpsd + "\r\n";

}

void email::setemailfrom(string frm){

    emailfrom = frm;
    mailfrom = "MAIL FROM: <" + frm + ">\r\n";
    data = "From: " + mailfrom + "\r\nTo: " + emailto + "\r\nSubject: " + emailsubj + "\r\n\r\n" + emailmessage + "\r\n.\r\n";

}

void email::setemailto(string t){

    emailto = t;
    rcptto = "RCPT TO: <" + emailto + ">\r\n";
    data = "From: " + mailfrom + "\r\nTo: " + emailto + "\r\nSubject: " + emailsubj + "\r\n\r\n" + emailmessage + "\r\n.\r\n";

}

void email::setemailsubj(string sub){

    emailsubj = sub;
    data = "From: " + mailfrom + "\r\nTo: " + emailto + "\r\nSubject: " + emailsubj + "\r\n\r\n" + emailmessage + "\r\n.\r\n";

}



void email::setemailmessage(string msg){

    emailmessage = msg;
    data = "From: " + mailfrom + "\r\nTo: " + emailto + "\r\nSubject: " + emailsubj + "\r\n\r\n" + emailmessage + "\r\n.\r\n";

}


int email::sendmail(){

    if (initsocket(SOCK_STREAM) < 0){ //connect to gmx server. must change structure values and add tls to conect gmail.
        return -1;
    }

    if (connectsocket() < 0){ //connect to gmx server. must change structure values and add tls to conect gmail.
        return -1;
    }


        read_socket(); /* SMTP Server logon string */
        send_socket(ehlo); /* introduce ourselves */

        read_socket(); /*Read reply */
        send_socket(authloginuser);

        read_socket(); /* Sender OK */
        send_socket(passwd);
        read_socket();

        send_socket(mailfrom);

        read_socket(); // Sender OK */
        send_socket(rcptto);
        read_socket(); // Recipient OK*/
        send_socket(datacmd);
        read_socket();
        send_socket(data);// body to follow*/
        read_socket();
        send_socket(quit);

        //=====Close socket and finish=====*/
        //wsa is not terminated here. only the socket. we reinitialize the socket at the begining of this function
        endsocket();

        return 1;

}

email::~email(){


}

