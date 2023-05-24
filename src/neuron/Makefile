.PHONY: build

MODULE:=neuron
TARGET:=$(ROOT)/lib/lib$(MODULE).so
IMPL:=$(ROOT)/lib/lib$(MODULE)impl.so
LDFLAGS:='-Wl,-rpath,$$ORIGIN'
LIBFLAGS:=-g -fPIC -shared

build: $(TARGET)

$(TARGET): lib.cpp impl.hpp $(ROOT)/include/$(MODULE).hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LIBFLAGS) lib.cpp -o $(TARGET) 
