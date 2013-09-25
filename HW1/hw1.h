//
//Author: William (Austin) Horne
//Date:   2/1/13
//Course: CSCI 4727
//Purpose:Header file for hw1.cc
#include <string>

using namespace std;

class hw1
{

private:

   string fileName;
   string userName;
   string logFileName;
   char cmd;

public: 

   //default constructor
   hw1();

   //parameterized constructor
   hw1(string f, string u, string l);


   //setters
   void setFile(string f);
   void setUser(string u);
   void setLog(string l);
   void setCmd(char c);

   //getters
   string getFile();
   string getUser();
   string getLog();
   char getCmd();

};//end of hw1.h
