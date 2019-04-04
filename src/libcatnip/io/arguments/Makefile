CC=g++
CFLAGS= -Wall -Wextra -pedantic -std=c++11

all: test_argumentfile test_argumentswitch test_argumentint test_argumentdouble test_argumentstring

test_argumentswitch: test_argumentswitch.cpp string_support.o argumentswitch.o propertyswitch.o 
	$(CC) $(CFLAGS) test_argumentswitch.cpp string_support.o argumentswitch.o propertyswitch.o -o test_argumentswitch 

argumentswitch.o : argumentswitch.cpp
	$(CC) $(CFLAGS) -c argumentswitch.cpp

test_argumentstring: test_argumentstring.cpp argumentstring.o string_support.o propertystring.o propertystringchoice.o
	$(CC) $(CFLAGS) test_argumentstring.cpp argumentstring.o propertystring.o string_support.o propertystringchoice.o -o test_argumentstring 

argumentstring.o : argumentstring.cpp
	$(CC) $(CFLAGS) -c argumentstring.cpp

propertystring.o : PROPERTIES/propertystring.cpp
	$(CC) $(CFLAGS) -c PROPERTIES/propertystring.cpp

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

test_argumentfile: test_argumentfile.cpp propertysisterfile.o argumentfile.o string_support.o propertyfileexist.o propertyfileext.o
	$(CC) $(CFLAGS) test_argumentfile.cpp argumentfile.o propertysisterfile.o propertyfileexist.o propertyfileext.o string_support.o -o test_argumentfile

argumentfile.o : argumentfile.cpp
	$(CC) $(CFLAGS) -c argumentfile.cpp

propertyswitch.o : PROPERTIES/propertyswitch.cpp
	$(CC) $(CFLAGS) -c PROPERTIES/propertyswitch.cpp

propertysisterfile.o : PROPERTIES/propertysisterfile.cpp
	$(CC) $(CFLAGS) -c PROPERTIES/propertysisterfile.cpp

propertyint.o : PROPERTIES/propertyint.cpp
	$(CC) $(CFLAGS) -c PROPERTIES/propertyint.cpp

propertyfileexist.o : PROPERTIES/propertyfileexist.cpp
	$(CC) $(CFLAGS) -c PROPERTIES/propertyfileexist.cpp

propertystringchoice.o : PROPERTIES/propertystringchoice.cpp
	$(CC) $(CFLAGS) -c PROPERTIES/propertystringchoice.cpp

propertyfileext.o : PROPERTIES/propertyfileext.cpp
	$(CC) $(CFLAGS) -c PROPERTIES/propertyfileext.cpp

string_support.o: ../../STRING_SUPPORT/string_support.cpp
	$(CC) $(CFLAGS) -c ../../STRING_SUPPORT/string_support.cpp

clean:
	rm *.o test_argumentfile test_argumentswitch test_argumentint test_argumentdouble test_argumentstring
