/*
 * xpmem_proc1: process one capable of running various XPMEM tests
 *
 * Copyright (c) 2010 Cray, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <xpmem.h>
#include "xpmem_test.h"


/**
 * test_base - a simple test to share and attach
 * Description:
 *      Creates a share (initialized to a random value), calls a second process
 *	to attach to the shared address and increment its value.
 * Return Values:
 *	Success: 0
 *	Failure: -1
 */
int test_base(test_args *xpmem_args)
{
	int i, ret=0, *data, expected;
	xpmem_segid_t segid;
	char *internal_data = malloc(SHARE_SIZE);

	segid = make_share(&data, SHARE_SIZE);
	if (segid == -1) {
		perror("xpmem_make");
		xpmem_args->share[LOCK_INDEX] = 1;
		return -1;
	}

	printf("xpmem_proc1: mypid = %d\n", getpid());
	printf("xpmem_proc1: sharing %ld bytes\n", SHARE_SIZE);
	printf("xpmem_proc1: segid = %llx at %p\n\n", segid, data);

	/* Copy data to mmap share */
	sprintf(xpmem_args->share, "%llx", segid);

	printf("xpmem_proc1: getting data...\n");
	sem_unlink("/mysem");
	sem_t* mysem = sem_open("/mysem", O_CREAT, 0777, 1);


	struct timespec start, finish;
	clock_gettime(CLOCK_REALTIME, &start);
	
	sem_wait(mysem);
	memcpy(internal_data, data, SHARE_SIZE);
	sem_post(mysem);

	clock_gettime(CLOCK_REALTIME, &finish);
	printf("done data copy\n");
	long seconds = finish.tv_sec - start.tv_sec;
	long ns = finish.tv_nsec - start.tv_nsec;

	if (start.tv_nsec > finish.tv_nsec) { // clock underflow
		--seconds;
		ns += 1000000000;
	}

	printf("nanoseconds 1:%ld\n", ns);
	
	getchar();
	sem_unlink("/mysem");

	unmake_share(segid, data, SHARE_SIZE);

	return ret;
}

int main(int argc, char **argv)
{
	test_args xpmem_args;
	int test_nr;

	if ((xpmem_args.fd = open("/tmp/xpmem.share", O_RDWR|O_CREAT)) == -1) {
		perror("open xpmem.share");
		return -1;
	}
	if ((xpmem_args.lock = open("/tmp/xpmem.lock", O_RDWR|O_CREAT)) == -1) {
		perror("open xpmem.lock");
		return -1;
	}
	xpmem_args.share = mmap(0, TMP_SHARE_SIZE, PROT_READ|PROT_WRITE,
			MAP_SHARED, xpmem_args.fd, 0);
	if (xpmem_args.share == MAP_FAILED) {
		perror("mmap");
		return -1;
	}
	
	ftruncate(xpmem_args.fd, TMP_SHARE_SIZE);

	test_base(&xpmem_args);

}
