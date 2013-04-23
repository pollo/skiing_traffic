CC=g++

#debug
#CFLAGS=-c -Wall -DDEBUG
#DEBUG=-g
#no debug
CFLAGS=-c -Wall -DNDEBUG -O3
DEBUG=

INC=-I/usr/lib/grass64/include -I/usr/include/gdal/
LIBS=-L/usr/lib/grass64/lib -lgrass_vect -lgrass_gis


all: skiing_traffic

skiing_traffic: main.o grassbackend.o slope.o physicalforces.o socialforces.o parameters.o skier.o vector.o
	$(CC) $(LIBS) $(INC) $(DEBUG) main.o slope.o grassbackend.o parameters.o skier.o vector.o physicalforces.o socialforces.o -o skiing_traffic

main.o: main.cpp
	$(CC) $(CFLAGS) $(INC) $(DEBUG) main.cpp

grassbackend.o: grassbackend.cpp grassbackend.h gisbackend.h
	$(CC) $(CFLAGS) $(INC) $(DEBUG) grassbackend.cpp

slope.o: slope.cpp slope.h skier.h socialforces.h physicalforces.h vector.h gisbackend.h
	$(CC) $(CFLAGS) $(INC) $(DEBUG) slope.cpp

skier.o: skier.cpp skier.h parameters.h slope.h
	$(CC) $(CFLAGS) $(INC) $(DEBUG) skier.cpp

parameters.o: parameters.cpp parameters.h
	$(CC) $(CFLAGS) $(INC) $(DEBUG) parameters.cpp

physicalforces.o: physicalforces.cpp physicalforces.h parameters.h
	$(CC) $(CFLAGS) $(INC) $(DEBUG) physicalforces.cpp


socialforces.o: socialforces.cpp socialforces.h parameters.h
	$(CC) $(CFLAGS) $(INC) $(DEBUG) socialforces.cpp

vector.o: vector.cpp vector.h
	$(CC) $(CFLAGS) $(INC) $(DEBUG) vector.cpp

clean:
	find . -name "*.o" | xargs rm -f
	find . -name "*~" | xargs rm -f
	rm -f skiing_traffic

.PHONY: clean
