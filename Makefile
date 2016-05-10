OBJDIR = $(GARFIELD_HOME)/Object
SRCDIR = $(GARFIELD_HOME)/Source
INCDIR = $(GARFIELD_HOME)/Include
HEEDDIR = $(GARFIELD_HOME)/Heed
LIBDIR = $(GARFIELD_HOME)/Library

# Directory which contains TGeoToStep.h file. It is not installed by root by default (bug?)
#GEOCADDIR = /home/bblidaru/Software/root-6.06.00/geom/geocad/inc/

# Compiler flags
CFLAGS = -Wall -Wextra -Wno-long-long -Wno-unused -Wno-unused-parameter \
        `root-config --cflags` -std=c++11 \
        -O3 -fno-common -c \
        -I$(INCDIR) -I$(HEEDDIR) 

# Debug flags
CFLAGS += -g

LDFLAGS = `root-config --glibs` -lGeom -lGeoCad -lgfortran -lm
LDFLAGS += -L$(LIBDIR) -lGarfield 
LDFLAGS += -g

all: clean micromegas plain low

clean:
	rm -f micromegas.o
	rm -f field.o
	rm -f drift.o
	rm -f geometry.o
	rm -f micromegas
	rm -f plot.o
	
plain: src/plain.cpp
	$(CXX) $(CFLAGS) src/plain.cpp
	$(CXX) -o plain plain.o $(LDFLAGS)
	rm plain.o

micromegas: src/micromegas.cpp
	$(CXX) $(CFLAGS) src/micromegas.cpp src/field.cpp src/drift.cpp src/geometry.cpp src/plot.cpp
	$(CXX) -o micromegas micromegas.o field.o drift.o geometry.o plot.o $(LDFLAGS)
	rm micromegas.o
	rm field.o
	rm drift.o
	rm geometry.o
	rm plot.o
	
low: src/mm_low_working_salome.cpp
	$(CXX) $(CFLAGS) src/mm_low_working_salome.cpp
	$(CXX) -o low mm_low_working_salome.o $(LDFLAGS)
	rm mm_low_working_salome.o
	
	
	
	
	
	
