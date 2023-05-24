# rules assume a single binary that depends on every library
# assume binary source code is split between 1 or more files

ROOT=$(CURDIR)
INCLUDES=$(wildcard include/*.hpp)
SRC=$(wildcard src/*)
MOD:=$(patsubst src/%/, %, $(wildcard src/*/))
LIBS=$(patsubst src/%, lib/lib%.so, $(SRC))

SRC_TEST_UNIT=$(wildcard test/src/test_unit_*.cpp)
BIN_TEST_UNIT=$(patsubst test/src/test_unit_%.cpp, test/bin/test_unit_%, $(SRC_TEST_UNIT))
BIN_TEST_INTEGRATION=test/bin/test_integration
OBJS_TEST_UNIT=$(patsubst test/src/test_unit_%.cpp, test/lib/test_unit_%.o, $(SRC_TEST_UNIT))
OBJS_TEST_INTEGRATION=test/lib/test_integration.o

LDFLAGS:='-Wl,-z,relro,-z,now,-rpath,$$ORIGIN/../lib'
LDLIBS:=$(patsubst %, -l %, $(MOD))
CXX=g++
CXX_FLAGS=-std=c++17 -Wall -Wextra -I $(ROOT)/include -L $(ROOT)/lib -O3
DEPENDS=/usr/lib/libSimAnneal/libAnnealMD.so /usr/lib/libmatplot.so

export ROOT CXX CXXFLAGS MOD

all: test-all build

build: conf 
	$(MAKE) -C src

conf:
	mkdir -p bin lib

clean:
	rm -rf bin lib

test-run: test-unit test-int
	./test/bin/test_unit_neuron
	./test/bin/test_integration

test-unit-run: test-unit
	./test/bin/test_unit_neuron

test-int-run: test-int
	./test/bin/test_integration

test-all: test-unit test-int

test-unit: $(BIN_TEST_UNIT)

test-int: $(BIN_TEST_INTEGRATION)

$(BIN_TEST_UNIT): test/bin/test_unit_% : test/lib/test_unit_%.o $(LIBS) #lib/lib%.so
	mkdir -p test/bin
	$(CXX) $(CXX_FLAGS) $^ -o $@

$(OBJS_TEST_UNIT) : test/lib/test_unit_%.o : test/src/test_unit_%.cpp include/lib%.hpp 
	mkdir -p test/lib
	$(CXX) $(CXX_FLAGS) -c $< -o $@ 

$(BIN_TEST_INTEGRATION) : $(OBJS_TEST_INTEGRATION) $(LIBS) $(DEPENDS)
	mkdir -p test/bin
	$(CXX) $(CXX_FLAGS) $^ -o $@

$(OBJS_TEST_INTEGRATION) : test/src/test_integration.cpp $(INCLUDES)
	mkdir -p test/lib
	$(CXX) $(CXX_FLAGS) -c test/src/test_integration.cpp -o $(OBJS_TEST_INTEGRATION)

