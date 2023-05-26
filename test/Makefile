.PHONY: build

OBJS:=$(patsubst ./%.cpp, $(ROOT)/lib/test%.o, $(wildcard ./*.cpp))
TARGET:=$(ROOT)/bin/test

CXXFLAGS:=-std=c++17 -Wall -Wextra -fstack-protector -fPIE
LDFLAGS:='-Wl,-z,relro,-z,now,-rpath,$$ORIGIN/../lib' -L $(ROOT)/lib -o $(TARGET) -pie 
LDLIBS:=$(patsubst %, -l %, $(MODS)) -l boost_unit_test_framework

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(LDLIBS) $(OBJS)

$(OBJS): $(ROOT)/lib/test%.o : %.cpp 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
