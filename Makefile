CXX=g++
CP=cp
CXXFLAGS= -c -Wall -g -O2 -std=c++0x
INCLUDE = -Iinclude
LDFLAGS= -lpthread 
RM=rm
SOURCES= $(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
DEPS = $(SOURCES:.cpp=.d)

EXECUTABLE=fuzzer

all: $(SOURCES) $(EXECUTABLE)

compile: $(OBJECTS)

recompile: clean all

depends: $(SOURCES)
	rm -f ./depends.d
	make do_make_deps

do_make_deps: $(DEPS)

%.d : %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -MG -MM -MP -MT"$(<:.cpp=.o)" $< >> depends.d

install:
	$(CP) $(EXECUTABLE) /usr/bin/
uninstall:
	$(RM) /usr/bin/$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@

clean:
	rm $(OBJECTS) $(EXECUTABLE)

-include depends.d
