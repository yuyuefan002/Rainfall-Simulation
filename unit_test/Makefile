CC=g++
CFLAGS=-std=gnu++11 -pedantic -ggdb3
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
BIN=PointTest RainfallSimulatorTest
all: $(BIN)
simulator:$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)
%.o:%.cpp
	$(CC) $(CFLAGS) -c $<
.PHONY: clean
clean:
	rm -f $(BIN) *.o *~
