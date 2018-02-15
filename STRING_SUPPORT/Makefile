CC = g++
CFLAGS = -Wall -Wextra -std=c++11

all: test_string_support

test_string_support: test_string_support.cpp string_support.o 
	$(CC) $(CFLAGS) test_string_support.cpp string_support.o -o test_string_support

string_support.o: string_support.cpp 
	$(CC) $(CFLAGS) -c string_support.cpp	

clean:
	rm *.o test_string_support test_string_support.exe
