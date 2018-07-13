CC = g++
CFLAGS = -Wall -Wextra -std=c++11

all: test_log1 test_log2

test_log1: test_log.cpp 
	$(CC) $(CFLAGS) test_log.cpp -o test_log1

test_log2: test_log.cpp 
	$(CC) $(CFLAGS) -DLOG_LEVEL=1 test_log.cpp -o test_log2

clean:
	rm test_log1 test_log2 
