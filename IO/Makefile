
all: test_io

test_io: test_io.cpp io.o 
	g++ -Wall -std=c++11 test_io.cpp io.o matrix.o -o test_io

io.o: io.cpp
	g++ -Wall -std=c++11 matrix.o -c io.cpp

matrix.o: ../MATRIX/matrix.cpp
	g++ -Wall -std=c++11 -c ../MATRIX/matrix.cpp

clean:
	rm *.o
