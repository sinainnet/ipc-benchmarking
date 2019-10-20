/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: xpmem/ipcb_sem_bench/ipcb_xpmem_writer.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include "ipcb_xpmem.h"

struct timeval start, end;

// 
int 
main(int argc, char **argv) {

	test_args xpmem_args;
	union semun u;
	u.val = 1;
	
	ipcb_xpmem_arg_generator(TMP_SHARE_SIZE, &xpmem_args);
	xpmem_args.buf = ipcb_fake_data_generator(XPMEM_ROW_SIZE, PAGE_SIZE);
	
	// check what is in the buf
	// for (int i = 0; i < XPMEM_ROW_SIZE; i++)
	// 	printf("%s", xpmem_args.buf[i]);
	
	ipcb_test_base_one(&xpmem_args);
	// ipcb_test_base_two(&xpmem_args);

    return 0;
}



/**
 * test_base - a simple test to share and attach
 * Description:
 *      Creates a share (initialized to a random value), calls a 
 * 			second process
 *	to attach to the shared address and increment its value.
 * Return Values:
 *	Success: 0
 *	Failure: -1
 */
int 
ipcb_test_base_one (test_args *xpmem_args) {
	int i, ret=0, *data, expected;
	xpmem_segid_t segid;
	printf("sharesize = %lld:%lld\n", SHARE_SIZE, PAGE_SIZE);
	segid = make_share(&data, SHARE_SIZE);
	if (segid == -1) {
		perror("xpmem_make");
		return -1;
	}
	sprintf(xpmem_args->share, "%llx", segid);

	printf("   \n\n==== WRITER: STARTS ====\n");
	
	printf("1- xpmem_proc_writer: mypid = %d\n", getpid());
	printf("2- xpmem_proc_writer: sharing %d bytes\n", TMP_SHARE_SIZE);
	printf("3- xpmem_proc_writer: segid = %llx at %p\n\n", segid, data);
	
	ipcb_get_time(&start, "\ntest_base_one:start: "); /* Start. */
	
	/* Copy data to mmap share */
    for (i = 0; i < XPMEM_ROW_SIZE; i++){
        memcpy((data), xpmem_args->buf[i],
				PAGE_SIZE);
	}

	printf("   \n\n==== WRITER: Ends ====\n");

	// check what has been wrote on data.
	printf("%ls\n\n", data);

	/* Give control back to xpmem_master */
	// printf("give control back to reader\n");
	// xpmem_args->share[LOCK_INDEX] = '1';

	// printf("writer i want to lock file.\n");

	int id = ipcb_get_semaphore(shared_sem_key, 1, 0666);
    ipcb_operate_semaphore(id, &increase, 1);


	int id_2 = ipcb_get_semaphore(shared_ums_key, 1, 0666);
    ipcb_operate_semaphore(id_2, &decrease, 1);
	ipcb_operate_semaphore(id_2, &increase, 1);

	printf("writer: I'm gonna unmake what i've shared before.\n");

	unmake_share(segid, data, SHARE_SIZE);
	return ret;
}


int ipcb_test_base_two(test_args *xpmem_args) {
	int i, ret=0, *data, expected;
	xpmem_segid_t segid;
	printf("sharesize = %lld:%lld\n", SHARE_SIZE, PAGE_SIZE);
	segid = make_share(&data, SHARE_SIZE);
	if (segid == -1) {
		perror("xpmem_make");
		return -1;
	}
	/* Copy data to mmap share */
	sprintf(xpmem_args->share, "%llx", segid);

	char *s = "abcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdcdefghkdkdkdk";
    int l = strlen(s);
    
	printf("   \n\n==== WRITER(SMALL): STARTS ====\n");

    for(int i = 0; i < l; ++i)
    {
        putchar(s[i]);
        fflush(stdout);
        putchar(s[i]);
        fflush(stdout);
    }

	printf("   \n\n==== WRITER(SMALL): Ends ====\n");

	/* Give control back to xpmem_master */
	// printf("give control back to reader\n");
	// xpmem_args->share[LOCK_INDEX] = '1';

	printf("writer i want to lock file.\n");
	int tmp;
	/* Wait for xpmem_proc1 to finish processing */
	while ((tmp = strtol(xpmem_args->share+LOCK_INDEX, NULL, 16)) == 1) { 
		usleep(100); }
	printf("writer i finally lockd and unlocked the file\n");

	unmake_share(segid, data, SHARE_SIZE);
	return 0;
}