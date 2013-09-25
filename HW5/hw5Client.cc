//
//Author: William (Austin) Horne, Samantha Bledsoe
//Date:   5/3/13
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
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>

//header file for hw5 class
#ifndef HW5_H
#include "hw5.h"
#endif

using namespace std;

hw5 obj; //hw5 object
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
pthread_mutex_t hw5_mu = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t log_mu = PTHREAD_MUTEX_INITIALIZER;

//semaphores
sem_t cmdSem, logSem;

//prototypes
void PauseConsole();
int Setup();
void* reader_fn(void* x);
void* processor_fn(void* x);
void serverLog(message log, int sockdesc);
void sendString(string log, int sockdesc);

int main(int argc, char** argv)
{   
   bool cmdFile = false;//use the command file
   int cmdCount = 0;	//semaphore counter for cmdQueue
   int logCount = 0;	//semaphore counter for logQueue
   pthread_t reader, processor, logger; //pthread structs
   struct addrinfo* myinfo;
   int sockdesc;
   char hostname[81];
   char portnum[81];   
   int connection;
   int value;
    
	sem_init(&cmdSem, 0, cmdCount);
	sem_init(&logSem, 0, logCount);
	
	//process command line arguments
	if(obj.processArgs(argc, argv) == -1)
	{
      	cout << "Required command line arguments not specified. Exiting" << endl;
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
			cout << "Error opening command file" << obj.getFile() << endl;
			exit(0);
		}
	}
		
	if(Setup() < 0) //create and change to master directory. Set the home path.
		exit(0);		
	
	//server address from command line
	strcpy(hostname, obj.getServer().c_str());
	//port from command line
	strcpy(portnum, obj.getPort().c_str());

   // Use AF_UNIX for unix pathnames instead
   // Use SOCK_DGRAM for UDP datagrams
   sockdesc = socket(AF_INET, SOCK_STREAM, 0);
   if ( sockdesc < 0 )
   {
      cout << "Error creating socket" << endl;
      exit(0);
   }

   if ( getaddrinfo( hostname, portnum, NULL, &myinfo) != 0 )
   {
      cout << "Error getting address" << endl;
      exit(0);
   }

	// Attempt to connect to the host
	connection = connect(sockdesc, myinfo->ai_addr, myinfo->ai_addrlen);
	if ( connection < 0 )
	{
      cout << "Error in connect" << endl;
      exit(0);
	}
	
	//create the threads to handle reading, processing, and logging
	pthread_create(&reader, NULL, reader_fn, (void*)&cmdFile);
	pthread_create(&processor, NULL, processor_fn, (void*)&sockdesc);
	pthread_join(reader, NULL);
	pthread_join(processor, NULL);
     
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
//and uses hw5.cc functions to process and execute the commands. 
//Passes the commands on to the logQueue so that the logger thread can 
//write them to the log file.
void* processor_fn(void* x)
{
	int sockdesc  = *((int*)x);
	message msg; //used to get message from queue
	bool cond = true; //loop condition
	
	//write the user name to the log file
	pthread_mutex_lock(&hw5_mu);		//lock access to dir data object
	sendString(obj.getLog(), sockdesc);
	sendString(obj.getUser(), sockdesc);
	pthread_mutex_unlock(&hw5_mu);		//unlock access to dir data object
	
	//continually get messages from reader thread by way of the shared queue
	while(cond)
	{
		sem_wait(&cmdSem); //wait for the processor thread
		
		//lock access to the command queue
		pthread_mutex_lock(&cmdQueue_mu);
		msg = cmdQueue.front();			//get the message off the queue
		cmdQueue.pop();
		pthread_mutex_unlock(&cmdQueue_mu);
		
		pthread_mutex_lock(&hw5_mu);		//lock access to dir data object
		if(obj.processCmd(msg.command, msg.param) == -1)
			cout << "Invalid command." << endl;
		pthread_mutex_unlock(&hw5_mu);		//unlock access to dir data object
		
		serverLog(msg, sockdesc);	//write command and param to socket
		
		//if command q, quit
		if(msg.command == 'q')
			cond = false;
	}//while
		
	pthread_exit(0);	//kill the processor thread
}//processor_fn()

//Handles packaging and transmission of message to server
void serverLog(message log, int sockdesc)
{
	stringstream ss;	//formatting
	string logBuffer;	//I like strings better than C strings :)
		
	ss << log.command << ' ' << log.param << endl;	//package stuff
	
	//stringstream and string libraries do work of converting to c++ string
	//then to c string
	logBuffer = ss.str();
	send(sockdesc, logBuffer.c_str(), 80, 0);
}//serverLog()


//accepts a string and sends it to the server. Used to send log file and user names
void sendString(string msg, int sockdesc)
{
	send(sockdesc, msg.c_str(), 80, 0);
}//sendLogName()