/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: writev/writv.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include "psvm.h"

#define PS_VM_ROW_SIZE     ((1ull) << 10)
#define PS_VM_COL_SIZE     ((1ull) << 20)

const unsigned long int memSize = ( PS_VM_ROW_SIZE * PS_VM_COL_SIZE );


/*
 *  Lorem Ipsum
 */
int
main() {
	int fdOne,
	    fdTwo;
	int pidPipe[2], 
	    bufPipe[2];
	char *str, 
	     **buf, 
	     *emptyBuf;
	pid_t childPid, 
	      childOnePid;
    	struct timeval start, 
		       end;

    	// int mkfifo(const char *pathname, mode_t mode);
    	mkfifo(FIFO_FILE_ONE, 0666);
    	mkfifo(FIFO_FILE_TWO, 0666);

    	fdOne = open(FIFO_FILE_ONE, O_RDONLY);
    	fdTwo = open(FIFO_FILE_TWO, O_RDONLY);

        pid_t otherChildId = 0;

	buf = ipcb_fake_data_generator(PS_VM_ROW_SIZE, PS_VM_COL_SIZE);

	read(fdOne, &otherChildId, sizeof(pid_t));
	int size = read(fdTwo, &emptyBuf, sizeof(char*));

        printf("OCPID: %d, size = %d, pointer: %p, row = %lld\n", otherChildId, size, emptyBuf, PS_VM_ROW_SIZE);
	ipcb_get_time(&start, "\nfirst_child:start: "); /* Start. */

	long long int nread = ipcb_process_vm_writev(otherChildId, buf, 
						PS_VM_ROW_SIZE, PS_VM_COL_SIZE,
						emptyBuf, PS_VM_ROW_SIZE, PS_VM_COL_SIZE);

	ipcb_get_time(&end, "\nfirst_child:end: ");  /* End. */

	printf("number of written chars in other ps: %lld", nread);
	printf("\nWriting Data into memory is done.\n");
    	printf("Time in microseconds: %ld microseconds\n",
            		((end.tv_sec - start.tv_sec)*1000000L
	           	+end.tv_usec) - start.tv_usec
          		); // Added semicolon
	kill(otherChildId, SIGHUP);
	return 0;
}
	
