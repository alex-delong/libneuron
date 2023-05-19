.PHONY: build

INCLUDE_DIR=../../include
LIB_DIR=../../lib
LIB=libneuron.so
CXX=g++
CXX_FLAGS=-Wall -std=c++17 -O3 -I$(INCLUDE_DIR) -L$(LIB_DIR) -lneuron -ledge 


build: $(LIB_DIR)/$(LIB)

$(LIB_DIR)/$(LIB): lib.cpp lib.hpp
	$(CXX) $(CXX_FLAGS) -c lib.cpp -o $(LIB_DIR)/$(LIB)

test: test.o
	$(CXX) $(CXX_FLAGS) -l boost_unit_test_framework test.o -o test

test.o: test.cpp lib.hpp
	$(CXX) $(CXX_FLAGS) -c test.cpp -o test.o

