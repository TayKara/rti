#include "./FileReader.h"

using namespace std;

int FileReader::portService;
int FileReader::portAdmin;
string FileReader::sepTrame;
string FileReader::finTrame;
string FileReader::sepCSV;
string FileReader::pwdMaster;
string FileReader::pwdAdmin;
vector<vector<string> > FileReader::F_AGENTS;
vector<vector<string> > FileReader::F_TERM;
vector<vector<string> > FileReader::F_WAITING;

int FileReader::init()
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

int FileReader::init_f()
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
            if ((pos = lineAgent.find(FileReader::sepCSV)) != std::string::npos)
            {
                elements.push_back(lineAgent.substr(0, pos));
                elements.push_back(lineAgent.substr(pos+1, lineAgent.length()));
                F_AGENTS.push_back(elements);
                elements.clear();
            }
            
        }
    }
    return 0;
}
