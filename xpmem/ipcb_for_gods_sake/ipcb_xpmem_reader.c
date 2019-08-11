#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include "ipcb_xpmem.h"

extern struct timeval start, end;
// 
int 
main(int argc, char **argv) {

	int tmp;
    union semun u;
	test_args xpmem_args;

    u.val = 1;

	ipcb_xpmem_arg_generator(TMP_SHARE_SIZE, &xpmem_args);

	xpmem_args.emptyAlloc = ipcb_fake_data_generator(XPMEM_ROW_SIZE, PAGE_SIZE);

	/* Wait for xpmem_proc1 to finish processing */
	while ((tmp = strtol(xpmem_args.share+LOCK_INDEX, NULL, 16)) == 0) { 
		usleep(100); }

    int id = ipcb_get_semaphore(KEY, 1, 0666 | IPC_CREAT);
   	ipcb_control_semaphore(id, 0, SETVAL, u);    
    ipcb_operate_semaphore(id, &decrease, 1);

	ipcb_test_base_one(&xpmem_args);
	// ipcb_test_base_two(&xpmem_args);

    ipcb_operate_semaphore(id, &increase, 1);
	
	xpmem_args.share[LOCK_INDEX] = '0';
    return 0;
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
	printf("Reader sharesize = %lld:%lld\n", SHARE_SIZE, PAGE_SIZE);

	segid = strtol(xpmem_args->share, NULL, 16);
	printf("%s\n", xpmem_args->share);
	printf("xpmem_proc_reader: segid = %llx\n", segid);

	data = attach_segid(segid, &apid);
	if (data == (void *)-1) {
		perror("xpmem_attach");
		return -2;
	}

	printf("   \n\n==== Reader: STARTS ====\n");
	
	printf("xpmem_proc_reader: mypid = %d\n", getpid());
	printf("xpmem_proc_reader: attached at %p\n", data);

	/* Copy data to mmap share */
    for (i = 0; i < XPMEM_ROW_SIZE; i++)
        memcpy(xpmem_args->emptyAlloc[i], data,  
				PAGE_SIZE);
	// printf("%s", xpmem_args->emptyAlloc[i-1]);

    
	ipcb_get_time(&end, "\ntest_base_one:end: "); /* End. */

	printf("   \n\n==== Reader: Ends ====\n");

	xpmem_detach(data);
	xpmem_release(apid);
	return ret;
}


int ipcb_test_base_two(test_args* xpmem_args) { 
	xpmem_segid_t segid;
	xpmem_apid_t apid;
	int i, ret=0, *data;

	segid = strtol(xpmem_args->share, NULL, 16);
	data = attach_segid(segid, &apid);
	if (data == (void *)-1) {
		perror("xpmem_attach");
		return -2;
	}

    char *s = "ABCDEFGH";
    int l = strlen(s);
    
	printf("   \n\n==== Reader(Capital): STARTS ====\n");
    for(int i = 0; i < l; ++i)
    {
        putchar(s[i]);
        fflush(stdout);
        putchar(s[i]);
        fflush(stdout);
    }
    
	printf("   \n\n==== Reader(Capital): Ends ====\n");
	
	xpmem_detach(data);
	xpmem_release(apid);
	
	return 0; 
}
