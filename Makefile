.PHONY: run build clean test conf build-lib build-test

ROOT:=$(CURDIR)
SRC:=$(wildcard src/*.cpp)
MOD:=$(patsubst src/%/, %, $(wildcard src/*/))
OBJ:=$(patsubst src/%.cpp, lib/%.o, $(SRC))
TESTOBJ:=$(patsubst %, lib/test%.o, $(MOD)) lib/testmain.o
TESTBIN:=bin/test
LIB:=$(patsubst %, lib/lib%.so, $(MOD))
BIN:=bin/program

CXX:=g++
CXXFLAGS:=-std=c++17 -Wall -Wextra -I $(ROOT)/include -L $(ROOT)/lib

LDFLAGS:='-Wl,-z,relro,-z,now,-rpath,$$ORIGIN/../lib'
LDLIBS:=$(patsubst %, -l %, $(MOD))
BINFLAGS:=-fstack-protector -fPIE -pie

export ROOT MOD CXX CXXFLAGS

run: build
	./$(BIN)

run-test: build-test
	./$(TESTBIN)

build: conf $(BIN)

clean:
	rm -rf bin lib

conf:
	mkdir -p bin lib

build-lib: conf
	$(MAKE) build -e -C src

build-test: $(TESTBIN)

$(BIN): $(OBJ) $(LIB)
	$(CXX) $(CXXFLAGS) $(BINFLAGS) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $(BIN) 

$(OBJ): lib/%.o: src/%.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TESTOBJ): lib/test%.o: test/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TESTBIN): build-lib $(TESTOBJ)
	$(CXX) $(CXXFLAGS) $(BINFLAGS) $(TESTOBJ) $(LDFLAGS) $(LDLIBS) -o $(TESTBIN)

$(LIB): build-lib


