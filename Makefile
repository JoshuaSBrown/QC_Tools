
all: run

run: main.cpp io.o matrix.o
	g++ -Wall main.cpp io.o matrix.o -o run

io.o: IO/io.cpp
	g++ -Wall -c IO/io.cpp

matrix.o: MATRIX/matrix.cpp 
	g++ -Wall -c MATRIX/matrix.cpp


clean:
	rm *.o
