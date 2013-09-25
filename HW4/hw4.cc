//
//Author:  William (Austin) Horne
//Date:    4/6/13
//Purpose: Process command line arguments and information for log file.
//Course:  CSCI 4727
//


#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#ifndef HW4_H
#include "hw4.h"
#endif


//default constructor
hw4::hw4()
{
   fileName = "";
   userName = "";
   logFileName = "log.txt";
}//end hw4()

//parameterized constructor
hw4::hw4(string file, string user, string log)
{
   fileName = file;
   userName = user;
   logFileName = log;

}//end hw4(string, string, string) 

//setters
void hw4::setFile(string f)
{
   fileName = f;
}//end setFile()

void hw4::setUser(string u)
{
   userName = u;
}//end setUser()

void hw4::setLog(string l)
{
   logFileName = l; 
}//end setLog()

void hw4::setHomeDir(char * path)
{
   if(strlen(path) > 0)
	homeDir = path;
}//setHomeDir(string)

string hw4:: getHomeDir()
{
	return homeDir;
}//getHomeDir()


//getters
string hw4::getFile()
{
   return fileName;
}//end getFile()

string hw4::getUser()
{
   return userName;
}//getUser()

string hw4::getLog()
{
   return logFileName;
}//getLog()

//Process command line arguments and set fields accordingly.
int hw4::processArgs(int argc, char** argv)
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
bool hw4::openLog()
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
void hw4::closeLog()
{
   if(logFile)
   {
      logFile.close();
   }
}//closeLog();


//write parameter to the log file
void hw4::writeLnToLog(string str)
{
   logFile << str << endl;
}//writeLnToLog

//processes commands using a switch on cmd
int hw4::processCmd(char cmd, string param)
{
   int err;
   char c = cmd;
   string p = param;

   switch(c)
   {
	case 'h':
	   goHome();
	   break;
	case 'q':
	   return 1; //signal to exit
	   break;
	case 'd':
	   if(dir(p) == -1)
	     cout << "Illegal string given with d command." << endl;
	   break;
	case 'c':
	   changeDir(p);
	   break;
	case 'l':
	   err = list();	
	   if(err == -1)
	   	cout << "Error opening current directory. Could not list contents." << endl;
	   break;
	default:
	   return -1; //signal command not found
   }//switch

   return 0;
}//processCmd(string) 

int hw4::list()
{
   int i = 1;
   char cwd[1024];
   DIR *dirptr;
   struct dirent *entry;

   if(getcwd(cwd, sizeof(cwd)) == NULL)
	return -1;
   //check for error opening current directory
   if((dirptr = opendir(cwd)) == NULL)
   	return -1;

   cout << endl;
   cout << "Directory listing for:" << endl << getcwd(cwd, sizeof(cwd)) << endl;
   while((entry = readdir(dirptr)) != NULL)
   {
	cout << i << " " << entry->d_name << endl;
	i++;	
   }//while
   
   return 0;
}//list()

//go to the home directory, "master"
int hw4::goHome()
{
    if(chdir(getHomeDir().c_str()) < 0)
		cout << "Error changing to home directory." << endl;   
    else
	cout << "Changed to home directory." << endl;   
}//home()

//create new directory. Called by command d
int hw4::dir(string s)
{
	//check for instances of ".." or "/", which are illegal for this command
	//if yes, return an error code.
	if(s.find("..") != string::npos)
		return -1;
	if(s.find("/") != string::npos)
		return -1;
		
	//create directory with all user and group privileges.	
	//if unsuccessful, determine whether directory already exists 
	if(mkdir(s.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP) == -1)
	{
	   if(access(s.c_str(), F_OK) == 0)
		cout << endl << "Directory " << s << " already exists. " << endl;
	   else
		cout << endl << "Failed to make directory " << s << endl;
	}
	else
	{	
		cout << "Directory " << s << " created" << endl;
	}
	return 0;
}//dir(string)

//change directories only below the master directory
void hw4::changeDir(string param)
{
    string s = param;
    char cwd[256];
    int val = validate(s);
    if(val == 0)
	{
		if(chdir(s.c_str()) == -1) //error changing directories
		   cout << "Error changing to " << s << endl;
		else	//successfully changed
		   cout << "Changed directories to " << getcwd(cwd, sizeof(cwd)) << endl;
	}
	else if(val == -1)
	{
		cout << "Bad string given with c command." << endl;
	}
  
}//changeDir(string)

//determine whether a change directory path should be allowed 
int hw4::validate(string s)
{
	char currentDir[256];	
	string home;
	
	//get current directory
	if(getcwd(currentDir, 255) == NULL)
	{
		cout << "Error getting current directory." << endl;
		return 1;
	}
	   
	string current(currentDir);
	home = getHomeDir();
	   
	//if path is only ".." and not in home dir 
	if(s == ".." && current != home)
		return 0;
			
	if(s.find("..") != string::npos)	//".." used in conjuntion
		return -1;
	if(s.find("/") != string::npos)		//use of "/" disallowed
		return -1;
		
	//all good, return 0
	return 0;
}//dots
