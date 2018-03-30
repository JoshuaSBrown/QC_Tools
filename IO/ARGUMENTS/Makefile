CC=g++
CFLAGS= -Wall -Wextra -pedantic -std=c++11

all: test_argumentfile test_argumentint test_argumentdouble

test_argumentdouble: test_argumentdouble.cpp argumentdouble.o string_support.o propertydouble.o
	$(CC) $(CFLAGS) test_argumentdouble.cpp argumentdouble.o propertydouble.o string_support.o -o test_argumentdouble

argumentdouble.o : argumentdouble.cpp
	$(CC) $(CFLAGS) -c argumentdouble.cpp

propertydouble.o : PROPERTIES/propertydouble.cpp
	$(CC) $(CFLAGS) -c PROPERTIES/propertydouble.cpp

test_argumentint: test_argumentint.cpp argumentint.o string_support.o propertyint.o
	$(CC) $(CFLAGS) test_argumentint.cpp argumentint.o propertyint.o string_support.o -o test_argumentint

argumentint.o : argumentint.cpp
	$(CC) $(CFLAGS) -c argumentint.cpp

test_argumentfile: test_argumentfile.cpp argumentfile.o string_support.o propertyfileexist.o propertyfileext.o
	$(CC) $(CFLAGS) test_argumentfile.cpp argumentfile.o propertyfileexist.o propertyfileext.o string_support.o -o test_argumentfile

argumentfile.o : argumentfile.cpp
	$(CC) $(CFLAGS) -c argumentfile.cpp

propertyint.o : PROPERTIES/propertyint.cpp
	$(CC) $(CFLAGS) -c PROPERTIES/propertyint.cpp

propertyfileexist.o : PROPERTIES/propertyfileexist.cpp
	$(CC) $(CFLAGS) -c PROPERTIES/propertyfileexist.cpp

propertyfileext.o : PROPERTIES/propertyfileext.cpp
	$(CC) $(CFLAGS) -c PROPERTIES/propertyfileext.cpp

string_support.o: ../../STRING_SUPPORT/string_support.cpp
	$(CC) $(CFLAGS) -c ../../STRING_SUPPORT/string_support.cpp

clean:
	rm *.o test_argumentfile test_argumentint
