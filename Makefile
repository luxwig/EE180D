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

.PHONY: train
.PHONY: classify

all: $(CFSOURCES) $(CFEXECUTABLE) $(TRSOURCES) $(TREXECUTABLE)

classify: $(CFSOURCES) $(CFEXECUTABLE)

$(CFEXECUTABLE): $(CFOBJECTS)
	$(CC) $(CFOBJECTS) $(LDFLAGS) -o $@


train: $(TRSOURCES) $(TREXECUTABLE)

$(TREXECUTABLE): $(TROBJECTS)
	$(CC) $(TROBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean_all: clean
	rm -rf *.net
		   
clean:
	rm -rf *_main
	rm -rf *.o
	rm -rf ${IMPORTDIR}/*.o
	rm -rf ${FANNDIR}/*.o
