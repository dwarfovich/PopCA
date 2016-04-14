CC=g++
CFLAGS = -g -Wall -fopenmp
CXXFLAGS = -std=c++11
LDFLAGS = -fopenmp

BIN = PopCA

SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=%.o)

all: $(OBJ)
	$(CC) $(LDFLAGS) -o $(BIN) $^

%.o: %.c
	$(CC) $(LDFLAGS) $(CXX) $@ -c $<

clean:
	rm -f *.o
	rm $(BIN)