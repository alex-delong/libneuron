.PHONY: build

INCLUDE_DIR=../../include
LIB_DIR=../../lib
LIB=liblayer


build: $(LIB_DIR)/$(LIB).so

$(LIB_DIR)/$(LIB).so: $(LIB).cpp
	$(CXX) $(CXX_FLAGS) -c $(LIB).cpp -o $(LIB_DIR)/$(LIB).so
