#ifndef FILERW_H
#define FILERW_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class FileRW{
    private:

    public :
        static int portService;
        static int portAdmin;
        static string sepTrame;
        static string finTrame;
        static string sepCSV;
        static string pwdMaster;
        static string pwdAdmin;
        static vector<vector<string> > F_AGENTS;
        static vector<vector<string> > F_TERM;
        static vector<vector<string> > F_WAITING;
        static int init();
        static int init_f();
        static int removeFerry(int terminal);
        static int placeFerry(int terminal, string ferry);
        static string addFerry(int terminal);
        static int writeLog(string log);
        
};

#endif