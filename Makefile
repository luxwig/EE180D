UNAME=$(shell uname)
CC=gcc
CXX=g++
CXXFLAGS=-std=c++11 -c -g
CFLAGS=-g -c -Wall -Werror 
INCLUDES=-I/usr/local/include
LDFLAGS=-lm -lfann -L/usr/local/lib -lpthread -L./ranger -lRandomForest
IMPORTDIR=matlab_import
FANNDIR=FANN
RANGERDIR=ranger
SOURCES=statistics.c util.c $(wildcard ${IMPORTDIR}/*.c) $(wildcard ${FANNDIR}/*.c)
CXXSOURCES=$(wildcard ${RANGERDIR}/*.cpp)
CXXOBJECTS=$(CXXSOURCES:.c=.o)
RANGERLIB=${RANGERDIR}/libRandomForest.so
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
.PHONY: ranger

all:  $(CXXSOURCES) $(RANGERLIB) $(CFSOURCES) $(CFEXECUTABLE) $(TRSOURCES) $(TREXECUTABLE)

classify: $(CFSOURCES) $(CFEXECUTABLE)

$(CFEXECUTABLE): $(CFOBJECTS)
	$(CC) $(CFOBJECTS) $(LDFLAGS) -o $@


train: $(TRSOURCES) $(TREXECUTABLE)

$(TREXECUTABLE): $(TROBJECTS)
	$(CC) $(TROBJECTS) $(LDFLAGS) -o $@

ranger: $(CXXSOURCES) $(RANGERLIB)

$(RANGERLIB): $(CXXSOURCES)
	$(CXX) $(CXXSOURCES) -shared -Wl -std=c++11 -o $@

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean_all: clean
	rm -rf *.net
	rm -rf *.forest
	rm -rf ${RANGERDIR}/*.o
	rm -rf ${RANGERDIR}/*.so	
clean:
	rm -rf *_main
	rm -rf *.o
	rm -rf ${IMPORTDIR}/*.o
	rm -rf ${FANNDIR}/*.o
