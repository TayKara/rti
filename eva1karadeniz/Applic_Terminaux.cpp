#include "./SocketClient.h"
#include <ctime>

void traiterMessage(char *);
void departureKnown();
void departureUnknown();
void noFerry();
vector<string> params;
SocketClient s;
char msg[MAXSTRING];

int main()
{
    char choix;
    char user[30];
    char password[30];
    int terminal;

    do
    {
        cout << "1. LOGIN" << endl;
        cout << "2. ASK_NEXT_DEPARTURE" << endl;
        cout << "0. CLOSE" << endl;

        choix = getchar();
        switch (choix)
        {
        case '1':
            cout << "USER : ";
            //fgets(user, sizeof(user), stdin);
            cin >> user;
            cout << "PASSWORD : ";
            //fgets(password, sizeof(password), stdin);
            cin >> password;
            cout << "TERMINAL (1-4)" << endl;
            cin >> terminal;
            sprintf(msg, "LOGIN|%s|%s|%d|#", user, password, terminal);

            s.sendmsg(msg);
            if (msg == NULL)
            {
                cout << "ERROR" << endl;
            }
            else
            {
                cout << msg << endl;
            }
            break;

        case '2':
            sprintf(msg, "ASK-NEXT-DEPARTURE|#");
            s.sendmsg(msg);
            if (msg == NULL)
            {
                cout << "ERROR" << endl;
            }
            else
            {
                traiterMessage(msg);
                if (params.size() > 0)
                {
                    if (params[0] == "DEPARTURE-KNOWN")
                    {
                        departureKnown();
                        if (msg == NULL)
                        {
                            cout << "ERROR" << endl;
                        }
                        else
                        {
                            cout << msg << endl;
                        }
                    }
                    if (params[0] == "DEPARTURE-UNKNOWN")
                    {
                        departureUnknown();

                        if (msg == NULL)
                        {
                            cout << "ERROR" << endl;
                        }
                        else
                        {
                            cout << msg << endl;
                        }
                    }
                    if (params[0] == "NO-FERRY")
                    {
                        noFerry();
                        if (msg == NULL)
                        {
                            cout << "ERROR" << endl;
                        }
                        else
                        {
                            cout << msg << endl;
                        }
                    }
                }
            }

            break;

        case '0':
            sprintf(msg, "CLOSE|#");
            s.sendmsg(msg);
            if (msg == NULL)
            {
                cout << "ERROR" << endl;
            }
            else
            {
                cout << msg << endl;
            }
            break;

        default:
            break;
        }
    } while (choix != 0);

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
    struct tm* tinfo;
    char time_str[10];
    time (&rawtime);
    tinfo = localtime(&rawtime);
    strftime (time_str,80,"%H-%M",tinfo);
    string time_string = time_str;

    string response = "BLABLA";

    cout << "1. ASK-BEGIN-LOADING" << endl;
    cout << "2. NOTIFY-END-LOADING" << endl;
    cout << "0. FERRY-LIVING" << endl;


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
}

void departureUnknown()
{
    cout << params[0] << endl;
}

void noFerry()
{
    cout << "1. ASK-FOR-FERRY" << endl;
    cout << "2. FERRY-ARRIVING" << endl;
    cout << "0. CLOSE" << endl;
}