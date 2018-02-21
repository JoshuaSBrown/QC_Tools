
CC = g++
CFLAGS = -Wall -Wextra -std=c++11

all: calc_J

calc_J: main.cpp io.o matrix.o qc_functions.o string_support.o parameters.o
	$(CC) $(CFLAGS) main.cpp io.o matrix.o qc_functions.o string_support.o parameters.o -o calc_J

io.o: IO/io.cpp string_support.o
	$(CC) $(CFLAGS) -c IO/io.cpp string_support.o

string_support.o: STRING_SUPPORT/string_support.cpp 
	$(CC) $(CFLAGS) -c STRING_SUPPORT/string_support.cpp

parameters.o: PARAMETERS/parameters.cpp 
	$(CC) $(CFLAGS) -c PARAMETERS/parameters.cpp

matrix.o: MATRIX/matrix.cpp 
	$(CC) $(CFLAGS) -c MATRIX/matrix.cpp

qc_functions.o: QC_FUNCTIONS/qc_functions.cpp
	$(CC) $(CFLAGS) -c QC_FUNCTIONS/qc_functions.cpp

clean:
	rm *.o
