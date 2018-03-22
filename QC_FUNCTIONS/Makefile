CC=g++
CFLAGS= -Wall -Wextra -std=c++11

all: test_qc_functions

test_qc_functions : test_qc_functions.cpp qc_functions.o matrix.o 
	$(CC) $(CFLAGS) test_qc_functions.cpp matrix.o qc_functions.o -o test_qc_functions

matrix.o: ../MATRIX/matrix.cpp 
	$(CC) $(CFLAGS) -c ../MATRIX/matrix.cpp	

qc_functions.o: qc_functions.cpp 
	$(CC) $(CFLAGS) -c qc_functions.cpp	

clean:
	rm *.o test_qc_functions test_qc_functions.exe
