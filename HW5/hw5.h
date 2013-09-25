//
//Author: William (Austin) Horne
//Date:   5/3/13
//Course: CSCI 4727
//Purpose:Header file for hw5.cc
//

#include <string>
#include <fstream>
using namespace std;

class hw5
{

private:

   string fileName;
   string userName;
   string logFileName;
   string homeDir;
   string serverName;
   string portNumber;
   fstream logFile;

public: 

   //default constructor
   hw5();

   //parameterized constructor
   hw5(string f, string u, string l);


   //setters
   void setFile(string f);
   void setUser(string u);
   void setLog(string l);
   void setServer(string server);
   void setPort(string port);
   void setHomeDir(char* path);

   //getters
   string getFile();
   string getUser();
   string getLog();
   string getHomeDir();
   string getServer();
   string getPort();
   
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

};//end of hw5.h
