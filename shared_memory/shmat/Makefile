CC      := gcc
CCFLAGS := -I
LDFLAGS :=

TARGETS:= writer reader
MAINS  := $(addsuffix .o, $(TARGETS) )
OBJ    := ../shared_memory.o 
DEPS   := # command.h

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