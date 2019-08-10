#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include "ipcb_xpmem.h"

struct timeval start, end;

int 
main(int argc, char **argv) {
    // if (argc < 2) {
	// 	printf("In Writer: Usage: %s <test number>\n", argv[0]);
	// 	return -1;
	// }
	// char *name = atoi(argv[1]);

    int masterWriterlock, writerReaderLock;
    if ((masterWriterlock = open(MW_LOCK_FILE, O_RDWR | O_CREAT)) == -1) {
		perror("open xpmem.lock");
		return -1;
	}

    if ((writerReaderLock = open(RW_LOCK_FILE, O_RDWR | O_CREAT)) == -1) {
		perror("open xpmem.lock");
		return -1;
	}
    int id = ipcb_get_semaphore(KEY, 1, 0666 | IPC_CREAT);

    	union semun u;
    	u.val = 1;
   	ipcb_control_semaphore(id, 0, SETVAL, u);
	
	char *s = "abcdefghkdkdkdk";
    int l = strlen(s);
    ipcb_operate_semaphore(id, &decrease, 1);
    
	printf("   \n\n==== WRITER(SMALL): STARTS ====\n");

    for(int i = 0; i < l; ++i)
    {
        putchar(s[i]);
        fflush(stdout);
        putchar(s[i]);
        fflush(stdout);
    }
	lockf(masterWriterlock, F_ULOCK, 0);
    ipcb_operate_semaphore(id, &increase, 1);


	printf("   \n\n==== WRITER(SMALL): Ends ====\n");
    
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
	segid = make_share(&data, SHARE_SIZE);
	if (segid == -1) {
		perror("xpmem_make");
		return -1;
	}

	printf("xpmem_proc_writer: mypid = %d\n", getpid());
	printf("xpmem_proc_writer: sharing %d bytes\n", TMP_SHARE_SIZE);
	printf("xpmem_proc_writer: segid = %llx at %p\n\n", segid, data);
	
	sprintf(xpmem_args->share, "%llx", segid);

	ipcb_get_time(&start, "\ntest_base:start: "); /* Start. */
	
	/* Copy data to mmap share */
    for (int i = 0; i < XPMEM_ROW_SIZE; i++)
        memcpy((data + (i * XPMEM_COL_SIZE) ), xpmem_args->buf[i], 
				XPMEM_COL_SIZE);

	unmake_share(segid, data, SHARE_SIZE);

	return ret;
}