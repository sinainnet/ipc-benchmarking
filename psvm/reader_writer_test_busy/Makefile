CC      := gcc
CCFLAGS := -I 
LDFLAGS := -lrt -lpthread -ansi -Wall 

TARGETS:= reader writer
MAINS  := $(addsuffix .o, $(TARGETS) )
# OBJ    := ../../semaphore_lib/ipcb_semaphore.o ../process_vm.o ../../commons/commons.o
OBJ	   := reader.o writer.o ../transblocklibbench.o
DEPS   := #header.h #./commons/commons.h

BUILDPATH	:= ./build

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -f $(TARGETS) $(OBJ)

$(OBJ): %.o : %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CCFLAGS) $(BUILDPATH)

$(TARGETS): % : $(filter-out $(MAINS), $(OBJ)) %.o
	$(CC) -g -o $@ $(LIBS) $^ $(CCFLAGS) $(BUILDPATH) $(LDFLAGS)
	echo Build Done
