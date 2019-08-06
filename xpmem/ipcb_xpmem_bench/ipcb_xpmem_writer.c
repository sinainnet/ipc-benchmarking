
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
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include "ipcb_xpmem.h"

const ull memSize = ( XPMEM_ROW_SIZE * XPMEM_COL_SIZE );
struct timeval start, end;

int test_base(test_args* t) { return 0; }
int test_two_attach(test_args* t) { return 0; }
int test_two_shares(test_args* t) { return 0; }
int test_fork(test_args* t) { return 0; }


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
	
	sem_t *mutex = ipcb_open_semaphore();
	ipcb_wait_semaphore(mutex);
    if (argc < 2) {
		printf("In Writer: Usage: %s <test number>\n", argv[0]);
		return -1;
	}
	test_nr = atoi(argv[1]);
	
	ipcb_xpmem_arg_generator(memSize, &xpmem_args);
    
	// memset(xpmem_args.share, '\0', TMP_SHARE_SIZE);
    
	xpmem_args.buf = ipcb_fake_data_generator(XPMEM_ROW_SIZE, XPMEM_COL_SIZE);
	// char* upper_name = ipcb_upper_string(xpmem_test[0].name);
    printf("   \n\n==== %s STARTS ====\n",  xpmem_test[0].name);

    int ret = (*xpmem_test[test_nr].function)(&xpmem_args);

	ipcb_post_semaphore(mutex);
	printf("\nSemaphore Posted.\n");
	ipcb_unlink_semaphore("alaki");
	ipcb_close_semaphore(mutex);
	return ret;
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
ipcb_test_base_one (test_args *xpmem_args) {
	int i, ret=0, *data, expected;
	xpmem_segid_t segid;
	segid = make_share(&data, SHARE_SIZE);
	if (segid == -1) {
		perror("xpmem_make");
		// xpmem_args->share[LOCK_INDEX] = 1;
		return -1;
	}

	printf("xpmem_proc_writer: mypid = %d\n", getpid());
	printf("xpmem_proc_writer: sharing %lld bytes\n", TMP_SHARE_SIZE);
	printf("xpmem_proc_writer: segid = %llx at %p\n\n", segid, data);

	ipcb_get_time(&start, "\ntest_base:start: "); /* Start. */
	/* Copy data to mmap share */
    for (int i = 0; i < XPMEM_ROW_SIZE; i++)
        memcpy((data + (i * XPMEM_COL_SIZE) ), xpmem_args->buf[i], 
				XPMEM_COL_SIZE);
	sprintf(xpmem_args->share, "%llx", segid);

	// printf("i wrote it.it is = %lld:%s\n", segid, xpmem_args->share);
	
	/* Give control back to xpmem_master */
	// xpmem_args->share[LOCK_INDEX] = 1;

	// printf("\nIn Writer: testbase one done.\n");
	unmake_share(segid, data, SHARE_SIZE);

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

	xpmem_args->share = ipcb_map_memory_to_fd(memSize, xpmem_args->fd, 0); 
    return 1;
}


/*
 *  Lorem Ipsum
 */
char*
ipcb_map_memory_to_fd (unsigned long long memorySize, int fd, off_t offset) {
    char* str;

    str = mmap(NULL, 2ull * memorySize, PROT_READ|PROT_WRITE, 
                MAP_SHARED, fd, offset);
    if (MAP_FAILED == str) 
        ipcb_print_error("ipcb_master:ipcb_map_memory_to_fd: mmap");    
    return str;
}

