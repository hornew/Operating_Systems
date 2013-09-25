//
//Author: William (Austin) Horne
//Date:   4/6/13
//Course: CSCI 4727
//Purpose:Gain practice with C/C++ and Unix/Linux. 
//	  Particulary focused on threads and counting semaphores for this assignment.
//	  Accepts command line arguments and creates a directory in
// 	  which to record a log file. Accepts commands from either the keyboard
//	  or from file and executes those commands using threads which share data
//	  using queues. A reader thread reads the commands, passes them to a 
//	  processor thread which executes the commands and passes them to a logger
//	  thread which keeps a log of execution.

#include <iostream>
#include <string>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <queue>

//header file for hw4 class
#ifndef HW4_H
#include "hw4.h"
#endif

using namespace std;

hw4 obj; //hw4 object
ifstream cf; 

//IPC struct
struct message
{
   char command;
   char param[80];
};

//queues used for thread communication
queue<message> cmdQueue;
queue<message> logQueue;

//mutexes
pthread_mutex_t cmdQueue_mu = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t hw4_mu = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t log_mu = PTHREAD_MUTEX_INITIALIZER;

//semaphores
sem_t cmdSem, logSem;

//prototypes
void PauseConsole();
int Setup();
void* reader_fn(void* x);
void* processor_fn(void* x);
void* log_fn(void* x);

int main(int argc, char** argv)
{   
   bool cmdFile = false;//use the command file
   int cmdCount = 0;	//semaphore counter for cmdQueue
   int logCount = 0;	//semaphore counter for logQueue
   pthread_t reader, processor, logger; //pthread structs
    
	sem_init(&cmdSem, 0, cmdCount);
	sem_init(&logSem, 0, logCount);
	
	//process command line arguments
	if(obj.processArgs(argc, argv) == -1)
	{
      	   cout << "Error. Exiting" << endl;
     	   exit(0);
	}
	
	//check for command file option specified
	if(strlen(obj.getFile().c_str()) > 0)
	{
		cmdFile = true;   
		//open command file.
		cf.open(obj.getFile().c_str());       
		if(!cf)
		{
			cout << "Error opening " << obj.getFile() << endl;
			exit(0);
		}
	}
		
	if(Setup() < 0) //create and change to master directory. Set the home path.
		exit(0);		
		
	//check for file successfully opened. Write user name if yes.
	if(!obj.openLog())
	{
		cout << "Failed to open log file " << obj.getLog() << endl;
		exit(0);
	}

	//create the threads to handle reading, processing, and logging
	pthread_create(&reader, NULL, reader_fn, (void*)&cmdFile);
	pthread_create(&processor, NULL, processor_fn, (void*)&cmdFile);
	pthread_create(&logger, NULL, log_fn, (void*)&cmdFile);
	pthread_join(reader, NULL);
	pthread_join(processor, NULL);
	pthread_join(logger, NULL);
     
	if(cmdFile) //if the command file was opened, close it
	   cf.close();
   return 0;  
}//end main

//used to pause the console between processing
void PauseConsole()
{
   cout << endl << "Press enter to continue...";
   cin.get();
   cout << endl;
}//PauseConsole()

int Setup()
{
   char path[256]; //for current working directory

   if(access("master", F_OK) == -1) //check for existence
   {
   	//make directory for obj file. Grant user all permissions. 
        if(mkdir("master", S_IRUSR | S_IWUSR | S_IXUSR) == -1)
		{
			cout << "Error creating master directory." << endl;
			return -1;   
		}
   }

   if(chdir("master") == -1) //change to master directory
   {
		cout << "Error changing to master directory." << endl;
		return -1;
   }
	
   //set the master directory as the home directory
   if(getcwd(path, sizeof(path) - 1) == NULL)
   {
		cout << "Error getting current directory." << endl;
		return -1;
   }
   
   obj.setHomeDir(path);
	
   return 0;
}//Setup()


