CC=g++
CFLAGS=-Wall -Wextra -std=c++11 -pedantic
objects= argumentparser.o propertystringchoice.o matrix.o string_support.o parameters.o argumentstring.o argumentdouble.o argumentint.o argumentfile.o argumentswitch.o propertystring.o propertydouble.o propertyint.o propertyfileext.o propertysisterfile.o propertyfileexist.o propertyswitch.o 


all: test_argumentparser test_io

test_io: $(objects) io.o test_io.cpp
	$(CC) $(CFLAGS) test_io.cpp $(objects) io.o -o test_io

test_argumentparser: test_argumentparser.cpp $(objects) 
	$(CC) $(CFLAGS) test_argumentparser.cpp $(objects) -o test_argumentparser

io.o : io.cpp
	$(CC) $(CFLAGS) -c io.cpp

argumentparser.o : argumentparser.cpp
	$(CC) $(CFLAGS) -c argumentparser.cpp

argumentfile.o : ARGUMENTS/argumentfile.cpp
	$(CC) $(CFLAGS) -c ARGUMENTS/argumentfile.cpp

argumentswitch.o : ARGUMENTS/argumentswitch.cpp
	$(CC) $(CFLAGS) -c ARGUMENTS/argumentswitch.cpp

argumentstring.o : ARGUMENTS/argumentstring.cpp
	$(CC) $(CFLAGS) -c ARGUMENTS/argumentstring.cpp

argumentdouble.o : ARGUMENTS/argumentdouble.cpp
	$(CC) $(CFLAGS) -c ARGUMENTS/argumentdouble.cpp

argumentint.o : ARGUMENTS/argumentint.cpp
	$(CC) $(CFLAGS) -c ARGUMENTS/argumentint.cpp

propertystringchoice.o : ARGUMENTS/PROPERTIES/propertystringchoice.cpp
	$(CC) $(CFLAGS) -c ARGUMENTS/PROPERTIES/propertystringchoice.cpp

propertysisterfile.o : ARGUMENTS/PROPERTIES/propertysisterfile.cpp
	$(CC) $(CFLAGS) -c ARGUMENTS/PROPERTIES/propertysisterfile.cpp

propertyfileext.o : ARGUMENTS/PROPERTIES/propertyfileext.cpp
	$(CC) $(CFLAGS) -c ARGUMENTS/PROPERTIES/propertyfileext.cpp

propertyfileexist.o : ARGUMENTS/PROPERTIES/propertyfileexist.cpp
	$(CC) $(CFLAGS) -c ARGUMENTS/PROPERTIES/propertyfileexist.cpp

propertyswitch.o : ARGUMENTS/PROPERTIES/propertyswitch.cpp
	$(CC) $(CFLAGS) -c ARGUMENTS/PROPERTIES/propertyswitch.cpp

propertystring.o : ARGUMENTS/PROPERTIES/propertystring.cpp
	$(CC) $(CFLAGS) -c ARGUMENTS/PROPERTIES/propertystring.cpp

propertydouble.o : ARGUMENTS/PROPERTIES/propertydouble.cpp
	$(CC) $(CFLAGS) -c ARGUMENTS/PROPERTIES/propertydouble.cpp

propertyint.o : ARGUMENTS/PROPERTIES/propertyint.cpp
	$(CC) $(CFLAGS) -c ARGUMENTS/PROPERTIES/propertyint.cpp

matrix.o: ../MATRIX/matrix.cpp
	$(CC) $(CFLAGS) -c ../MATRIX/matrix.cpp

parameters.o: ../PARAMETERS/parameters.cpp
	$(CC) $(CFLAGS) -c ../PARAMETERS/parameters.cpp

string_support.o: ../STRING_SUPPORT/string_support.cpp
	$(CC) $(CFLAGS) -c ../STRING_SUPPORT/string_support.cpp

clean:
	rm *.o test_argumentparser test_io
