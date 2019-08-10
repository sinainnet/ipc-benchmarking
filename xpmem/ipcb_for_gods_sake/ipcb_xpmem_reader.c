#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include "ipcb_xpmem.h"

extern struct timeval start, end;


int 
main(int argc, char **argv) {
    // if (argc < 2) {
	// 	printf("In Writer: Usage: %s <test number>\n", argv[0]);
	//   	return -1;
	// }
	// char *name = atoi(argv[1]);
    int masterWriterlock, writerReaderLock;
    int lock;
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
    
    lockf(writerReaderLock, F_LOCK, 0);
    // printf("I got lock WR \n");
    ipcb_operate_semaphore(id, &decrease, 1);
    lockf(masterWriterlock, F_LOCK, 0);
    // printf("I got lock MW \n");
    
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
    ipcb_operate_semaphore(id, &increase, 1);
    
	lockf(masterWriterlock, F_ULOCK, 0);
	lockf(writerReaderLock, F_ULOCK, 0);
	printf("   \n\n==== Reader(Capital): Ends ====\n");
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