//function for the reader thread. Performs all reading of commands and passes
//them to the processor thread through the shared queue cmdQueue
void* reader_fn(void* x)
{
	
	bool fileOption = *((bool*)x);	//unwrap the parameter
	message msg;			//used to enqueue messages into cmdQueue
	
	//determine if a file was selected for commands. Open it if so.
	istream& input = (fileOption) ? cf : cin;
		
	if(!fileOption)
		cout << "Enter commands at keyboard." << endl;
	while(msg.command != 'q')	
	{		   
		//get commands from selected input
		input >> msg.command;
		
		//get the parameter if command was c or d
		if(msg.command == 'c' || msg.command == 'd')
		{
			input >> msg.param;
		}
		else
			strcpy(msg.param, "");
			
		//lock the queue shared by reader and processor
		pthread_mutex_lock(&cmdQueue_mu);
		cmdQueue.push(msg);
		pthread_mutex_unlock(&cmdQueue_mu);

		sem_post(&cmdSem);
				
	}//while
	
	pthread_exit(0);	//kill the reader thread
}//reader_fn()


//Function for the processor thread. Gets commands from cmdQueue
//and uses hw4.cc functions to process and execute the commands. 
//Passes the commands on to the logQueue so that the logger thread can 
//write them to the log file.
void* processor_fn(void* x)
{
	message msg; //used to get message from queue
	bool cond = true; //loop condition
	
	//continually get messages from reader thread by way of the shared queue
	while(cond)
	{
		sem_wait(&cmdSem); //wait for the processor thread
		
		//lock access to the command queue
		pthread_mutex_lock(&cmdQueue_mu);
		msg = cmdQueue.front();			//get the message off the queue
		cmdQueue.pop();
		pthread_mutex_unlock(&cmdQueue_mu);
		
		pthread_mutex_lock(&hw4_mu);		//lock access to dir data object
		if(obj.processCmd(msg.command, msg.param) == -1)
			cout << "Invalid command." << endl;
		pthread_mutex_unlock(&hw4_mu);		//unlock access to dir data object
		
		//lock the logging queue
		pthread_mutex_lock(&log_mu);
		//push the message onto the logging queue
		logQueue.push(msg);
		pthread_mutex_unlock(&log_mu);
		
		sem_post(&logSem);	//inc the logging semaphore
		
		//if command q, quit
		if(msg.command == 'q')
			cond = false;
	}//while
		
	pthread_exit(0);	//kill the processor thread
}//processor_fn()

//Function for the logger thread. Gets commands from the logQueue and 
//records the commands in the 
void* log_fn(void* x)
{
	stringstream ss; //used to pacakge the command and parameter back into one string
	message msg;	 //used to get message off the logQueue
	bool cond = true;
	
	pthread_mutex_lock(&hw4_mu);		//lock access to dir data object
	obj.writeLnToLog("user: " + obj.getUser());    //write user name to log file
	pthread_mutex_unlock(&hw4_mu);		//unlock access to dir data object

	//gets commands off the logQueue
	while(cond)
	{
		sem_wait(&logSem);	//wait for the count to be positive
		
		//lock the logging queue
		pthread_mutex_lock(&log_mu);		
		//get the message from the front of the queue
		msg = logQueue.front();
		logQueue.pop();
		pthread_mutex_unlock(&log_mu); //unlock the logQueue
		
		ss.str(""); //clear the stringstream

		//put command and parameter into string stream
		ss << msg.command << ' ' << msg.param;
		
		pthread_mutex_lock(&hw4_mu);	//lock access to dir data object
		obj.writeLnToLog(ss.str());	//convert streamstring to string and write it	
		pthread_mutex_unlock(&hw4_mu);	//unlock access to dir data object
		
		if(msg.command == 'q') //check for q command
			cond = false;
	}//while
		
	pthread_exit(0);	//kill the logger thread
}//log_fn
