#ifndef SOCKET_H
#define SOCKET_H
#define NB_MAX_CLIENTS 2 /* Nombre maximum de clients connectes */
#define EOC "END_OF_CONNEXION"
#define DOC "DENY_OF_CONNEXION"
#define PORT 51101 /* Port d'ecoute de la socket serveur */ 
#define MAXSTRING 100 /* Longueur des messages */
#define affThread(num, msg) printf("th_%s> %s\n", num, msg) 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>                      /* pour select et timeval */
#include <sys/types.h> /* pour exit */ /* pour memcpy */
#include <sys/socket.h>                /* pour les types de socket */
#include <netdb.h>                     /* pour la structure hostent */
#include <netinet/in.h>                /* pour la conversion adresse reseau */
#include <netinet/tcp.h>               /* pour la conversion adresse reseau*/
#include <arpa/inet.h>                 /* pour la conversion adresse reseau*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Socket{
    private:


    public: 

        Socket();
        ~Socket();

};

#endif