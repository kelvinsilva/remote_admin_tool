#include <iostream>
#include "email.h"
using namespace std;

int main()
{

    socketclass testsocket("127.0.0.1", 80, 5000, SOCK_STREAM);

    testsocket.nametoip("www.google.com", 80);
    testsocket.connectsocket();
    testsocket.send_socket("GET / HTTP/1.1\r\n\r\n");


    cout << testsocket.read_socket();

    return 0;
}
