#include "SocketServeur.h"
using namespace std;

pthread_mutex_t mutexIndiceCourant;
pthread_cond_t condIndiceCourant;
int indiceCourant = -1;
pthread_t threadHandle[NB_MAX_CLIENTS]; /* Threads pour clients*/
int socketServeur;
int hSocketConnectee[NB_MAX_CLIENTS]; /* Sockets pour clients*/
void *fctThread(void *param);
char *getThreadIdentity();
int actionServeur(char *, char *, int &, int &, char *, char *);
int hSocketEcoute, /* Socket d'ecoute pour l'attente */
    hSocketService;
SocketServeur::SocketServeur()
{
    FileRW::init();
    FileRW::init_f();
    int i, j,                  /* variables d'iteration */
        retRecv;               /* Code de retour dun recv */
    struct hostent *infosHost; /*Infos sur le host : pour gethostbyname */
    struct in_addr adresseIP;  /* Adresse Internet au format reseau */
    struct sockaddr_in adresseSocket;
    int tailleSockaddr_in;
    int ret, *retThread;
    char msgServeur[MAXSTRING];

    /* 1. Initialisations */
    puts("* Thread principal serveur demarre *");
    printf("identite = %d.%u\n", getpid(), pthread_self());

    pthread_mutex_init(&mutexIndiceCourant, NULL);
    pthread_cond_init(&condIndiceCourant, NULL);
    /* Si la socket n'est pas utilisee, le descripteur est a -1 */
    for (i = 0; i < NB_MAX_CLIENTS; i++)
        hSocketConnectee[i] = -1;

    /* 2. Creation de la socket d'ecoute */
    hSocketEcoute = socket(AF_INET, SOCK_STREAM, 0);
    if (hSocketEcoute == -1)
    {
        printf("Erreur de creation de la socket %d\n", errno);
        exit(1);
    }
    else
        printf("Creation de la socket OK\n");

    /* 3. Acquisition des informations sur l'ordinateur local */
    if ((infosHost = gethostbyname("ubuntu")) == 0)
    {
        printf("Erreur d'acquisition d'infos sur le host %d\n", errno);
        exit(1);
    }
    else
        printf("Acquisition infos host OK\n");

    memcpy(&adresseIP, infosHost->h_addr, infosHost->h_length);
    printf("Adresse IP = %s\n", inet_ntoa(adresseIP));

    /* 4. Préparation de la structure sockaddr_in */
    memset(&adresseSocket, 0, sizeof(struct sockaddr_in));
    adresseSocket.sin_family = AF_INET;
    adresseSocket.sin_port = htons(PORT);
    memcpy(&adresseSocket.sin_addr, infosHost->h_addr, infosHost->h_length);
    /* 5. Le système prend connaissance de l'adresse et du port de la socket */
    if (bind(hSocketEcoute, (struct sockaddr *)&adresseSocket,
             sizeof(struct sockaddr_in)) == -1)
    {
        printf("Erreur sur le bind de la socket %d\n", errno);
        exit(1);
    }
    else
        printf("Bind adresse et port socket OK\n");
    /* 6. Lancement des threads */
    for (i = 0; i < NB_MAX_CLIENTS; i++)
    {
        ret = pthread_create(&threadHandle[i], NULL, fctThread, (void *)i);
        printf("Thread secondaire %d lance !\n", i);
        ret = pthread_detach(threadHandle[i]);
    }

    do
    {
        /* 7. Mise a l'ecoute d'une requete de connexion */
        puts("Thread principal : en attente d'une connexion");
        if (listen(hSocketEcoute, SOMAXCONN) == -1)
        {
            printf("Erreur sur lel isten de la socket %d\n", errno);
            close(hSocketEcoute); /* Fermeture de la socket */
            exit(1);
        }
        else
            printf("Listen socket OK\n");
        /* 8. Acceptation d'une connexion */
        tailleSockaddr_in = sizeof(struct sockaddr_in);
        if ((hSocketService =
                 accept(hSocketEcoute, (struct sockaddr *)&adresseSocket, &tailleSockaddr_in)) == -1)
        {
            printf("Erreur sur l'accept de la socket %d\n", errno);
            close(hSocketEcoute); /* Fermeture de la socket */
            exit(1);
        }
        else
            printf("Accept socket OK\n");

        /* 9. Recherche d'une socket connectee libre */
        printf("Recherche d'une socket connecteee libre ...\n");
        for (j = 0; j < NB_MAX_CLIENTS && hSocketConnectee[j] != -1; j++)
            ;
        if (j == NB_MAX_CLIENTS)
        {
            printf("Plus de connexion disponible\n");
            sprintf(msgServeur, DOC);
            if (send(hSocketService, msgServeur, MAXSTRING, 0) == -1)
            {
                printf("Erreur sur le send de refus%d\n", errno);
                close(hSocketService); /* Fermeture de la socket */
                exit(1);
            }
            else
                printf("Send socket refusee OK");
            close(hSocketService); /* Fermeture de la socket */
        }
        else
        {
            /* Il y a une connexion de libre */
            printf("Connexion sur la socket num. %d\n", j);
            pthread_mutex_lock(&mutexIndiceCourant);
            hSocketConnectee[j] = hSocketService;
            indiceCourant = j;
            pthread_mutex_unlock(&mutexIndiceCourant);
            pthread_cond_signal(&condIndiceCourant);
        }
    } while (1);

    /* 10. Fermeture de la socket d'ecoute */
    close(hSocketEcoute); /* Fermeture de la socket */
    printf("Socket serveur fermee\n");
    puts("Fin du thread principal");
}

