
all: test_argumentint test_argumentdouble test_argumentstring test_argumentfileext test_argumentfileexist test_argumentparser 
#test_io test_logreader test_punreader 
#test_argumentfile

test_argumentparser: test_argumentparser.cpp argumentparser.o matrix.o string_support.o parameters.o argumentstring.o argumentdouble.o argumentint.o argumentfileext.o argumentfileexist.o
	g++ -Wall -std=c++11 test_argumentparser.cpp argumentparser.o matrix.o string_support.o parameters.o argumentstring.o argumentdouble.o argumentint.o argumentfileext.o argumentfileexist.o -o test_argumentparser

#test_io: test_io.cpp io.o matrix.o string_support.o parameters.o 
#	g++ -Wall -std=c++11 test_io.cpp io.o matrix.o string_support.o parameters.o -o test_io

#test_logreader: test_logreader.cpp filereader.o logreader.o matrix.o string_support.o parameters.o 
#	g++ -Wall -std=c++11 test_logreader.cpp filereader.o logreader.o matrix.o string_support.o parameters.o -o test_logreader

#test_punreader: test_punreader.cpp filereader.o punreader.o matrix.o string_support.o parameters.o 
#	g++ -Wall -std=c++11 test_punreader.cpp filereader.o punreader.o matrix.o string_support.o parameters.o -o test_punreader

#test_argumentfile: test_argumentfile.cpp string_support.o 
#	g++ -Wall -std=c++11 test_argumentfile.cpp string_support.o -o test_argumentfile

test_argumentfileexist: test_argumentfileexist.cpp string_support.o argumentfileexist.o
	g++ -Wall -std=c++11 test_argumentfileexist.cpp argumentfileexist.o string_support.o -o test_argumentfileexist

argumentfileexist.o : argumentfileexist.cpp
	g++ -Wall -std=c++11 -c argumentfileexist.cpp

test_argumentfileext: test_argumentfileext.cpp string_support.o argumentfileext.o
	g++ -Wall -std=c++11 test_argumentfileext.cpp argumentfileext.o string_support.o -o test_argumentfileext

argumentfileext.o : argumentfileext.cpp
	g++ -Wall -std=c++11 -c argumentfileext.cpp

argumentparser.o : argumentparser.cpp
	g++ -Wall -std=c++11 -c argumentparser.cpp

test_argumentstring: test_argumentstring.cpp string_support.o argumentstring.o
	g++ -Wall -std=c++11 test_argumentstring.cpp argumentstring.o string_support.o -o test_argumentstring

argumentstring.o : argumentstring.cpp
	g++ -Wall -std=c++11 -c argumentstring.cpp

test_argumentdouble: test_argumentdouble.cpp string_support.o argumentdouble.o
	g++ -Wall -std=c++11 test_argumentdouble.cpp argumentdouble.o string_support.o -o test_argumentdouble

argumentdouble.o : argumentdouble.cpp
	g++ -Wall -std=c++11 -c argumentdouble.cpp

test_argumentint: test_argumentint.cpp string_support.o argumentint.o
	g++ -Wall -std=c++11 test_argumentint.cpp argumentint.o string_support.o -o test_argumentint

argumentint.o : argumentint.cpp
	g++ -Wall -std=c++11 -c argumentint.cpp

#logreader.o: logreader.cpp filereader.o matrix.o string_support.o
#	g++ -Wall -std=c++11 matrix.o string_support.o filereader.o -c logreader.cpp

#punreader.o: punreader.cpp filereader.o matrix.o string_support.o
#	g++ -Wall -std=c++11 matrix.o string_support.o filereader.o -c punreader.cpp

#filereader.o: filereader.cpp matrix.o string_support.o
#	g++ -Wall -std=c++11 matrix.o string_support.o -c filereader.cpp

io.o: io.cpp matrix.o string_support.o
	g++ -Wall -std=c++11 matrix.o string_support.o -c io.cpp

matrix.o: ../MATRIX/matrix.cpp
	g++ -Wall -std=c++11 -c ../MATRIX/matrix.cpp

parameters.o: ../PARAMETERS/parameters.cpp
	g++ -Wall -std=c++11 -c ../PARAMETERS/parameters.cpp

string_support.o: ../STRING_SUPPORT/string_support.cpp
	g++ -Wall -std=c++11 -c ../STRING_SUPPORT/string_support.cpp

clean:
	rm *.o
