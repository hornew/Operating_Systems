//
//Author: William (Austin) Horne
//Date:   2/19/13
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

//header file for hw2 class
#ifndef HW2_H
#include "hw2.h"
#endif

using namespace std;

#define STRLEN 32 //c constant used for c strings

//handles all io and sets hw2 class variables
int main(int argc, char** argv)
{
   char opt; //the command line option entered.
   char strUser[STRLEN - 1] = ""; //user name entered at command line
   char strFile[STRLEN - 1] = ""; //file that includes commands to be executed
   char strLog[STRLEN - 1 ] = ""; //name of the obj file
   hw2 obj; //hw2 object
   fstream cf;
   char path[STRLEN - 1] = ""; //directory to create 
   string cFile;
   string cmd; //the single character command entered
   int i;  //used for error testing 
   int err;
   bool fOption = false;

   strncpy(path, "master", STRLEN - 1);

   cout << "# of args " << argc << endl;

   //process command line arguments
   if(obj.processArgs(argc, argv) == -1)
   {
      cout << "Error. Exiting" << endl;
      exit(0);
   }
   
   //check for command file option specified
   if(strlen(obj.getFile().c_str()) > 0)
   {
      fOption = true;
      cFile = obj.getFile();
      cf.open(cFile.c_str()); 
      
      if(!cf)
      {
         err = -1;
      }
      else
      {
         err = 0;
      }
   }//end if - checks for command file option 

   //set objFileName to command line name if yes, set to obj.txt if no 
   if(strlen(obj.getLog().c_str()) < 1)
   {
      obj.setLog("log.txt");
   }
   
   //make directory for obj file. Grant user all permissions. 
   i = mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR);   

   //notify of successful creation or directory already in existence.
   if (i == -1)
   {
      cout << "Directory already exists" << endl;
      obj.setCD(false);
   }
   else
   {
      cout << "Directory " << path << " created." << endl;
      obj.setMD(true);
   }

   i = chdir(path); //change to new directory

   //notify of successful directory change   
   if(i == 0)
   {  
      obj.setCD(true);
      cout << "Directory changed to: " << path << endl;
   }
   else
   {
      cout << "Failed to change directory." << endl;
      obj.setCD(false); 
   }

   //check for file successfully opened. Write user name if yes.
   if(!obj.openLog())
   {
      cout << "Failed to open log file!" << endl;
      exit(0);
   }

   obj.writeLnToLog("user: " + obj.getUser());    //write user name to log file
   obj.logErrors(path);

   if(!fOption) //checks for not command file given.
   {
      cout << "Command file option not specified. Reading commands from keyboard." << endl;
      obj.writeLnToLog("Command file option not specified.");
      obj.writeLnToLog("Reading commands from keyboard.");
      obj.cmdFromKB();
      cout << "Command q entered. Terminating." << endl;      
   }
   else //command file given
   {
      if(err == 0) //if no error occurred opening the file 
      {
	 cout << "Command file opened successfully.";
	 cout << " Attempting to read commands from file." << endl;
	 obj.writeLnToLog("Command file opened successfully."); 
	 obj.writeLnToLog("Attempting to read commands from file.");
	 while(!cf.eof() && cmd != "q")
	 {
	    cf >> cmd; 
	    obj.writeLnToLog(cmd);	    
	 }
	 if(cmd == "q")
	 {   
	    cout << "Command q entered. Terminating." << endl;
	 }
	 else
	 {
	    cout << "End of file reached. Terminating." << endl;
	 }
      }
      else //if error occurred opening file, read commands from keyboard
      {
	 cout << "Could not open command file!\nReading commands from keyboard." << endl;
	 obj.writeLnToLog("Could not open command file! Reading commands from keyboard.");
	 obj.cmdFromKB();
         cout << "Command q entered. Terminating." << endl;
      }  
   }//command file option specified
  
   //close log file and command file 
   obj.closeLog(); 
   cf.close();
   return 0;  
}//end main
