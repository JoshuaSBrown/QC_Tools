CC = g++
CFLAGS = -Wall -Wextra -std=c++11

all: test_parameters

test_parameters: test_parameters.cpp string_support.o parameters.o 
	$(CC) $(CFLAGS) test_parameters.cpp string_support.o parameters.o -o test_parameters

string_support.o: ../STRING_SUPPORT/string_support.cpp 
	$(CC) $(CFLAGS) -c ../STRING_SUPPORT/string_support.cpp	

parameters.o: parameters.cpp 
	$(CC) $(CFLAGS) -c parameters.cpp	

clean:
	rm *.o test_parameters test_parameters.exe