void *fctThread(void *param)
{
    char *nomCli, *buf = (char *)malloc(100);
    char msgClient[MAXSTRING], msgServeur[MAXSTRING];
    int vr = (int)(param), finDialogue = 0, i, iCliTraite;
    int temps, retRecv;
    char *numThr = getThreadIdentity();
    int hSocketServ;
    int state = DISCONNECTED; //0 = non authentifié, 1=authentifié
    int terminal = -1;
    char heure[10], heure2[10];
    while (1)
    {
        /* 1. Attente d'un client à traiter */
        pthread_mutex_lock(&mutexIndiceCourant);
        while (indiceCourant == -1)
            pthread_cond_wait(&condIndiceCourant, &mutexIndiceCourant);
        iCliTraite = indiceCourant;
        indiceCourant = -1;
        hSocketServ = hSocketConnectee[iCliTraite];
        pthread_mutex_unlock(&mutexIndiceCourant);
        sprintf(buf, "Je m'occupe du numero %d ...", iCliTraite);
        affThread(numThr, buf);
        /* 2. Dialogue thread-client */
        finDialogue = 0;
        do
        {
            if ((retRecv = recv(hSocketServ, msgClient, MAXSTRING, 0)) == -1)
            {
                printf("Erreur sur le recv de la socket connectee : %d\n", errno);
                close(hSocketServ);
                exit(1);
            }
            else if (retRecv == 0)
            {
                sprintf(buf, "Le client est parti !!!");
                affThread(numThr, buf);
                finDialogue = 1;
                break;
            }
            else
            {
                sprintf(buf, "Message recu = %s\n", msgClient);
                affThread(numThr, buf);
            }
            if (strcmp(msgClient, EOC) == 0)
            {
                finDialogue = 1;
                break;
            }

            sprintf(msgServeur, "ACK pour votre message : <%s>", msgClient);
            finDialogue = actionServeur(msgClient, msgServeur, state, terminal, heure, heure2);
            if (send(hSocketServ, msgServeur, MAXSTRING, 0) == -1)
            {
                printf("Erreur sur le send de la socket %d\n", errno);
                close(hSocketServ); /* Fermeture de la socket */
                exit(1);
            }
            else
            {
                sprintf(buf, "Send socket connectee OK\n");
                affThread(numThr, buf);
            }
        } while (!finDialogue);
        /* 3. Fin de traitement */
        pthread_mutex_lock(&mutexIndiceCourant);
        hSocketConnectee[iCliTraite] = -1;
        pthread_mutex_unlock(&mutexIndiceCourant);
    }
    close(hSocketServ);
    return (void *)vr;
}

char *getThreadIdentity()
{
    unsigned long numSequence;
    char *buf = (char *)malloc(30);
    //numSequence = pthread_getsequence_np(pthread_self());// fonction non-implémentée sur Ubuntu
    sprintf(buf, "%d.%u", getpid(), pthread_self());
    return buf;
}

SocketServeur::~SocketServeur()
{
    /* 10. Fermeture de la socket d'ecoute */
    close(hSocketEcoute); /* Fermeture de la socket */
    printf("Socket serveur fermee\n");
    puts("Fin du thread principal");
}

