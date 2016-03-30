
all: test

test: test.cpp io.o io.hpp
	g++ -Wall test.cpp io.o -o test

io.o: io.cpp io.hpp
	g++ -Wall -c io.cpp

clean:
	rm *.o
