CC=g++
CFLAGS=-c -Wall
INC=-I/usr/lib/grass64/include -I/usr/include/gdal/
LIBS=-L/usr/lib/grass64/lib -lgrass_vect -lgrass_gis
DEBUG=-g

all: skiing_traffic

skiing_traffic: main.o grassbackend.o slope.o downhill_force.o parameters.o skier.o
	$(CC) $(LIBS) $(INC) $(DEBUG) main.o slope.o grassbackend.o parameters.o skier.o -o skiing_traffic

main.o: main.cpp
	$(CC) $(CFLAGS) $(INC) $(DEBUG) main.cpp

grassbackend.o: grassbackend.cpp grassbackend.h gisbackend.h
	$(CC) $(CFLAGS) $(INC) $(DEBUG) grassbackend.cpp

slope.o: slope.cpp slope.h skier.h socialforce.h physicalforce.h vector.h gisbackend.h
	$(CC) $(CFLAGS) $(INC) $(DEBUG) slope.cpp

skier.o: skier.cpp skier.h parameters.h slope.h
	$(CC) $(CFLAGS) $(INC) $(DEBUG) skier.cpp

parameters.o: parameters.cpp parameters.h
	$(CC) $(CFLAGS) $(INC) $(DEBUG) parameters.cpp

downhill_force.o: downhill_force.cpp downhill_force.h parameters.h
	$(CC) $(CFLAGS) $(INC) $(DEBUG) downhill_force.cpp

clean:
	find . -name "*.o" | xargs rm -f
	find . -name "*~" | xargs rm -f
	rm -f skiing_traffic

.PHONY: clean
