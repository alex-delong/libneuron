.PHONY: build test 

INCLUDE_DIR=../../include
LIB_DIR=../../lib
LIB=libneuron.so
CXX=g++
CXX_FLAGS=-Wall -std=c++17 -O3 -I$(INCLUDE_DIR) -L$(LIB_DIR) -lneuron -ledge 


build: $(LIB_DIR)/$(LIB)

test: bin/test

$(LIB_DIR)/$(LIB): src/lib.cpp src/lib.hpp
	$(CXX) $(CXX_FLAGS) -c src/lib.cpp -o $(LIB_DIR)/$(LIB)

bin/test: lib/test.o
	mkdir -p bin
	$(CXX) $(CXX_FLAGS) -l boost_unit_test_framework lib/test.o -o bin/test

lib/test.o: src/test.cpp src/lib.hpp
	mkdir -p lib
	$(CXX) $(CXX_FLAGS) -c src/test.cpp -o lib/test.o

