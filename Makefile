CC=g++
CFLAGS=-std=gnu++11 -pedantic -O3 -pthread
SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))
PT_OBJS=$(filter-out rainfall_seq.o, $(OBJS))
SEQ_OBJS=$(filter-out rainfall_pt.o, $(OBJS))
BIN=rainfall_pt rainfall_seq
all: $(BIN)

rainfall_pt:$(PT_OBJS)
	$(CC) $(CFLAGS) -o $@ $(PT_OBJS) 

rainfall_seq:$(SEQ_OBJS)
	$(CC) $(CFLAGS) -o $@ $(SEQ_OBJS)
%.o:%.cpp
	$(CC) $(CFLAGS) -c $<
.PHONY: clean
clean:
	rm -f $(BIN) *.o *~ gmon.out result analysis.txt
test:
	./test.sh
