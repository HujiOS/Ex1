CC = g++
CFLAGS = -Wall -DNDEBUG -g -std=c++11

default: libosm.a
	$(CC) -O0 -o ex1 simpletest.cpp -L. -losm

libosm.a: osm.o
	ar -rv libosm.a osm.o

osm.o: osm.cpp osm.h
	$(CC) -c osm.cpp osm.h $(CFLAGS)


clean:
	-rm -f *.o *.a *.gch