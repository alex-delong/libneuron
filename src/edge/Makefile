.PHONY: build test

MODULE:=edge
TARGET:=$(ROOT)/lib/lib$(MODULE).so
TEST:=$(ROOT)/lib/test$(MODULE).o
LDFLAGS:='-Wl,-rpath,$$ORIGIN'
LIBFLAGS:=-g -fPIC -shared

build: $(TARGET)

test: $(TEST)

$(TARGET): lib.cpp $(ROOT)/include/$(MODULE).hpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LIBFLAGS) lib.cpp -o $(TARGET) 

$(TEST): test.cpp $(ROOT)/include/$(MODULE).hpp
	$(CXX) $(CXXFLAGS) -c test.cpp -o $(TEST)
