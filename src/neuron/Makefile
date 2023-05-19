.PHONY: build

INCLUDE_DIR=../../include
LIB_DIR=../../lib
LIB=libneuron.so
CXX=g++
CXX_FLAGS=-Wall -std=c++17 -O3 -I $(INCLUDE_DIR) -L $(LIB_DIR)

build: $(LIB_DIR)/$(LIB)

$(LIB_DIR)/$(LIB): lib.cpp lib.hpp
	$(CXX) $(CXX_FLAGS) -c lib.cpp -o $(LIB_DIR)/$(LIB)
