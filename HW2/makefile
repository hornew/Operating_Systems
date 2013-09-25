hw2main: hw2main.o hw2.o
	g++ hw2main.o hw2.o -o hw2
hw2main.o: hw2main.cc hw2.h
	g++ -c -g hw2main.cc
hw2.o: hw2.cc hw2.h
	g++ -c -g hw2.cc
part2:   part2.o
	g++ part2.o -o part2
part2.o: part2.cc
	g++ -c -g part2.cc 
clean: 
	rm -f *.o
	rm -f hw2
	rm -r master
