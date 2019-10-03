#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include "./Socket.h"
using namespace std;

class SocketClient : public Socket{
    private:


    public: 

        SocketClient();
        ~SocketClient();
        int sendmsg(char msgClient[]);
};
#endif