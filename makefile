OBJDIR = $(GARFIELD_HOME)/Object
SRCDIR = $(GARFIELD_HOME)/Source
INCDIR = $(GARFIELD_HOME)/Include
HEEDDIR = $(GARFIELD_HOME)/Heed
LIBDIR = $(GARFIELD_HOME)/Library

# Compiler flags
CFLAGS = -Wall -Wextra -Wno-long-long \
        `root-config --cflags` \
        -O3 -fno-common -c \
        -I$(INCDIR) -I$(HEEDDIR)

# Debug flags
CFLAGS += -g

LDFLAGS = `root-config --glibs` -lGeom -lgfortran -lm
LDFLAGS += -L$(LIBDIR) -lGarfield
LDFLAGS += -g

micromegas: src/micromegas.cpp
	$(CXX) $(CFLAGS) src/micromegas.cpp
	$(CXX) -o micromegas micromegas.o $(LDFLAGS)
	rm micromegas.o
