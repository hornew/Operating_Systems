//
//Author: William (Austin) Horne
//Date:   4/5/13
//Course: CSCI 4727
//Purpose:Header file for hw4.cc
//

#include <string>
#include <fstream>
using namespace std;

class hw4
{

private:

   string fileName;
   string userName;
   string logFileName;
   string homeDir;
   fstream logFile;

public: 

   //default constructor
   hw4();

   //parameterized constructor
   hw4(string f, string u, string l);


   //setters
   void setFile(string f);
   void setUser(string u);
   void setLog(string l);
   void setHomeDir(char* path);

   //getters
   string getFile();
   string getUser();
   string getLog();
   string getHomeDir();
   
   //other methods
   int processArgs(int argc, char** argv);   

   bool openLog();

   void closeLog();

   int openCMDFile();

   void closeCMDFile();

   void writeLnToLog(string str);

   void changeDir(string s);

   int processCmd(char cmd, string s);

   int list();

   int goHome();

   int dir(string s);

   int validate(string s);

};//end of hw4.h
