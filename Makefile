.PHONY: all run build build-libs clean config test

ROOT:=$(CURDIR)
SRCS:=$(wildcard src/*.cpp)
MODS:=$(patsubst src/%/, %, $(wildcard src/*/))
OBJS:=$(patsubst src/%.cpp, lib/%.o, $(SRCS))
LIBS:=$(patsubst %, lib/lib%.so, $(MODS))
TARGET:=bin/program
BUILDDIRS:=bin lib

CXX:=g++
CXXFLAGS:=-std=c++17 -Wall -Wextra -fstack-protector -fPIE
CPPFLAGS:= -I $(ROOT)/include
LDFLAGS:='-Wl,-z,relro,-z,now,-rpath,$$ORIGIN/../lib' -L $(ROOT)/lib -o $(TARGET) -pie 
LDLIBS:=$(patsubst %, -l %, $(MODS))

export ROOT MODS CXX CPPFLAGS

all: build test 

build: config $(TARGET)

build-libs: config
	$(MAKE) build -e -C src

run: build 
	./$(TARGET)

clean:
	rm -rf bin lib

config: $(BUILDDIRS)

test: build-libs
	$(MAKE) build -e -C test
	./bin/test

$(BUILDDIRS):
	mkdir -p $(BUILDDIRS)

$(TARGET): $(OBJS) $(LIBS)
	$(CXX) $(LDFLAGS) $(LDLIBS) $(OBJS) 

$(OBJS): lib/%.o: src/%.cpp 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(LIBS): build-libs
