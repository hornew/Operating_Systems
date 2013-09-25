hw4main: hw4main.o hw4.o
	g++ hw4main.o hw4.o -lpthread -o hw4
hw4main.o: hw4main.cc hw4.h
	g++ -c -g hw4main.cc
hw4.o: hw4.cc hw4.h
	g++ -c -g hw4.cc
clean: 
	rm -f *.o
	rm -f hw4
	rm -r master
