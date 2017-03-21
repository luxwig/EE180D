CC=gcc
CFLAGS=-g -c -Wall
INCLUDES=-I/usr/local/include
LDFLAGS=-lm -lfann -L/usr/local/lib 
IMPORTDIR=matlab_import
FANNDIR=FANN
SOURCES=main.c get_feature_from_file.c $(wildcard ${IMPORTDIR}/*.c) $(wildcard ${FANNDIR}/*.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf main train.txt test.txt feature.net
	rm -rf *.o
	rm -rf ${IMPORTDIR}/*.o
	rm -rf ${FANNDIR}/*.o
