.PHONY: build

MODULE:=binfuns
TARGET:=$(ROOT)/lib/lib$(MODULE).so
OBJ:=$(ROOT)/lib/$(MODULE).o
LDFLAGS:='-Wl,-rpath,$$ORIGIN'
OBJFLAGS:=-g -fPIC

build: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared $(OBJ) -o $(TARGET)

$(OBJ): lib.cpp $(ROOT)/include/$(MODULE).hpp
	$(CXX) $(CXXFLAGS) $(OBJFLAGS) -c lib.cpp -o $(OBJ) 
