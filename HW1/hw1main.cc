//
//Author: William (Austin) Horne
//Date:   2/1/13
//Course: CSCI 4727
//Purpose:Gain practice with C/C++ and Linux.
//	  Accepts command line arguments and creates a directory in
// 	  which to record a log file.

#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

//header file for hw1 class
#ifndef HW1_H
#include "hw1.h"
#endif

using namespace std;

#define STRLEN 32 //c constant used for c strings

//handles all io and sets hw1 class variables
int main(int argc, char** argv)
{
   char opt; //the command line option entered.
   char cmd; //the single character command entered
   char strUser[STRLEN - 1] = ""; //user name entered at command line
   char strFile[STRLEN - 1] = ""; //file that includes commands to be executed
   char strLog[STRLEN - 1 ] = ""; //name of the log file
   hw1 log; //hw1 object
   char path[STRLEN - 1] = ""; //directory to create 
   int i;  //used for error testing 
   fstream file; //used for output to the log file

   strncpy(path, "master", STRLEN - 1);

   cout << "# of args " << argc << endl;
   while( ( opt = getopt(argc, argv, "u:f:l:")) != -1)
   {
      switch (opt)
      {
         case 'u':
	    strncpy(strUser, optarg, STRLEN-1);
	    break;
	 case 'f':
	    strncpy(strFile, optarg, STRLEN-1);
	    break;
	 case 'l':
	    strncpy(strLog, optarg, STRLEN-1);
	    break;
      };//end switch

   }//end while

   //check to see if user option was entered at command line
   //set userName field if yes, print error and exit if no
   if (strlen(strUser) < 1)
   {
     cout << "Required user name not found. Terminating" << endl;
     exit(0);
   } 
   else
   {
     log.setUser(strUser);
   }
  
   //check for l option
   //set logFileName to command line name if yes, set to log.txt if no 
   if(strlen(strLog) < 1)
   {
      log.setLog("log.txt");
   }
   else
   {
     log.setLog(strLog);
   }
  
   //make directory for log file. Grant user all permissions. 
   i = mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR);   

   //notify of successful creation or directory already in existence.
   if (i == -1)
   {
      cout << "Directory already exists" << endl;
   }
   else
   {
      cout << "Directory " << path << " created." << endl;
   }

   i = chdir(path); //change to new directory

   //notify of successful directory change   
   if(i == 0)
   {
      cout << "Directory changed to: " << path << endl;
   }
   else
   {
      cout << "Failed to change directory." << endl;
   }

   //create/open file with name of log file. 
   string fName(log.getLog());  
   file.open(fName.c_str(), fstream::out);

   //check for file successfully opened. Write user name if yes.
   if(!file)
   {
      cout << "Failed to open file" << endl;
   }
   else
   {
      file << "user:" << log.getUser() << endl;
      file.close();
   }
  
   //get single character commands until character q entered
   cout << "Enter single character command" << endl;
   cin >> cmd;
   while(cmd != 'q')
   {
      cout << "Command entered: " << cmd << endl;
      cout << "Enter next command" << endl;
      cin >> cmd;
   }

   cout << "Command q entered. Terminating." << endl;

   return 0;  
}//end main
