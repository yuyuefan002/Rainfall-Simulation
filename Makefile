CC=g++
CFLAGS=-std=gnu++11 -O3 -pedantic
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
PT_OBJS=$(filter-out rainfall_seq.o, $(OBJS))
SEQ_OBJS=$(filter-out rainfall_pt.o, $(OBJS))
BIN=rainfall_pt rainfall_seq
all: $(BIN)

rainfall_pt:$ Point_pt.o RainfallSimulator_pt.o rainfall_pt.o
	$(CC) $(CFLAGS) -o $@ Point_pt.o RainfallSimulator_pt.o rainfall_pt.o -lpthread

rainfall_seq:$(SEQ_OBJS)
	$(CC) $(CFLAGS) -o $@ $(SEQ_OBJS) -lpthread
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -lpthread
.PHONY: clean
clean:
	rm -f $(BIN) *.o *~ gmon.out result analysis.txt
test:
	./test.sh
