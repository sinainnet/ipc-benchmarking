
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: ipcb_xpmem_bench/ipcb_xpmem.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include "xpmem.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#include "ipcb_xpmem.h"

#define test_result(name, val1, val2) (((val1) == (val2)) ?	\
		printf("==== %s PASSED ====\n\n", (name)) :	\
		printf("==== %s FAILED ====\n\n", (name)))

const ull memSize = ( XPMEM_ROW_SIZE * XPMEM_COL_SIZE );

struct timeval start, end;

int test_base(test_args* t) { return 0; }
int ipcb_test_base_one(test_args* t) { return 0; }
int test_two_attach(test_args* t) { return 0; }
int test_two_shares(test_args* t) { return 0; }
int test_fork(test_args* t) { return 0; }


/*
 *  Lorem Ipsum
 */
int 
main(){
    pid_t p1, p2;
	char *share, test_nr[4];
	int i, fd, lock, status[2];
	test_args xpmem_args;

    ipcb_xpmem_arg_generator(memSize, &xpmem_args);

    printf("===============  %s STARTS  =============== \n", "MASTER");
    memset(xpmem_args.share, '\0', TMP_SHARE_SIZE);
    // lockf(lock, F_LOCK, 0);
    
    p1 = ipcb_fork();
    if (p1 == 0) {
        if (execl("./ipcb_xpmem_writer", "xpmem_proc1", test_nr, NULL) == -1) {
            perror("execl p1");
            return -1;
        }
    }
    p2 = ipcb_fork();
    if (p2 == 0) {
        if (execl("./ipcb_xpmem_reader", "xpmem_proc2", test_nr,
                NULL) == -1) {
            perror("execl p2");
            return -1;
        }
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
    
    test_result(xpmem_test[0].name, status[0], status[1]);


	munmap(share, TMP_SHARE_SIZE);
	return 0;
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


/*
 *  Lorem Ipsum
 */
int
ipcb_xpmem_arg_generator (ull memorySize, test_args* xpmem_args) {
    // printf("hre");
	if ((xpmem_args->fd = open(SHARE_FILE, O_RDWR)) == -1)
		return ipcb_print_error("open ipcb_xpmem.share");
    // else{printf("xpmem fd=%d", xpmem_args->fd);}

	if ((xpmem_args->lock = open(LOCK_FILE, O_RDWR)) == -1)
		return ipcb_print_error("open ipcb_xpmem.lock");

	xpmem_args->share = ipcb_map_memory_to_fd(memSize, xpmem_args->fd, 0); 
    return 1;
}






	// if ((xpmem_args.fd = open(SHARE_FILE, O_RDWR)) == -1) {
	// 	perror("open ipcb_xpmem.share");
	// 	return -1;
	// }
	// if ((xpmem_args.lock = open(LOCK_FILE, O_RDWR)) == -1) {
	// 	perror("open ipcb_xpmem.lock");
	// 	return -1;
	// }

	// xpmem_args.share = ipcb_map_memory_to_fd(memSize, xpmem_args.fd, 0); 