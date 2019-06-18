CC      := gcc
CCFLAGS := -I
LDFLAGS :=

TARGETS:= shmat 
MAINS  := $(addsuffix .o, $(TARGETS) )
OBJ    := ../shared_memory.o ../../commons/commons.o
DEPS   := .#./commons/commons.h

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