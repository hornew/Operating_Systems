hw5Server: hw5Server.o
	g++ hw5Server.o -lpthread -o hw5Server
hw5Server.o: hw5Server.cc
	g++ -c hw5Server.cc

hw5Client: hw5Client.o hw5.o
	g++ hw5Client.o hw5.o -lpthread -o hw5Client
hw5Client.o: hw5Client.cc hw5.h
	g++ -c -g hw5Client.cc
hw5.o: hw5.cc hw5.h
	g++ -c -g hw5.cc
clean: 
	rm -f *.o
	rm -f hw5Client 
	rm -f hw5Server
	rm -r master
	rm -f log.txt
