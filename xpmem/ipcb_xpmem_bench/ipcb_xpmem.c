
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: ipcb_xpmem_bench/ipcb_xpmem.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <xpmem.h>

#include "ipcb_xpmem.h"

const unsigned long int memSize = ( XPMEM_ROW_SIZE * XPMEM_COL_SIZE );


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


int 
main(){
    pid_t p1, p2;
	int i, fd, lock, status[2];
	char *share, test_nr[4];
	test_args xpmem_args;

	if ((xpmem_args.fd = open(SHARE_FILE, O_RDWR)) == -1) {
		perror("open ipcb_xpmem.share");
		return -1;
	}
	if ((xpmem_args.lock = open(LOCK_FILE, O_RDWR)) == -1) {
		perror("open ipcb_xpmem.lock");
		return -1;
	}

	xpmem_args.share = ipcb_map_memory_to_fd(memSize, xpmem_args.fd, 0); 

    printf(" ======== %s STARTS ======== \n", "MASTER");
    // memset(share, '\0', TMP_SHARE_SIZE);

    p1 = ipcb_fork();
    if (p1 == 0) {
        // if (execl("./xpmem_proc1", "xpmem_proc1", test_nr,
        //         NULL) == -1) {
        //     perror("execl p1");
        //     return -1;
        // }
    }

    p2 = ipcb_fork();
    if (p2 == 0) {
        // if (execl("./xpmem_proc2", "xpmem_proc2", test_nr,
        //         NULL) == -1) {
        //     perror("execl p2");
        //     return -1;
        // }
    }

    waitpid(p2, &status[1], 0);
    status[1] = WEXITSTATUS(status[1]);

    /* Release lock so xpmem_proc1 can exit */
    // lockf(lock, F_ULOCK, 0);
    waitpid(p1, &status[0], 0);
    status[0] = WEXITSTATUS(status[0]);
    
    
    printf("\nWriting Data into memory is done. %ld, %ld .\n", end.tv_sec, start.tv_sec);
    printf("Time in microseconds: %ld microseconds\n",
                ((end.tv_sec - start.tv_sec)*1000000L
            +end.tv_usec) - start.tv_usec
            ); // Added semicolon
    
    test_result(xpmem_test[i].name, status[0], status[1]);


	munmap(share, TMP_SHARE_SIZE);
	return 0;
}