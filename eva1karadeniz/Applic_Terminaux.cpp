#include "./SocketClient.h"

int main(){
    SocketClient s;
    char choix;
    char user[30];
    char password[30];
    int terminal;
    char msg[MAXSTRING];

    do{
        cout << "1. LOGIN"<<endl;
        cout << "2. ASK_NEXT_DEPARTURE"<<endl;
        cout << "3. CLOSE"<<endl;

        choix = getchar();
        switch (choix)
        {
            case '1':
                    cout <<"USER : ";
                    //fgets(user, sizeof(user), stdin);
                    cin >> user;
                    cout <<"PASSWORD : ";
                    //fgets(password, sizeof(password), stdin);
                    cin >> password;
                    cout <<"TERMINAL (1-4)"<<endl;
                    cin >> terminal;
                    sprintf(msg, "LOGIN|%s|%s|%d|#", user, password, terminal);

                    s.sendmsg(msg);
                    if(msg == NULL){
                        cout << "ERROR"<<endl;

                    }
                    else{
                        cout << msg <<endl;
                    }
                break;

            case '2':

                break;

            case '3':

                break;

            default:
                break;
        }
    }while(1);
    

    return 0;
}