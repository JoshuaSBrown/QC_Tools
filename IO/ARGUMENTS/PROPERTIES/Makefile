CC=g++
CFLAGS= -Wall -Wextra -pedantic -std=c++11 -O2

all: test_propertyint test_propertydouble test_propertystring test_propertyfileext test_propertyfileexist test_propertysisterfile test_propertystringchoice

test_propertystringchoice: test_propertystringchoice.cpp string_support.o propertystringchoice.o
	$(CC) $(CFLAGS) test_propertystringchoice.cpp propertystringchoice.o string_support.o -o test_propertystringchoice

propertystringchoice.o : propertystringchoice.cpp
	$(CC) $(CFLAGS) -c propertystringchoice.cpp

test_propertyfileexist: test_propertyfileexist.cpp string_support.o propertyfileexist.o
	$(CC) $(CFLAGS) test_propertyfileexist.cpp propertyfileexist.o string_support.o -o test_propertyfileexist

propertyfileexist.o : propertyfileexist.cpp
	$(CC) $(CFLAGS) -c propertyfileexist.cpp

test_propertyfileext: test_propertyfileext.cpp string_support.o propertyfileext.o
	$(CC) $(CFLAGS) test_propertyfileext.cpp propertyfileext.o string_support.o -o test_propertyfileext

test_propertysisterfile: test_propertysisterfile.cpp string_support.o propertysisterfile.o
	$(CC) $(CFLAGS) test_propertysisterfile.cpp propertysisterfile.o string_support.o -o test_propertysisterfile

propertysisterfile.o : propertysisterfile.cpp
	$(CC) $(CFLAGS) -c propertysisterfile.cpp

propertyfileext.o : propertyfileext.cpp
	$(CC) $(CFLAGS) -c propertyfileext.cpp

test_propertystring: test_propertystring.cpp string_support.o propertystring.o
	$(CC) $(CFLAGS) test_propertystring.cpp propertystring.o string_support.o -o test_propertystring

propertystring.o : propertystring.cpp
	$(CC) $(CFLAGS) -c propertystring.cpp

test_propertydouble: test_propertydouble.cpp string_support.o propertydouble.o
	$(CC) $(CFLAGS) test_propertydouble.cpp propertydouble.o string_support.o -o test_propertydouble

propertydouble.o : propertydouble.cpp
	$(CC) $(CFLAGS) -c propertydouble.cpp

test_propertyint: test_propertyint.cpp string_support.o propertyint.o
	$(CC) $(CFLAGS) test_propertyint.cpp propertyint.o string_support.o -o test_propertyint

propertyint.o : propertyint.cpp
	$(CC) $(CFLAGS) -c propertyint.cpp

string_support.o: ../../../STRING_SUPPORT/string_support.cpp
	$(CC) $(CFLAGS) -c ../../../STRING_SUPPORT/string_support.cpp

clean:
	rm *.o test_propertyint test_propertydouble test_propertystring test_propertyfileext test_propertyfileexist test_propertysisterfile
