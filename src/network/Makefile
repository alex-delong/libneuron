.PHONY: build config

MODULE:=network
TARGET:=$(ROOT)/lib/lib$(MODULE).so
OBJS:=$(patsubst ./%.cpp, $(ROOT)/lib/$(MODULE)/%.o, $(wildcard ./*.cpp))

CXXFLAGS:=-std=c++17 -Wall -Wextra -fPIC -g
LDFLAGS:='-Wl,-rpath,$$ORIGIN' -shared -o $(TARGET)

config: $(ROOT)/lib/$(MODULE) 

build: config $(TARGET)

$(TARGET): $(OBJS) 
	$(CXX) $(LDFLAGS) $(OBJS)

$(OBJS): $(ROOT)/lib/$(MODULE)/%.o : %.cpp $(wildcard ./*.hpp) $(ROOT)/include/$(MODULE).hpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(ROOT)/lib/$(MODULE):
	mkdir -p $(ROOT)/lib/$(MODULE)
