
all: test_punreader test_logreader

test_logreader: test_logreader.cpp filereader.o logreader.o matrix.o string_support.o parameters.o 
	g++ -Wall -std=c++11 test_logreader.cpp filereader.o logreader.o matrix.o string_support.o parameters.o -o test_logreader

test_punreader: test_punreader.cpp filereader.o punreader.o matrix.o string_support.o parameters.o 
	g++ -Wall -std=c++11 test_punreader.cpp filereader.o punreader.o matrix.o string_support.o parameters.o -o test_punreader

#argumentfileexist.o : argumentfileexist.cpp
#	g++ -Wall -std=c++11 -c argumentfileexist.cpp
#
#argumentfileext.o : argumentfileext.cpp
#	g++ -Wall -std=c++11 -c argumentfileext.cpp
#
#argumentparser.o : argumentparser.cpp
#	g++ -Wall -std=c++11 -c argumentparser.cpp
#
#argumentstring.o : argumentstring.cpp
#	g++ -Wall -std=c++11 -c argumentstring.cpp
#
#argumentdouble.o : argumentdouble.cpp
#	g++ -Wall -std=c++11 -c argumentdouble.cpp
#
#argumentint.o : argumentint.cpp
#	g++ -Wall -std=c++11 -c argumentint.cpp
#
logreader.o: logreader.cpp filereader.o matrix.o string_support.o
	g++ -Wall -std=c++11 matrix.o string_support.o filereader.o -c logreader.cpp

punreader.o: punreader.cpp filereader.o matrix.o string_support.o
	g++ -Wall -std=c++11 matrix.o string_support.o filereader.o -c punreader.cpp

filereader.o: filereader.cpp matrix.o string_support.o
	g++ -Wall -std=c++11 matrix.o string_support.o -c filereader.cpp

io.o: ../io.cpp matrix.o string_support.o
	g++ -Wall -std=c++11 matrix.o string_support.o -c ../io.cpp

matrix.o: ../../MATRIX/matrix.cpp
	g++ -Wall -std=c++11 -c ../../MATRIX/matrix.cpp

parameters.o: ../../PARAMETERS/parameters.cpp
	g++ -Wall -std=c++11 -c ../../PARAMETERS/parameters.cpp

string_support.o: ../../STRING_SUPPORT/string_support.cpp
	g++ -Wall -std=c++11 -c ../../STRING_SUPPORT/string_support.cpp

clean:
	rm *.o test_punreader test_logreader
