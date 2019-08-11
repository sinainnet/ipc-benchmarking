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
    
	int id = ipcb_get_semaphore(KEY, 1, 0666 | IPC_CREAT);
   	ipcb_control_semaphore(id, 0, SETVAL, u);
    ipcb_operate_semaphore(id, &decrease, 1);

	xpmem_args.buf = ipcb_fake_data_generator(XPMEM_ROW_SIZE, PAGE_SIZE);

	ipcb_test_base_one(&xpmem_args);
	// ipcb_test_base_two(&xpmem_args);

    ipcb_operate_semaphore(id, &increase, 1);

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
	
	printf("xpmem_proc_writer: mypid = %d\n", getpid());
	printf("xpmem_proc_writer: sharing %d bytes\n", TMP_SHARE_SIZE);
	printf("xpmem_proc_writer: segid = %llx at %p\n\n", segid, data);
	

	ipcb_get_time(&start, "\ntest_base:start: "); /* Start. */
	
	/* Copy data to mmap share */
    for (i = 0; i < XPMEM_ROW_SIZE; i++){
		// if (i > 65000)
			// printf(" %d ", i);
        memcpy((data), xpmem_args->buf[i],
				PAGE_SIZE);
		// memcpy((data + (i * PAGE_SIZE)), xpmem_args->buf[i], 
		// PAGE_SIZE);
	}
	// printf("%s", xpmem_args->buf[i-1]);

		// memcpy((data + (i * PAGE_SIZE)), xpmem_args->buf[i], 
		// 		PAGE_SIZE);

	printf("   \n\n==== WRITER: Ends ====\n");


	/* Give control back to xpmem_master */
	printf("give control back to reader\n");
	xpmem_args->share[LOCK_INDEX] = '1';

	printf("writer i want to lock file.\n");
	int tmp;
	/* Wait for xpmem_proc1 to finish processing */
	while ((tmp = strtol(xpmem_args->share+LOCK_INDEX, NULL, 16)) == 1) { 
		usleep(100); }
	printf("writer i finally lockd and unlocked the file\n");

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
	printf("give control back to reader\n");
	xpmem_args->share[LOCK_INDEX] = '1';

	printf("writer i want to lock file.\n");
	int tmp;
	/* Wait for xpmem_proc1 to finish processing */
	while ((tmp = strtol(xpmem_args->share+LOCK_INDEX, NULL, 16)) == 1) { 
		usleep(100); }
	printf("writer i finally lockd and unlocked the file\n");

	unmake_share(segid, data, SHARE_SIZE);
	return 0;
}