hw1main: hw1main.o hw1.o
	g++ hw1main.o hw1.o -o hw1
hw1main.o: hw1main.cc hw1.h
	g++ -c -g hw1main.cc
hw1.o: hw1.cc hw1.h
	g++ -c -g hw1.cc
clean: 
	rm -f *.o
	rm -f hw1main
