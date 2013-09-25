Operating_Systems
=================

Collection of Projects from Operating Systems course at East Tennessee State University

Assignment HW1

  Notes: 
    -Checking for bad input not required as it is not the purpose of the assignment.
    -Command line options other than the -u option should be ignored for this assignment.
    -Use of the system() command not allowed

  Requirements:
  
    1.Produce an executable which will:
        -Accept -u, -f, and -l as command line options, each paired with a parameter.
        -Create a directory called "master" and change to that directory.
        -Create a log file in this which will contain the user name (-u parameter) as its only entry.
        -Read in and echo to the screen single-letter commands typed at the keyboard. Echo but ignore everything but 'q'.
        -Exit when 'q' is entered.
  
    2.Write a makefile to compile the project using g++

  
Assignment HW2

  Purpose: Build upon Assignment HW1

  Notes:
    -Modularize the program.

  Requirements:
  
    1.Produce an executable which will:
      -Check the return value from system calls.  Display an error message to the screen when an error occurs.
      -Implement the -f option from assignment HW1.  When present, the associated file should be opened and command
       should be read from it instead of reading them from the keyboard.
      -Implement simple logging: after writing the user name in the log file, write each command that is read in.

    2.Write a bash shell script name part1 that will:
      -Check for the existence of a makefile; if it's not there, display an error message and quit, else run the makefile.
      -Check for the existence of an executeable called "hw2".  If there, run it; if not, display an error message and quit.
      
    3.Write a short program name part2 that makes use of the fork() function to create a child process.
      -The child should print its pid and its parent pid.
      -The child should also use the execlp function to run the hw2 program.
