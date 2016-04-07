OBJDIR = $(GARFIELD_HOME)/Object
SRCDIR = $(GARFIELD_HOME)/Source
INCDIR = $(GARFIELD_HOME)/Include
HEEDDIR = $(GARFIELD_HOME)/Heed
LIBDIR = $(GARFIELD_HOME)/Library

# Compiler flags
CFLAGS = -Wall -Wextra -Wno-long-long \
        `root-config --cflags` -std=c++11 \
        -O3 -fno-common -c \
        -I$(INCDIR) -I$(HEEDDIR)

# Debug flags
CFLAGS += -g

LDFLAGS = `root-config --glibs` -lGeom -lgfortran -lm
LDFLAGS += -L$(LIBDIR) -lGarfield 
LDFLAGS += -g

all: clean micromegas

clean:
	rm -f micromegas.o
	rm -f field.o
	rm -f drift.o
	rm -f micromegas
	

micromegas: src/micromegas.cpp
	$(CXX) $(CFLAGS) src/micromegas.cpp src/field.cpp src/drift.cpp
	$(CXX) -o micromegas micromegas.o field.o drift.o $(LDFLAGS)
	rm micromegas.o
	rm field.o
	rm drift.o
	
