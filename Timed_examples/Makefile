CC = g++ -g

INCLUDE = -I../include/

CFLAGS = -O3 -std=c++14 -Wall -pedantic $(INCLUDE)

RTLIBS = -lrt -lm

# Mac OS doesn't fully implement POSIX timing
UNAME := $(shell uname)

# Darwin is mac value
ifeq ($(UNAME), Darwin)
  RTLIBS=
endif

# default rule to build C files
.c.o: 
	$(CC) -c $(CFLAGS) $*.c

REPORT = ../libs/report.o
TIMING = ../libs/timing.o  

# all: singletest grouptest dijkstratest typetest CLlegtest gentest
all: grouptest typetest dijkstratest CLlegtest gentest mempooltest allocatortest

grouptest: grouptest.o
	$(CC) -o $@ $+ $(REPORT)

dijkstratest: dijkstratest.o
	$(CC) -o $@ $+ $(REPORT) ../libs/Graph.o ../libs//BinaryHeap.o ../libs/dijkstra_visitor.o

typetest: typetest.o
	$(CC) -o $@ $+ $(REPORT)

CLlegtest: CLlegtest.o
	$(CC) -o $@ $+ $(REPORT) ../libs/Graph.o ../libs//BinaryHeap.o ../libs/dijkstra_visitor.o ../libs/bellmanford_visitor.o

gentest: gentest.o
	$(CC) -o $@ $+ $(REPORT) ../libs/Graph.o ../libs//BinaryHeap.o ../libs/dijkstra_visitor.o 

mempooltest: mempooltest.o
	$(CC) -o $@ $+ $(REPORT) ../libs/Graph.o ../libs//BinaryHeap.o ../libs/CMemPool.o

allocatortest: allocatortest.o
	$(CC) -o $@ $+ $(REPORT)
 
clean:
	rm -f *~ a.out *.o singletest grouptest dijkstratest typetest gentest allocatortest



