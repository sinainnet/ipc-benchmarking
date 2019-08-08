CC      := gcc
CCFLAGS := -I
LDFLAGS := -lrt -lpthread

TARGETS:= ipcb_xpmem_master ipcb_xpmem_writer ipcb_xpmem_reader
MAINS  := $(addsuffix .o, $(TARGETS) )
# OBJ    := ../../commons/commons.o
XPMEM  := #../lib/.libs/libxpmem.so  #../lib/.libs/libxpmem.a #
LIBS := #-L ../lib/.libs/libxpmem.so.0
DEPS   := #./commons/commons.h

BUILDPATH       := ../.

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -f $(TARGETS) $(OBJ)

$(OBJ): %.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CCFLAGS) $(BUILDPATH)

$(TARGETS): % : $(filter-out $(MAINS), $(OBJ)) %.o
	$(CC) -g -o $@ $(LIBS) $^ $(CCFLAGS) $(BUILDPATH) $(LDFLAGS)
	echo Build Done

