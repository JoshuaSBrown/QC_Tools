
all: run

run: main.cpp io.o matrix.o
	g++ -Wall -std=c++11 main.cpp io.o matrix.o -o run

io.o: IO/io.cpp
	g++ -Wall -std=c++11 -c IO/io.cpp

matrix.o: MATRIX/matrix.cpp 
	g++ -Wall -std=c++11 -c MATRIX/matrix.cpp


clean:
	rm *.o
