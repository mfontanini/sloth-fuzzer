CXX=g++
CP=cp
MV=mv
CXXFLAGS= -c -Wall -g -std=c++0x
FLEX=flex
BISON=bison
INCLUDE = -Iinclude
LDFLAGS= -lpthread -lcrypto
RM=rm
SOURCES= $(wildcard src/*.cpp src/functions/*.cpp) src/parser/syntax_parser.cpp
OBJECTS=$(SOURCES:.cpp=.o) src/parser/grammar.o src/parser/syntax.o
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

src/parser/grammar.o: parser/grammar.y parser/syntax.lex include/parser/nodes.h
	bison -d -o parser/grammar-output parser/grammar.y
	$(MV) parser/grammar-output src/parser/grammar.cpp 
	$(MV) parser/grammar-output.h include/parser/syntax.tab.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) src/parser/grammar.cpp -o src/parser/grammar.o

src/parser/syntax.o: parser/syntax.lex src/parser/grammar.o
	$(FLEX) -o src/parser/syntax.cpp parser/syntax.lex
	$(CXX) $(CXXFLAGS) $(INCLUDE) src/parser/syntax.cpp -o src/parser/syntax.o

clean:
	rm $(OBJECTS) $(EXECUTABLE)

-include depends.d
