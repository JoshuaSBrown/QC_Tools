
all: test_io

test_io: test_io.cpp io.o matrix.o string_support.o parameters.o 
	g++ -Wall -std=c++11 test_io.cpp io.o matrix.o string_support.o parameters.o -o test_io

io.o: io.cpp matrix.o string_support.o
	g++ -Wall -std=c++11 matrix.o string_support.o -c io.cpp

matrix.o: ../MATRIX/matrix.cpp
	g++ -Wall -std=c++11 -c ../MATRIX/matrix.cpp

parameters.o: ../PARAMETERS/parameters.cpp
	g++ -Wall -std=c++11 -c ../PARAMETERS/parameters.cpp

string_support.o: ../STRING_SUPPORT/string_support.cpp
	g++ -Wall -std=c++11 -c ../STRING_SUPPORT/string_support.cpp

clean:
	rm *.o
