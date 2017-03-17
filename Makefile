CC=g++
RANLIB=ranlib

LIBSRC=osm.cpp
LIBOBJ=$(LIBSRC:.cpp=.o)

INCS= -I. -std=c++11 -DNDEBUG
CFLAGS = -Wall -g $(INCS)
LOADLIBES = -L./

OSMLIB = libosm.a
TARGETS = $(OSMLIB)

TAR=tar
TARFLAGS=-cvf
TARNAME=ex1.tar
TARSRCS=$(LIBSRC) Makefile README


all: $(TARGETS) test

osm.o:
	$(CC) -c osm.cpp osm.h $(CFLAGS)

$(TARGETS): $(LIBOBJ)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

clean:
	$(RM) $(TARGETS) $(OSMLIB) $(OBJ) $(LIBOBJ) *~ *core test

depend:
	makedepend -- $(CFLAGS) -- $(SRC) $(LIBSRC)

test:
	$(CC) -O0 -o test simpletest.cpp osm.o  $(CFLAGS)
tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)