#ifndef SOCKETSERVEUR_H
#define SOCKETSERVEUR_H

#define DISCONNECTED 0
#define CONNECTED 1
#define DEPARTURE_PLANNED 2
#define DEPARTURE_LOADED 3
#define FERRY_ASKED 4

#include "./Socket.h"
#include "./FileRW.h"
#include <ctime>

using namespace std;

class SocketServeur : public Socket{
    private:


    public: 

        SocketServeur();
        ~SocketServeur();

};
#endif