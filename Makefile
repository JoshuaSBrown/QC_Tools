
all: run

run: main.cpp io.o matrix.o qc_functions.o
	g++ -Wall -std=c++11 main.cpp io.o matrix.o qc_functions.o -o calc_J

io.o: IO/io.cpp
	g++ -Wall -std=c++11 -c IO/io.cpp

matrix.o: MATRIX/matrix.cpp 
	g++ -Wall -std=c++11 -c MATRIX/matrix.cpp

qc_functions.o: QC_FUNCTIONS/qc_functions.cpp
	g++ -Wall -std=c++11 -c QC_FUNCTIONS/qc_functions.cpp

clean:
	rm *.o
