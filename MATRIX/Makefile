
all: test

test: test.cpp matrix.o matrix.hpp
	g++ -Wall test.cpp matrix.o -o test

matrix.o: matrix.cpp matrix.hpp
	g++ -Wall -c matrix.cpp	

clean:
	rm *.o test test.exe
