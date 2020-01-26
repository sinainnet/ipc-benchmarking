# Digital UNIX 4.0 compilation flags:
CFLAGS= -mrtd -pthread -g -w $(DEBUGFLAGS)
RTFLAGS=-lrt

# Solaris 2.5 compilation flags:
#CFLAGS=-D_POSIX_C_SOURCE=199506 -D_REENTRANT -Xa -lpthread -g $(DEBUGFLAGS)
#RTFLAGS=-lposix4

SOURCES= barrier_main.c	test.c
PROGRAMS=$(SOURCES:.c=)
all:	${PROGRAMS}

barrier_main: barrier.h barrier.c barrier_main.c
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ barrier_main.c barrier.c

test: barrier.h barrier.c test.c
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ test.c barrier.c

clean:
	@rm -rf $(PROGRAMS) *.o
recompile:	clean all

