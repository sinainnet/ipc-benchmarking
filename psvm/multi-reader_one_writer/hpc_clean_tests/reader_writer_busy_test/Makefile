CC      := gcc
CCFLAGS := -I
LDFLAGS := -lrt -lpthread -ansi -Wall 

TARGETS:= writer reader
MAINS  := $(addsuffix .o, $(TARGETS) )
OBJ    := reader.o writer.o ../../../transblocklibbench.o
OBJ2   := writer.o reader.o
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

