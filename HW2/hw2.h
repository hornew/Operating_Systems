//
//Author: William (Austin) Horne
//Date:   2/19/13
//Course: CSCI 4727
//Purpose:Header file for hw2.cc
#include <string>
#include <fstream>
using namespace std;

class hw2
{

private:

   string fileName;
   string userName;
   string logFileName;
   char cmd;
   bool mdSuccess;
   bool cdSuccess;
   fstream logFile;
   fstream cmdFile;
   char* commands;

public: 

   //default constructor
   hw2();

   //parameterized constructor
   hw2(string f, string u, string l);


   //setters
   void setFile(string f);
   void setUser(string u);
   void setLog(string l);
   void setCmd(char c);
   void setMD(bool md);
   void setCD(bool cd);

   //getters
   string getFile();
   string getUser();
   string getLog();
   char getCmd();
   bool getMD();
   bool getCD();
   char* getCommands();

   //other methods
   int processArgs(int argc, char** argv);   

   bool openLog();

   void closeLog();

   void logErrors(char* path);

   void cmdFromKB();

   bool cmdFromFile();

   int openCMDFile();

   void closeCMDFile();

   void writeLnToLog(string str);

};//end of hw2.h
