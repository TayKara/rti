#include "./FileRW.h"

using namespace std;

int FileRW::portService;
int FileRW::portAdmin;
string FileRW::sepTrame;
string FileRW::finTrame;
string FileRW::sepCSV;
string FileRW::pwdMaster;
string FileRW::pwdAdmin;
vector<vector<string>> FileRW::F_AGENTS;
vector<vector<string>> FileRW::F_TERM;
vector<vector<string>> FileRW::F_WAITING;

int FileRW::init()
{
    ifstream cFile("Serveur_Terminaux.conf");
    if (cFile.is_open())
    {
        string line;
        while (getline(cFile, line))
        {
            if (line[0] == '#' || line.empty())
                continue;
            int delimiterPos = line.find("=");
            string name = line.substr(0, delimiterPos);
            string value = line.substr(delimiterPos + 1);
            if (name == "Port_Service")
                portService = stoi(value);
            if (name == "Port_Admin")
                portAdmin = stoi(value);
            if (name == "sep-trame")
                sepTrame = value;
            if (name == "fin-trame")
                finTrame = value;
            if (name == "sep-csv")
                sepCSV = value;
            if (name == "pwd-master")
                pwdMaster = value;
            if (name == "pwd-admin")
                pwdAdmin = value;
        }
    }
    else
    {
        cerr << "Couldn't open config file for reading.\n";
    }
    return 0;
}

int FileRW::init_f()
{
    ifstream fAgents("F_AGENTS");
    ifstream fTerm("F_TERM");
    ifstream fWaiting("F_WAITING");

    if (F_AGENTS.size() > 0)
    {
        F_AGENTS.clear();
    }
    if (F_TERM.size() > 0)
    {
        F_TERM.clear();
    }
    if (F_WAITING.size() > 0)
    {
        F_WAITING.clear();
    }

    if (fAgents.is_open())
    {
        string lineAgent;
        while (getline(fAgents, lineAgent))
        {
            vector<string> elements;
            int pos = 0;
            if ((pos = lineAgent.find(FileRW::sepCSV)) != std::string::npos)
            {
                elements.push_back(lineAgent.substr(0, pos));
                elements.push_back(lineAgent.substr(pos + 1, lineAgent.length()));
                F_AGENTS.push_back(elements);
                elements.clear();
            }
        }
    }
    if (fTerm.is_open())
    {
        string lineTerm;
        while (getline(fTerm, lineTerm))
        {
            vector<string> elements;
            int pos = 0;
            while ((pos = lineTerm.find(FileRW::sepCSV)) != std::string::npos)
            {
                elements.push_back(lineTerm.substr(0, pos));
                lineTerm.erase(0, pos + FileRW::sepCSV.length());
            }
            F_TERM.push_back(elements);
            elements.clear();
        }
    }
    if (fWaiting.is_open())
    {
        string lineWaiting;
        while (getline(fWaiting, lineWaiting))
        {
            vector<string> elements;
            int pos = 0;
            while ((pos = lineWaiting.find(FileRW::sepCSV)) != std::string::npos)
            {
                elements.push_back(lineWaiting.substr(0, pos));
                lineWaiting.erase(0, pos + FileRW::sepCSV.length());
            }

            F_WAITING.push_back(elements);
            elements.clear();
        }
    }
    return 0;
}

int FileRW::removeFerry(int terminal)
{
    fstream fTerm;
    string lineTerm;
    string newContent = "";
    fTerm.open("F_TERM", fstream::in);
    while (getline(fTerm, lineTerm))
    {
        if (lineTerm.substr(0, lineTerm.find(FileRW::sepCSV)) == to_string(terminal))
        {
            newContent = newContent + to_string(terminal) + ";-;NA;-\n";
        }
        else
        {
            newContent = newContent + lineTerm + "\n";
        }
    }
    fTerm.close();
    fTerm.open("F_TERM", fstream::out);
    fTerm << newContent;
    fTerm.close();

    init_f();
    return 0;
}

string FileRW::addFerry(int terminal){
    init_f();
    fstream fWaiting, fTerm;
    string lineWaiting = "NO WAITING FERRY !";
    fWaiting.open("F_WAITING", fstream::in);
    if (getline(fWaiting, lineWaiting))
    {
        placeFerry(terminal, lineWaiting);

        lineWaiting = lineWaiting.substr(0, lineWaiting.find(FileRW::sepCSV));
    }
    return lineWaiting;
}

int FileRW::placeFerry(int terminal, string ferry){
    fstream fTerm;
    string lineTerm;
    string newContent = "";
    fTerm.open("F_TERM", fstream::in);
    while (getline(fTerm, lineTerm))
    {
        if (lineTerm.substr(0, lineTerm.find(FileRW::sepCSV)) == to_string(terminal))
        {
            newContent = newContent + to_string(terminal) + FileRW::sepCSV + ferry + "\n";
        }
        else
        {
            newContent = newContent + lineTerm + "\n";
        }
    }
    fTerm.close();
    fTerm.open("F_TERM", fstream::out);
    fTerm << newContent;
    fTerm.close();

    init_f();
    return 0;
}

int FileRW::writeLog(string log){
    time_t rawtime;
    struct tm *tinfo;
    char time_str[10];
    time(&rawtime);
    tinfo = localtime(&rawtime);
    strftime(time_str, 80, "[%H:%M:%S]", tinfo);
    string time_string = time_str;

    fstream fLogs;
    fLogs.open("logs.txt", fstream::out | fstream::app);
    fLogs << time_string+log+"\n";
    return 0;
}
