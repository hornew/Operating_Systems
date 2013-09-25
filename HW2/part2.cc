//
//Author: William (Austin) Horne
//Date:   2/19/13
//Course: CSCI 4727
//Purpose:Experience with fork() and other system calls

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <vector> 
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
   int rCode; //return code from fork()
   int pid;   //child process id
   int ppid;  //parent process id
   int status;//termination status of child
   string item;
   string s = "../../path/name";
   stringstream ss(s);
   vector<string> elems;
   char delim = '/';
   

  /* //call to fork
   rCode = fork();
   
   if(rCode == 0) //child process
   {
	pid = getpid();
	ppid = getppid();
	cout << "Child process ID: " << pid << endl;
	cout << "Parent process ID: " << ppid << endl;
	execlp("hw2", "hw2", "-u", "Austin", "-l", "MyLog.txt", "-f", "test.txt", 0);
	exit(0);	
   }
   else if(rCode == -1) //error occurred
   {
	cout << "Error. Fork Unsuccessful.";
   }
   else //parent process
   {
	wait(&status);	
   }
*/
   while(getline(ss, item, delim))
   {
	elems.push_back(item);
   }

   for(int i = 0; i < elems.size(); i++)
   {
	cout << elems[i] << endl;
   }

}//main
