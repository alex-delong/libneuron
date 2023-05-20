.PHONY: build test clean 

INCLUDE_DIR=../../include
LIB_DIR=../../lib
LIB=libneuron.so
CXX=g++
CXX_FLAGS=-Wall -std=c++17 -O3 -I include -I$(INCLUDE_DIR) -L lib  -L $(LIB_DIR)

clean:
	rm -rf bin lib
	mkdir bin lib

build: lib/libimpl.so $(LIB_DIR)/$(LIB)

test: bin/test

$(LIB_DIR)/$(LIB): lib/$(LIB)
	cp lib/$(LIB) $(LIB_DIR)/$(LIB)

lib/$(LIB): src/lib.cpp include/impl.hpp
	$(CXX) $(CXX_FLAGS) -l impl -c src/lib.cpp -o lib/$(LIB)

bin/test: lib/test.o
	mkdir -p bin
	$(CXX) $(CXX_FLAGS) -l neuron -l edge -l impl -l boost_unit_test_framework lib/test.o -o bin/test

lib/test.o: src/test.cpp include/impl.hpp
	mkdir -p lib
	$(CXX) $(CXX_FLAGS) -c src/test.cpp -o lib/test.o

lib/libimpl.so: src/impl.cpp include/impl.hpp 
	$(CXX) $(CXX_FLAGS) -c src/impl.cpp -o lib/libimpl.so