int actionServeur(char *msgClient, char *msgServeur, int &state, int &terminal, char *heure, char *heure2)
{
    string msgc = msgClient;
    msgc = msgc.substr(0, msgc.find(FileRW::finTrame));
    int pos = 0, ret = 0;
    vector<string> params;
    string response = "VIDE" + FileRW::sepTrame;
    while ((pos = msgc.find(FileRW::sepTrame)) != std::string::npos)
    {
        params.push_back(msgc.substr(0, pos));
        msgc.erase(0, pos + FileRW::sepTrame.length());
    }
    if (params[0] == "LOGIN")
    {
        bool found = false;

        for (pos = 0; pos < FileRW::F_AGENTS.size() && !found; pos++)
        {
            if (FileRW::F_AGENTS[pos][0] == params[1])
                found = true;
        }
        if (found)
        {
            if (FileRW::F_AGENTS[pos - 1][1] == params[2])
            {
                terminal = stoi(params[3]);
                state = CONNECTED;
                response = "CONNECTED !"+ FileRW::sepTrame;
            }
            else
                response = "WRONG PWD !"+ FileRW::sepTrame;
        }
        else
            response = "AGENT NOT FOUND"+ FileRW::sepTrame;
    }

    if (params[0] == "ASK-NEXT-DEPARTURE")
    {
        if (state == DISCONNECTED)
        {
            response = "NOT CONNECTED !" + FileRW::sepTrame;
        }
        else
        {
            bool found = false;
            for (pos = 0; pos < FileRW::F_TERM.size() && !found; pos++)
            {
                if (FileRW::F_TERM[pos][0] == to_string(terminal))
                    found = true;
            }
            if (found)
            {
                if (FileRW::F_TERM[pos - 1][1] == "-")
                    response = "NO-FERRY" + FileRW::sepTrame;
                else{
                    if(FileRW::F_TERM[pos - 1][2] == "NA"){
                        response = "DEPARTURE-UNKNOWN" + FileRW::sepTrame;
                    }
                    else{
                        response = "DEPARTURE-KNOWN" + FileRW::sepTrame;
                        strcpy(heure, FileRW::F_TERM[pos - 1][2].c_str());
                    }
                    response = response + FileRW::F_TERM[pos - 1][1]+ FileRW::sepTrame
                    +FileRW::F_TERM[pos - 1][2] + FileRW::sepTrame
                    +FileRW::F_TERM[pos - 1][3] + FileRW::sepTrame;
                } 
                

                
            }
            else
                response = "BAD-TERMINAL" + FileRW::sepTrame;
        }
    }

    if(params[0] == "ASK-BEGIN-LOADING"){
        if (state == DISCONNECTED)
        {
            response = "NOT CONNECTED !" + FileRW::sepTrame;;
        }
        else
        {
            
            cout << "H Depart : "<< heure<<endl;
            cout << "H demandee : "<< params[1]<<endl;
            struct tm tm;
            strptime(params[1].c_str(), "%H-%M", &tm);
            time_t savedTime_t = mktime(&tm);
            strptime(heure, "%H-%M", &tm);
            time_t heure_t = mktime(&tm);
            cout << heure_t<<endl;
            cout << savedTime_t << endl;
            double diff = difftime(heure_t, savedTime_t);
            cout << "diff : "<<diff<<endl;
            if( diff > 0 && diff<(45*60)){
                state = DEPARTURE_PLANNED;
                response = "ACK" + FileRW::sepTrame;
                strcpy(heure2, params[1].c_str());
            }
            else{
                response = "FAIL" + FileRW::sepTrame;
            }
            
        }

    }

    if(params[0] == "NOTIFY-END-LOADING"){
        if (state == DEPARTURE_PLANNED)
        {
            cout << "H Depart : "<< heure2<<endl;
            cout << "H demandee : "<< params[1]<<endl;
            struct tm tm;
            strptime(params[1].c_str(), "%H-%M", &tm);
            time_t savedTime_t = mktime(&tm);
            strptime(heure2, "%H-%M", &tm);
            time_t heure_t = mktime(&tm);
            cout << heure_t<<endl;
            cout << savedTime_t << endl;
            double diff = difftime(savedTime_t, heure_t);
            cout << "diff : "<<diff<<endl;
            if( diff >= 0 && diff<(15*60)){
                state = DEPARTURE_LOADED;
                response = "ACK" + FileRW::sepTrame;
                strcpy(heure, params[1].c_str());
            }
            else{
                response = "FAIL" + FileRW::sepTrame;
            }
        }
        else
        {
            response = "NOT PLANNED !" + FileRW::sepTrame;
        }
    }

    if(params[0] == "FERRY-LIVING"){
        if (state == DEPARTURE_LOADED){
            FileRW::removeFerry(terminal);
            state = CONNECTED;
            response = "FERRY LIVED !" + FileRW::sepTrame;
        }
        else{
            response = "NOT LOADED !" + FileRW::sepTrame;
        }
    }

    if(params[0] == "ASK-FOR-FERRY"){
        if (state == DISCONNECTED)
        {
            response = "NOT CONNECTED !" + FileRW::sepTrame;
        }
        else
        {
            state = FERRY_ASKED;
            string ferryName = FileRW::addFerry(terminal);
            response = ferryName + FileRW::sepTrame;
        }
        
    }

    if(params[0] == "FERRY-ARRIVING"){
        if(state == FERRY_ASKED){
            if(FileRW::F_TERM[terminal-1][1] == params[1]){
                response = "ACK" + FileRW::sepTrame;
            }
            else
            {
                response = "FAIL" + FileRW::sepTrame;
            }
            
        }
        else{
            response = "FERRY NOT ASKED !" + FileRW::sepTrame;
        }
    }

    if (params[0] == "CLOSE")
    {
        state = DISCONNECTED;
        terminal = -1;
            time_t rawtime;
        struct tm* tinfo;
        char time_str[10];
        time (&rawtime);
        tinfo = localtime(&rawtime);
        strftime (time_str,80,"%H-%M",tinfo);
        string time_string = time_str;
        response = time_string + FileRW::sepTrame;
        ret = 1;
    }
    response = response + FileRW::finTrame;
    string log = msgClient;
    log = log  + " -> " + response;
    FileRW::writeLog(log);
    strcpy(msgServeur, response.c_str());
    return ret;
}
