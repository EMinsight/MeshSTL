# This makefile creates an executable for the program MeshSTL
# Author: Christopher Kozuch
# Date: 24 September 2015

COMP = g++
CFLAGS = -O3 -Wall
OBJS = $(SOURCES: .cpp = .o)
SOURCES = Edge.cpp Extrema.cpp Facet.cpp FacetVertex.cpp Hex.cpp HexVertex.cpp main.cpp Octree.cpp Point.cpp Surface.cpp Writer.cpp
EXE = meshstl

all: $(SOURCES) $(EXE)

$(EXE): $(OBJS)
	$(COMP) $(OBJS) -o $(EXE)
	
clean:
		rm *o
