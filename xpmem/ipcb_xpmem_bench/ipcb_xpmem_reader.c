
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: ipcb_xpmem_bench/ipcb_xpmem_reader.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <xpmem.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "ipcb_xpmem.h"

const ull memSize = ( XPMEM_ROW_SIZE * XPMEM_COL_SIZE );
struct timeval start, end;

int test_base(test_args* t) { return 0; }
int test_two_attach(test_args* t) { return 0; }
int test_two_shares(test_args* t) { return 0; }
int test_fork(test_args* t) { return 0; }

/* */
/*
 *  Lorem Ipsum
 */
int 
main(int argc, char **argv) {
    char *str;
    char **buf;
    pid_t otherChildId;
	test_args xpmem_args;
    struct timeval start, end;
    int fd, fdSync, fdTime, test_nr;

    if (argc < 2) {
		printf("In Reader: Usage: %s <test number>\n", argv[0]);
		return -1;
	}
	test_nr = atoi(argv[1]);
	
	ipcb_xpmem_arg_generator(memSize, &xpmem_args);

    
	xpmem_args.buf = ipcb_empty_allocator(1, 32);
    
    sem_t *mutex = ipcb_open_semaphore_other();
	printf("   \n\n==== READER: %s Going to Wait ====\n",  xpmem_test[0].name);

	printf("   \n\n==== READER: %s STARTS ====\n",  xpmem_test[0].name);
    ipcb_wait_semaphore(mutex);

    int ret = (*xpmem_test[test_nr].function)(&xpmem_args);

    ipcb_post_semaphore(mutex);
    ipcb_close_semaphore(mutex);
    ipcb_destroy_semaphore(mutex);

    return ret;
}


/**
 * test_base - a simple test to share and attach
 * Description:
 *      Creates a share (initialized to a random value), calls a second process
 *	to attach to the shared address and increment its value.
 * Return Values:
 *	Success: 0
 *	Failure: -2
 */
int 
ipcb_test_base_one (test_args *xpmem_args) {
	xpmem_apid_t apid;
	xpmem_segid_t segid;
	int i, ret=0, *data;

	segid = strtol(xpmem_args->share, NULL, 16);
	printf("%s\n", xpmem_args->share);
	printf("xpmem_proc_reader: segid = %llx\n", segid);

	data = attach_segid(segid, &apid);
	if (data == (void *)-1) {
		perror("xpmem_attach");
		return -2;
	}

	printf("xpmem_proc_reader: mypid = %d\n", getpid());
	printf("xpmem_proc_reader: attached at %p\n", data);

	/* Copy data to mmap share */
    for (int i = 0; i < XPMEM_ROW_SIZE; i++)
        memcpy(xpmem_args->buf[i], (data + (i * XPMEM_COL_SIZE) ),  
				XPMEM_COL_SIZE);
    ipcb_get_time(&end, "\ntest_base_one:end: "); /* End. */

	xpmem_detach(data);
	xpmem_release(apid);
	return ret;
}


/*
 *  Lorem Ipsum
 */
int
ipcb_xpmem_arg_generator (ull memorySize, test_args* xpmem_args) {

	if ((xpmem_args->fd = open(SHARE_FILE, O_RDWR)) == -1)
		return ipcb_print_error("open ipcb_xpmem.share");

	if ((xpmem_args->lock = open(LOCK_FILE, O_RDWR)) == -1)
		return ipcb_print_error("open ipcb_xpmem.lock");

	xpmem_args->share = ipcb_map_memory_to_fd(TMP_SHARE_SIZE, xpmem_args->fd, 0); 
    return 1;
}


/*
 *  Lorem Ipsum
 */
char*
ipcb_map_memory_to_fd (unsigned long long memorySize, int fd, off_t offset) {
    char* str;

    str = mmap(NULL, memorySize, PROT_READ|PROT_WRITE, 
                MAP_SHARED, fd, offset);
    if (MAP_FAILED == str) 
        ipcb_print_error("ipcb_master:ipcb_map_memory_to_fd: mmap");    
    return str;
}