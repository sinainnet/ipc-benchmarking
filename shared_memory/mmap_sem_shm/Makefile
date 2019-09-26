CC      := gcc
CCFLAGS := -I
LDFLAGS := -lrt -lpthread

TARGETS:= shm_master shm_writer shm_reader 
MAINS  := $(addsuffix .o, $(TARGETS) )
OBJ    := ../../semaphore_lib/ipcb_semaphore.o ../shared_memory.o ../../commons/commons.o
DEPS   := #./commons/commons.h

BUILDPATH	:= ./build

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -f $(TARGETS) $(OBJ)

$(OBJ): %.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CCFLAGS) $(BUILDPATH)

$(TARGETS): % : $(filter-out $(MAINS), $(OBJ)) %.o
	$(CC) -o $@ $(LIBS) $^ $(CCFLAGS) $(BUILDPATH) $(LDFLAGS)
	echo Build Done
