CXX = g++
CXXFLAGS = -g -ggdb -Wall -pedantic -std=c++14
INCLUDES = -I. -I../include/
LIBS = -lrt -lm
LIB_DIR = ../libs/
PROGRAM = ImpliedEngine_test
OBJS = $(LIB_DIR)DataElement.o $(LIB_DIR)Visitor.o $(LIB_DIR)Graph.o $(LIB_DIR)MarketGraph.o $(LIB_DIR)BinaryHeap.o $(LIB_DIR)cat1_visitor.o $(LIB_DIR)cat2_visitor.o $(LIB_DIR)SecPair.o $(LIB_DIR)ImpliedEngine.o $(LIB_DIR)Decomposer.o

COMPILE = $(CXX) $(CXXFLAGS) $(INCLUDES)

all: $(PROGRAM)

SecPair_test :  SecPair_test.cpp SecPair_test.hpp $(OBJS)
	$(COMPILE) -o $(PROGRAM) SecPair_test.cpp $(OBJS) $(LIBS)

ImpliedEngine_test :  ImpliedEngine_test.cpp ImpliedEngine_test.hpp $(OBJS)
	$(COMPILE) -o $(PROGRAM) ImpliedEngine_test.cpp $(OBJS) $(LIBS)

clean:
	rm -fr *.o *~ $(PROGRAM)
