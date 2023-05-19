.PHONY: build test 

INCLUDE_DIR=../../include
LIB_DIR=../../lib
LIB=libneuron.so
CXX=g++
CXX_FLAGS=-Wall -std=c++17 -O3 -I$(INCLUDE_DIR) -L$(LIB_DIR) -lneuron -ledge 

clean:
	rm -rf bin lib
	mkdir bin lib

build: $(LIB_DIR)/$(LIB)

test: bin/test

$(LIB_DIR)/$(LIB): lib/$(LIB)
	cp lib/$(LIB) $(LIB_DIR)/$(LIB)

lib/$(LIB): src/lib.cpp src/lib.hpp
	$(CXX) $(CXX_FLAGS) -c src/lib.cpp -o lib/$(LIB)

bin/test: lib/test.o
	mkdir -p bin
	$(CXX) $(CXX_FLAGS) -l boost_unit_test_framework lib/test.o -o bin/test

lib/test.o: src/test.cpp src/lib.hpp
	mkdir -p lib
	$(CXX) $(CXX_FLAGS) -c src/test.cpp -o lib/test.o

