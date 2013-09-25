//
//Author:  William (Austin) Horne
//Date:    2/19/13
//Purpose: Process command line arguments and information for log file.
//Course:  CSCI 4727
//


#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>

#ifndef HW2_H
#include "hw2.h"
#endif

//default constructor
hw2::hw2()
{
   fileName = "";
   userName = "";
   logFileName = "log.txt";
   cmd = ' ';
}//end hw2()

//parameterized constructor
hw2::hw2(string file, string user, string log)
{
   fileName = file;
   userName = user;
   logFileName = log;

}//end hw2(string, string, string) 

//setters
void hw2::setFile(string f)
{
   fileName = f;
}//end setFile()

void hw2::setUser(string u)
{
   userName = u;
}//end setUser()

void hw2::setLog(string l)
{
   logFileName = l;
}//end setLog()

void hw2::setCmd(char c)
{
   cmd = c;
}//end setCmd()

void hw2::setMD(bool md)
{
   mdSuccess = md;
}//end setMD(bool)

void hw2::setCD(bool cd)
{
   cdSuccess = cd;
}//setCD(bool)

//getters
string hw2::getFile()
{
   return fileName;
}//end getFile()

string hw2::getUser()
{
   return userName;
}//getUser()

string hw2::getLog()
{
   return logFileName;
}//getLog()

bool hw2::getMD()
{
   return mdSuccess;
}//getMD()

bool hw2::getCD()
{
   return cdSuccess;
}//getCD()

char* hw2::getCommands()
{
   return commands;
}//getCommands()

//Process command line arguments and set fields accordingly.
int hw2::processArgs(int argc, char** argv)
{
   char opt;
   
   //check for no args
   if(argc == 1)
   {
      return -1;
   } 
   
   //get args while there are args left to process
   while( ( opt = getopt(argc, argv, "u:f:l:")) != -1)
   {
      switch (opt)
      {
         case 'u':
		setUser(optarg);
	    break;
	 case 'f':
		setFile(optarg);
	    break;
	 case 'l':
		setLog(optarg);
	    break;
      }//switch
   }//while

   //check for user name not specified   
   if(getUser().length() == 0)
   {
      return -1;
   }

   return 0;

}//processArgs(int, string)

//attempts to open the log file. Returns true if successful, false if not.
bool hw2::openLog()
{
   string fName(getLog()); 
   logFile.open(fName.c_str(), fstream::out);

   //check for file successfully opened. Write user name if yes.
   if(!logFile)
   {
      return false;
   }
   return true;
}//openLog()

//close the log file.
void hw2::closeLog()
{
   if(logFile)
   {
      logFile.close();
   }
}//closeLog();

//open cmd file
int hw2::openCMDFile()
{
   string cmdFileName(getFile()); 
   cmdFile.open(cmdFileName.c_str(), fstream::out);
   
   //check for file successfully opened.
   if(!cmdFile)
   {
      return -1;
   }
   return 0;
}//openCMDFile()

//close the command file.
void hw2::closeCMDFile()
{
   if(cmdFile)
   {
      cmdFile.close();
   }
}//closeCMDFile();

//if there were errors making or changing to master directory, write to log file
void hw2::logErrors(char* path)
{
   //write error to file if mkdir() was unsuccessul 
   if(!getMD())
   {
      logFile << "Failed to make new directory " << path << endl;
   }
   //write error to file if chdir() was unsuccessful
   if(!getCD())
   { 
      logFile << "Failed to change to directory " << path << endl;
   }
}//logErrors()

//write parameter to the log file
void hw2::writeLnToLog(string str)
{
   logFile << str << endl;
}//writeLnToLog

//read in commands from keyboard and write them to log file
void hw2::cmdFromKB()
{
   //get single character commands until character q entered
   cin >> cmd;
   while(cmd != 'q')
   {
      logFile << cmd << endl;
      cout << "Command entered: " << cmd << endl;
      cout << "Enter next command" << endl;
      cin >> cmd;
   }//while

   logFile << cmd << endl;
}//cmdFromKB()
