.PHONY: build test
NAME=edge

BIN_DIR=../../bin
TEST=$(BIN_DIR)/$(NAME)/test
TEST_FLAGS=-L $(LIB_DIR)/$(NAME) -L $(LIB_DIR)/neuron

INCLUDE_DIR=../../include
IMPL_HPP=$(INCLUDE_DIR)/$(NAME)/impl.hpp

LIB_DIR=../../lib
LIB=$(LIB_DIR)/$(NAME)/lib$(NAME).so
LIB_IMPL=$(LIB_DIR)/$(NAME)/libimpl.so
OBJ_TEST=$(LIB_DIR)/$(NAME)/test.o


build: $(LIB)

test: $(TEST)


$(LIB): lib.cpp $(IMPL_HPP)
	$(CXX) $(CXX_FLAGS) -l impl -c lib.cpp -o $(LIB)

$(TEST): $(OBJ_TEST) $(LIB_IMPL)
	$(CXX) $(CXX_FLAGS) $(TEST_FLAGS) -l neuron -l edge -l impl -l boost_unit_test_framework $(OBJ_TEST) -o $(TEST)

$(OBJ_TEST): test.cpp $(IMPL_HPP)
	$(CXX) $(CXX_FLAGS) -c test.cpp -o $(OBJ_TEST)

$(LIB_IMPL): impl.cpp $(IMPL_HPP) 
	$(CXX) $(CXX_FLAGS) -c impl.cpp -o $(LIB_IMPL)
