#comments
#redefine these from command line
LIB_GL=-lglu32 -lopengl32
LIB_SDL=-lSDL

CFLAGS=-Wall -g -c
LDFLAGS=
LIBS=$(LIB_GL) $(LIB_SDL)

#directories
HEDDIR=src
SRCDIR=src
OBJDIR=obj
EXEDIR=bin

#header file list
_HEADERS=grid_graph.hpp topology.hpp sdl_app.hpp vec2.hpp vec2.hpp \
include_important_things.hpp pathfinder.hpp
HEADERS=$(patsubst %,$(HEDDIR)/%,$(_HEADERS))

#source file list
_SOURCES=main.cpp grid_graph.cpp sdl_app.cpp
SOURCES=$(patsubst %,$(SRCDIR)/%,$(_SOURCES))

#object file list
_OBJECTS=$(_SOURCES:.cpp=.o)
OBJECTS=$(patsubst %,$(OBJDIR)/%,$(_OBJECTS))

#output file list
_EXECUTABLE=main.exe
EXECUTABLE=$(patsubst %,$(EXEDIR)/%,$(_EXECUTABLE))

#rules

$(EXECUTABLE): $(OBJECTS) $(EXEDIR)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) $(OBJDIR)
	$(CXX) $(LDFLAGS) $(CFLAGS) -o $@ $<


$(EXEDIR):
	test -d $(EXEDIR) || mkdir $(EXEDIR)
$(OBJDIR):
	test -d $(OBJDIR) || mkdir $(OBJDIR)

clean:
	rm -r -f $(OBJDIR) $(EXEDIR)