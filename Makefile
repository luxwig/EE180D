UNAME=$(shell uname)
CC=gcc
CFLAGS=-g -c -Wall -Werror
INCLUDES=-I/usr/local/include
LDFLAGS=-lm -lfann -L/usr/local/lib -lpthread 
IMPORTDIR=matlab_import
FANNDIR=FANN
SOURCES=statistics.c util.c $(wildcard ${IMPORTDIR}/*.c) $(wildcard ${FANNDIR}/*.c)
CFEXECUTABLE=cf_main
TREXECUTABLE=tr_main
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

TRSOURCES=$(SOURCES) train.c
CFSOURCES=$(SOURCES) cf.c
TROBJECTS=$(TRSOURCES:.c=.o)
CFOBJECTS=$(CFSOURCES:.c=.o)


all: $(CFSOURCES) $(CFEXECUTABLE)

train: $(TRSOURCES) $(TREXECUTABLE)

$(CFEXECUTABLE): $(CFOBJECTS)
	$(CC) $(CFOBJECTS) $(LDFLAGS) -o $@

$(TREXECUTABLE): $(TROBJECTS)
	$(CC) $(TROBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf main train.txt test.txt feature.net
	rm -rf *.o
	rm -rf ${IMPORTDIR}/*.o
	rm -rf ${FANNDIR}/*.o
