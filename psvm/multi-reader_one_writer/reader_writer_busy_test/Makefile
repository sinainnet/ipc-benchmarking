CC      := gcc
CCFLAGS := -I
LDFLAGS := -lrt -lpthread -ansi -Wall 

TARGETS:= writer reader
MAINS  := $(addsuffix .o, $(TARGETS) )
OBJ    := barrier.o reader.o writer.o ../../transblocklibbench.o #../../../semaphore_lib/ipcb_semaphore.o ../../../commons/commons.o
OBJ2   := writer.o barrier.o reader.o
DEPS   := #./commons/commons.h

BUILDPATH	:= ./build

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -f $(TARGETS) $(OBJ2)

recompile:	clean all

$(OBJ): %.o : %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CCFLAGS) $(BUILDPATH)

$(TARGETS): % : $(filter-out $(MAINS), $(OBJ)) %.o
	$(CC) -g -o $@ $(LIBS) $^ $(CCFLAGS) $(BUILDPATH) $(LDFLAGS)
	echo Build Done




# # Digital UNIX 4.0 compilation flags:
# CFLAGS= -mrtd -pthread -g -w $(DEBUGFLAGS)
# RTFLAGS=-lrt

# # Solaris 2.5 compilation flags:
# #CFLAGS=-D_POSIX_C_SOURCE=199506 -D_REENTRANT -Xa -lpthread -g $(DEBUGFLAGS)
# #RTFLAGS=-lposix4

# SOURCES= barrier_main.c	test.c writer.c
# PROGRAMS=$(SOURCES:.c=)
# all:	${PROGRAMS}

# barrier_main: barrier.h barrier.c barrier_main.c
# 	${CC} ${CFLAGS} ${LDFLAGS} -o $@ barrier_main.c barrier.c

# test: barrier.h barrier.c test.c
# 	${CC} ${CFLAGS} ${LDFLAGS} -o $@ test.c barrier.c

# writer: ../../header.h barrier.h barrier.c writer.c ../../transblocklibbench.c
# 	${CC} ${CFLAGS} ${LDFLAGS} -o $@ writer.c barrier.c ../../transblocklibbench.c

# clean:
# 	@rm -rf $(PROGRAMS) *.o
# recompile:	clean all

