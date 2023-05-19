.PHONY: build

INCLUDE_DIR=../../include
LIB_DIR=../../lib
LIB=libneuron
CXX=g++
CXX_FLAGS=-Wall -std=c++17 -O3 -I $(INCLUDE_DIR) -L $(LIB_DIR)

build: $(LIB_DIR)/$(LIB).so

$(LIB_DIR)/$(LIB).so: lib.cpp lib.hpp
	$(CXX) $(CXX_FLAGS) -c lib.cpp -o $(LIB_DIR)/$(LIB).so
	
