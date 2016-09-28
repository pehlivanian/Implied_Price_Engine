all: 	timing.o \
	report.o \
	benchmark.o \
	Visitor.o \
	DataElement.o \
	BinaryHeap.o \
	Graph.o \
	MarketGraph.o \
	SecPair.o \
	Decomposer.o \
	ImpliedQuoteSubscriber.o \
	CMemPool.o \
	cat1_visitor.o \
	cat2_visitor.o \
	ImpliedEngine.o
	mv *.o ../libs

# should we count each comparison between two elements? 
# report will include these results. Also takes care of
# comparing against NULL
COUNTCOMPARE = -DCOUNT
INCLUDE = -I./

CC = g++

DEBUG = -g -ggdb -Wall -Wparentheses -pedantic -std=c++14
LIBS = -lm

CFLAGS = -Wall -Wno-unused-variable -Wno-sign-compare -pedantic $(INCLUDE) $(COUNTCOMPARE) $(DEBUG)

# default rule to build C files
.cpp.o: 
	$(CC) -c $(CFLAGS) $(INC) $*.cpp $(LIBS)

benchmark: benchmark.o timing.o report.o
	$(CC) -o $@ $+ timing.o report.o

test:
	@echo "No tests for Timing"

clean:
	rm -f *~ *.o
	rm -f a.out out
	rm -f ../libs/*.*	
