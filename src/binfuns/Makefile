.PHONY: build

INCLUDE_DIR=../../include
LIB_DIR=../../lib
LIB=libbinfuns
CXX=g++
CXX_FLAGS=-Wall -std=c++17 -O3 -I $(INCLUDE_DIR) -L $(LIB_DIR)

build: $(LIB_DIR)/$(LIB).so

$(LIB_DIR)/$(LIB).so: $(LIB).cpp 
	$(CXX) $(CXX_FLAGS) -c $(LIB).cpp -o $(LIB_DIR)/$(LIB).so
	
