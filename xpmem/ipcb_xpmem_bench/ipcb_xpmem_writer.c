
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: ipcb_xpmem_bench/ipcb_xpmem_writer.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <xpmem.h>
#include <stdio.h> 
#include <string.h>
#include <xpmem_test.h>

#include "ipcb_xpmem.h"

const unsigned long int memSize = ( XPMEM_ROW_SIZE * XPMEM_COL_SIZE );


/*
 *  Lorem Ipsum
 */
int 
main(int argc, char **argv) {

    int fd, fdSync, fdTime, test_nr;
    char *str;
    char **buf;
    pid_t otherChildId;
    struct timeval start, end;

    if (argc < 2) {
		printf("Usage: %s <test number>\n", argv[0]);
		return -1;
	}
	test_nr = atoi(argv[1]);

    buf = ipcb_fake_data_generator(XPMEM_ROW_SIZE, XPMEM_COL_SIZE);
    str = ipcb_map_memory_to_fd(memSize, fd, 0);
    memset(str, '\0', TMP_SHARE_SIZE);

    sem_t *mutex = ipcb_open_semaphore();
    ipcb_wait_semaphore(mutex);
	
    read(fdSync, &otherChildId, sizeof(pid_t));

    printf("==== %s STARTS ====\n", xpmem_test[0].name);

    return (*xpmem_test[test_nr].function)(&xpmem_args);




    ipcb_get_time(&start, "\n shm_writer:start: "); /* Start. */
    for (int i = 0; i < XPMEM_ROW_SIZE; i++)
        memcpy(str, buf[i], XPMEM_COL_SIZE);
    ipcb_get_time(&end, "\n shm_writer:end: ");  /* End. */
    
    ipcb_post_semaphore(mutex);

    // write(fdTime, &start.tv_sec, sizeof(long int));
    // write(fdTime, &start.tv_usec, sizeof(long int));
    
    printf("\nWriting Data into memory is done.\n");
    printf("Time in microseconds: %ld microseconds\n",
            ((end.tv_sec - start.tv_sec)*1000000L
           +end.tv_usec) - start.tv_usec
          ); // Added semicolon

    ipcb_unlink_semaphore("alaki");
    ipcb_close_semaphore(mutex);

    return 0;
}



/**
 * test_base - a simple test to share and attach
 * Description:
 *      Creates a share (initialized to a random value), calls a second process
 *	to attach to the shared address and increment its value.
 * Return Values:
 *	Success: 0
 *	Failure: -1
 */
int 
ipcb_test_base_one (test_args *xpmem_args)
{
	int i, ret=0, *data, expected;
	xpmem_segid_t segid;

	segid = make_share(&data, SHARE_SIZE);
	if (segid == -1) {
		perror("xpmem_make");
		xpmem_args->share[LOCK_INDEX] = 1;
		return -1;
	}

	printf("xpmem_proc1: mypid = %d\n", getpid());
	printf("xpmem_proc1: sharing %lld bytes\n", SHARE_SIZE);
	printf("xpmem_proc1: segid = %llx at %p\n\n", segid, data);

	ipcb_get_time(&start, "\ntest_base:start: "); /* Start. */
	/* Copy data to mmap share */
	sprintf(xpmem_args->share, "%llx", segid);

	/* Give control back to xpmem_master */
	xpmem_args->share[LOCK_INDEX] = 1;

	/* Wait for xpmem_proc2 to finish */
	lockf(xpmem_args->lock, F_LOCK, 0);
	lockf(xpmem_args->lock, F_ULOCK, 0);

	printf("xpmem_proc1: verifying data...");
	expected = (xpmem_args->add == 2 ? 2 : 1); /* Slightly hackish */
	for (i = 0; i < SHARE_INT_SIZE; i++) {
		if (*(data + i) != i + expected) {
			printf("xpmem_proc1: ***mismatch at %d: expected %d "
				"got %d\n", i, i + expected, *(data + i));
			ret = -1;
		}
	}
	printf("done\n");

	unmake_share(segid, data, SHARE_SIZE);

	return ret;
}