CC=g++
CFLAGS= -Wall -Wextra -std=c++11

all: test_matrix

test_matrix: test_matrix.cpp matrix.o matrix.hpp
	$(CC) $(CFLAGS) test_matrix.cpp matrix.o -o test_matrix

matrix.o: matrix.cpp matrix.hpp
	$(CC) $(CFLAGS) -c matrix.cpp	

clean:
	rm *.o test_matrix test_matrix.exe
