//
//Author:  William (Austin) Horne
//Date:    2/1/13
//Purpose: Hold information for log file.
//Course:  CSCI 4727
//


#include <string>
#include <stdio.h>

#ifndef HW1_H
#include "hw1.h"
#endif

//default constructor
hw1::hw1()
{
   fileName = "";
   userName = "";
   logFileName = "";
   cmd = ' ';
}//end hw1()

//parameterized constructor
hw1::hw1(string file, string user, string log)
{
   fileName = file;
   userName = user;
   logFileName = log;

}//end hw1(string, string, string) 

//setters
void hw1::setFile(string f)
{
   fileName = f;
}//end setFile()

void hw1::setUser(string u)
{
   userName = u;
}//end setUser()

void hw1::setLog(string l)
{
   logFileName = l;
}//end setLog()

void hw1::setCmd(char c)
{
   cmd = c;
}//end setCmd()


//getters
string hw1::getFile()
{
   return fileName;
}//end getFile()

string hw1::getUser()
{
   return userName;
}//getUser()

string hw1::getLog()
{
   return logFileName;
}//getLog()
