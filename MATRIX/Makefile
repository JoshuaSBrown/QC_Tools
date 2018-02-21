
all: test_matrix

test_matrix: test_matrix.cpp matrix.o matrix.hpp
	g++ -Wall test_matrix.cpp matrix.o -o test_matrix

matrix.o: matrix.cpp matrix.hpp
	g++ -Wall -c matrix.cpp	

clean:
	rm *.o test_matrix test_matrix.exe
