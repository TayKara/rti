#include "./SocketClient.h"

using namespace std;
int hSocket;  /* Handle de la socket */
SocketClient::SocketClient()
{
                 
    struct hostent *infosHost; /*Infos sur le host : pour gethostbyname */
    struct in_addr adresseIP;  /* Adresse Internet au format reseau */
    struct sockaddr_in adresseSocket;
    int tailleSockaddr_in;
    int ret; /* valeur de retour */
    char msgClient[MAXSTRING], msgServeur[MAXSTRING];
    /* 1. Création de la socket */
    hSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (hSocket == -1)
    {
        printf("Erreur de creation de la socket %d\n", errno);
        exit(1);
    }
    else
        printf("Creation de la socket OK\n");
    /* 2. Acquisition des informations sur l'ordinateur distant */
    if ((infosHost = gethostbyname("ubuntu")) == 0)
    {
        printf("Erreur d'acquisition d'infos sur le host distant %d\n", errno);
        exit(1);
    }
    else
        printf("Acquisition infos host distant OK\n");
    memcpy(&adresseIP, infosHost->h_addr, infosHost->h_length);
    printf("Adresse IP = %s\n", inet_ntoa(adresseIP));
    /* 3. Préparation de la structure sockaddr_in */
    memset(&adresseSocket, 0, sizeof(struct sockaddr_in));
    adresseSocket.sin_family = AF_INET; /* Domaine */
    adresseSocket.sin_port = htons(PORT);
    memcpy(&adresseSocket.sin_addr, infosHost->h_addr, infosHost->h_length);
    /* 4. Tentative de connexion */
    tailleSockaddr_in = sizeof(struct sockaddr_in);
    if ((ret = connect(hSocket, (struct sockaddr *)&adresseSocket, tailleSockaddr_in)) == -1)
    {
        printf("Erreur sur connect de la socket %d\n", errno);
        switch (errno)
        {
        case EBADF:
            printf("EBADF - hsocket n'existe pas\n");
            break;
        default:
            printf("Erreur inconnue ?\n");
        }
        close(hSocket);
        exit(1);
    }
    else
        printf("Connect socket OK\n");
    
    
}

SocketClient::~SocketClient(){

    /* 9. Fermeture de la socket */
    close(hSocket); /* Fermeture de la socket */
    printf("Socket client fermee\n");

}

int SocketClient::sendmsg(char msgClient[]){
    char msgServeur[MAXSTRING];
    if (send(hSocket, msgClient, MAXSTRING, 0) == -1)
        /* pas message urgent */
        {
            printf("Erreur sur le send de la socket %d\n", errno);
            close(hSocket); /* Fermeture de la socket */
            exit(1);
        }
        else
            printf("Send socket OK\n");
    printf("Message envoye = %s\n", msgClient);
    //msgClient = NULL;
    if (strcmp(msgClient, EOC))
    {
        /* 6. Reception de l'ACK du serveur au client */
        if (recv(hSocket, msgClient, MAXSTRING, 0) == -1)
        {
            printf("Erreur sur le recv de la socket %d\n", errno);
            close(hSocket); /* Fermeture de la socket */
            exit(1);
        }
        else
            printf("Recv socket OK\n");
    }
    return 1;
}