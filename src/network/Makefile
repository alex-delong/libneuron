.PHONY: build

MODULE:=network
TARGET:=$(ROOT)/lib/lib$(MODULE).so
OBJ:=$(ROOT)/lib/$(MODULE).o
IMPL:=$(ROOT)/lib/$(MODULE)impl.o
LDFLAGS:='-Wl,-rpath,$$ORIGIN'
OBJFLAGS:=-g -fPIC

build: $(TARGET)

$(TARGET): $(IMPL) $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared $(IMPL) $(OBJ) -o $(TARGET)

$(OBJ): lib.cpp impl.hpp $(ROOT)/include/$(MODULE).hpp
	$(CXX) $(CXXFLAGS) $(OBJFLAGS) -c lib.cpp -o $(OBJ) 

$(IMPL): impl.hpp impl.cpp 
	$(CXX) $(CXXFLAGS) $(OBJFLAGS) -c impl.cpp -o $(IMPL)
