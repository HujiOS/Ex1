CC = g++
CFLAGS = -Wall -Wextra -DNDEBUG -g -std=c++11

default:
	$(CC) simpletest.cpp osm.cpp osm.h -o ex1 $(CFLAGS)