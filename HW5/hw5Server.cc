//
//Author: 	Samantha Bledsoe, William (Austin) Horne
//Date:   	5/3/13
//Course: 	CSCI 4727
//Purpose: 	Setup a server to handle logging duties.  
//			Referrence linuxhowtos.org/C_C++/socket.htm for explanation of socket
//			programming in UNIX/Linux


#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <sstream>
#include <fstream>

using namespace std;

// Function prototypes

// Thread function to handle one request
void* handleRequest( void* arg );
//processes command line argument to get the port number
int getPort(int argc, char** argv);

//could have declared this locally and passed as a parameter, but lazy is good.
ofstream logFile;
string portnum;
int main(int argc, char** argv)
{  	
	int sockdesc;	
	int connection;		
	struct addrinfo* myinfo;
	pthread_t requestThread;      
	
	// Use AF_UNIX for unix pathnames instead
    // Use SOCK_DGRAM for UDP datagrams
	sockdesc = socket(AF_INET, SOCK_STREAM, 0);	
	if ( sockdesc < 0 ) //error getting socket
	{
		cout << "Error getting socket. Exiting." << endl;
		exit(0);
	}	
	
	if(getPort(argc, argv) == -1)
	{
		cout << "Port number not specified at command line. Exiting." << endl;
		exit(0);
	}
	
	// Set up the address record
	if ( getaddrinfo( "0.0.0.0", portnum.c_str(), NULL, &myinfo ) != 0 )
	{
      cout << "Error getting address" << endl;
	  exit(0);
	}
	
	// Bind the socket to an address
	if (bind(sockdesc, myinfo->ai_addr, myinfo->ai_addrlen) < 0 )
	{
      cout << "Error binding to socket" << endl;
      exit(0);
	}

   // Display the port number that was successful
   cout << "Bind successful, using portnum = " << portnum << endl;

   // Now listen to the socket
   if ( listen(sockdesc, 1) < 0 )
   {
      cout << "Error in listen" << endl;
      exit(0);
   }

   // Forever loop - kill manually
   // Accept a connect request and use connection, *NOT* sockdesc
   // (tcp handoff)
   for(;;)
   {
      cout << "Calling accept" << endl;
	  //accept blocks until a client connects to the socket
      connection = accept(sockdesc, NULL, NULL);
	  
	  //couldn't accept connection from this client for some reason.
	  //Continue to next request
      if ( connection < 0 )
      {
         cout << "Error in accept" << endl;
		 close(connection);
      }
      else	//got the connection, now handle the request.
      {
         pthread_create(&requestThread, NULL, handleRequest, (void *)&connection);
	     cout << "After create" << endl;
      }
   } // for

  return 0;
}

// Thread function - handles one client connection
void* handleRequest( void* arg )
{
   int connection;
   int value;
   char buffer[81], str[81];
   void* dummyPointer;		//lets me return from this.

   pthread_detach( pthread_self() );
   connection = *((int*)arg);
   
   // Show the actual file descriptor used
   cout << "Server thread, connection = " << connection << endl;
   
   // Receive a message   
   value = read(connection, buffer, 80);
   if ( value < 0 )
   {
      cout << "Error on receive" << endl;
      // Close the socket
	  close(connection);
	  pthread_exit(0);
	  return dummyPointer;
   }
   else if ( value == 0 )
   {
      cout << "End of transmission" << endl;
      // Close the socket
      close(connection);
	  pthread_exit(0);
	  return dummyPointer;
   }
   
   strcpy(str, buffer);
   cout << "Log name received: " << str << endl;   
   logFile.open(str, ofstream::out | ofstream::app);
   
   //get the user name from the client
   value = read(connection, buffer, 80);
   if ( value < 0 )
   {
      cout << "Error on receive" << endl;      
	  close(connection);	// Close the socket
	  pthread_exit(0);
	  return dummyPointer;
   }
   else if ( value == 0 )
   {
      cout << "End of transmission" << endl;      
      close(connection);	// Close the socket
	  pthread_exit(0);
	  return dummyPointer;
   }
   logFile << "Connection from user name: " << buffer << "\n" << endl;
   cout << "User name '" << buffer << "' connected." << endl;
   
   // Continue until the message is "q"
   while ( strcmp(buffer, "q") != 0 ) 
   {
      // Get the next message      
	  value = read(connection, buffer, 80);
      if ( value < 0 )
      {
         cout << "Error on receive. Closing connection." << endl;
		 close(connection);
		 pthread_exit(0);
		 return dummyPointer;
      }
      else if ( value == 0 )
      {
         cout << "End of transmission" << endl;
         break;
      }
	  
      cout << "Message received: " << buffer << endl;
	  logFile << buffer << endl;
   } // while
   
   logFile.close();
   
   // Close the socket
   close(connection);
   pthread_exit(0);
   
}//handleRequest()

//process the command line arguments to get the port number
int getPort(int argc, char** argv)
{
	char opt;
	
	if(argc == 1)
		return -1;
		
	while( ( opt = getopt(argc, argv, "p:")) != -1)
	{	if(opt == 'p')
		{
			portnum = optarg;
			return 0;
		}
	}
	return -1;
}
