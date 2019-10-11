#include "./SocketClient.h"
#include <ctime>

void traiterMessage(char *);
void departureKnown();
void departureUnknown();
int noFerry();
vector<string> params;
SocketClient s;
char msg[MAXSTRING];

int main()
{
    bool finCon = false;
    char choix;
    string user;
    string password;
    int terminal;
    string request;
    do
    {
        if(params.size()>0)
            params.clear();

        cout << "1. LOGIN" << endl;
        cout << "2. ASK_NEXT_DEPARTURE" << endl;
        cout << "0. CLOSE" << endl;

        cin >> choix;
        switch (choix)
        {
        case '1':
            cout << "USER : ";
            cin >> user;
            cout << "PASSWORD : ";
            cin >> password;
            cout << "TERMINAL (1-4)" << endl;
            cin >> terminal;
            request = "LOGIN|" + user + "|" + password + "|" + to_string(terminal) + "|#";
            break;

        case '2':
            request = "ASK-NEXT-DEPARTURE|#";
            break;
        case '0':
            request = "CLOSE|#";
            finCon = true;
            break;
        default:
            break;
        }
        strcpy(msg, request.c_str());
        s.sendmsg(msg);
        traiterMessage(msg);
        cout << params[0] << endl;
        if ( request == "ASK-NEXT-DEPARTURE|#" && params.size() > 0)
        {
            if (params[0] == "DEPARTURE-KNOWN")
            {
                departureKnown();
                if (msg == NULL)
                {
                    cout << "ERROR" << endl;
                }
            }
            if (params[0] == "DEPARTURE-UNKNOWN")
            {
                departureUnknown();

                if (msg == NULL)
                {
                    cout << "ERROR" << endl;
                }
            }
            if (params[0] == "NO-FERRY")
            {
                if (noFerry() == 0)
                {
                    finCon = true;
                }
                if (msg == NULL)
                {
                    cout << "ERROR" << endl;
                }
            }
            cout << params[0] << endl;
        }
    } while (!finCon);

    return 0;
}

void traiterMessage(char *msg)
{
    string msgc = msg;
    string finTrame = "#";
    string sepTrame = "|";
    msgc = msgc.substr(0, msgc.find(finTrame));
    int pos = 0, ret = 0;
    if (params.size() > 0)
        params.clear();
    string response = "BLABLA";
    while ((pos = msgc.find(sepTrame)) != std::string::npos)
    {
        params.push_back(msgc.substr(0, pos));
        msgc.erase(0, pos + sepTrame.length());
    }
}

void departureKnown()
{
    time_t rawtime;
    struct tm *tinfo;
    char time_str[10];
    time(&rawtime);
    tinfo = localtime(&rawtime);
    strftime(time_str, 80, "%H-%M", tinfo);
    string time_string = time_str;

    string response = "VIDE|#";

    cout << "1. ASK-BEGIN-LOADING" << endl;
    cout << "2. NOTIFY-END-LOADING" << endl;
    cout << "3. FERRY-LIVING" << endl;

    int choix = 0;

    cin >> choix;

    switch (choix)
    {
    case 1:
        response = "ASK-BEGIN-LOADING|" + time_string + "|#";
        break;

    case 2:
        response = "NOTIFY-END-LOADING|" + time_string + "|#";
        break;

    case 3:
        response = "FERRY-LIVING|" + time_string + "|#";
        break;

    default:
        response = "ERROR|#";
        break;
    }
    strcpy(msg, response.c_str());
    s.sendmsg(msg);
    traiterMessage(msg);
    
}

void departureUnknown()
{
    
}

int noFerry()
{
    cout << "1. ASK-FOR-FERRY" << endl;
    cout << "2. FERRY-ARRIVING" << endl;
    cout << "0. CLOSE" << endl;

    string response = "BLABLA";
    int choix = 0;

    cin >> choix;

    switch (choix)
    {
    case 1:
        response = "ASK-FOR-FERRY|#";
        break;

    case 2:
        response = "FERRY-ARRIVING|" + params[0] + "|#";
        break;

    case 0:
        response = "CLOSE|#";
        break;

    default:
        response = "ERROR|#";
        break;
    }
    strcpy(msg, response.c_str());
    s.sendmsg(msg);
    traiterMessage(msg);
    if (choix == 1){
        cout << params[0] << endl;
        noFerry();
    }
    return choix;
}