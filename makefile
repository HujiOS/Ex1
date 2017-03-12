CC = g++
CFLAGS = -Wall -DNDEBUG -g -std=c++11

default:
	$(CC) -O0 simpletest.cpp osm.cpp osm.h -o ex1 $(CFLAGS)
