
CC = g++
CFLAGS = -Wall -Wextra -std=c++11

all: calc_J

calc_J: main.cpp io.o matrix.o punreader.o logreader.o filereader.o qc_functions.o string_support.o parameters.o
	$(CC) $(CFLAGS) main.cpp io.o matrix.o punreader.o logreader.o filereader.o qc_functions.o string_support.o parameters.o -o calc_J

io.o: IO/io.cpp string_support.o
	$(CC) $(CFLAGS) -c IO/io.cpp string_support.o

logreader.o: IO/logreader.cpp filereader.o matrix.o string_support.o
	$(CC) $(CFLAGS) matrix.o string_support.o filereader.o -c IO/logreader.cpp

punreader.o: IO/punreader.cpp filereader.o matrix.o string_support.o
	$(CC) $(CFLAGS) matrix.o string_support.o filereader.o -c IO/punreader.cpp

filereader.o: IO/filereader.cpp matrix.o string_support.o
	$(CC) $(CFLAGS) matrix.o string_support.o -c IO/filereader.cpp

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
