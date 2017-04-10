UNAME=$(shell uname)
CC=gcc
CFLAGS=-g -c -Wall -Werror
INCLUDES=-I/usr/local/include
LDFLAGS=-lm -lfann -L/usr/local/lib -lpthread 
IMPORTDIR=matlab_import
FANNDIR=FANN
SOURCES=cf.c statistics.c $(wildcard ${IMPORTDIR}/*.c) $(wildcard ${FANNDIR}/*.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main
MODE=NORMAL

ifeq ($(UNAME), Linux)
    $(info OS: LINUX)
    ifeq ($(MODE), DEBUG)
        $(info MODE: **DEBUG**)
        CFLAGS += -D _DEBUG
    else
        $(info MODE: NORMAL)
        LDFLAGS += -lmraa
        SOURCES += LSM9DS0.c
    endif
else
    $(info OS: DARWIN)
    $(info MODE: **DEBUG**)
    CFLAGS += -D _DEBUG
endif


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
