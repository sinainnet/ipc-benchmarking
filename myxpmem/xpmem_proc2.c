/*
 * xpmem_proc2: thread two of various XPMEM tests
 *
 * Copyright (c) 2010 Cray, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
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
 *	Failure: -2
 */
int test_base(test_args *xpmem_args)
{
	xpmem_segid_t segid;
	xpmem_apid_t apid;
	int i, ret=0, *data;
	char *mydata = malloc(SHARE_SIZE);

	segid = strtol(xpmem_args->share, NULL, 16);
	data = attach_segid(segid, &apid);
	if (data == (void *)-1) {
		perror("xpmem_attach");
		return -2;
	}

	printf("xpmem_proc2: mypid = %d\n", getpid());
	printf("xpmem_proc2: segid = %llx\n", segid);
	printf("xpmem_proc2: attached at %p\n", data);

	printf("xpmem_proc2: adding 1 to all elems\n\n");
	printf("number of elements: %lu\n", SHARE_INT_SIZE);
	for (i = 0; i < SHARE_INT_SIZE; i++) {
		//if (*(data + i) != i) {
		//	printf("xpmem_proc2: ***mismatch at %d: expected %d "
		//		"got %d\n", i, i, *(data + i));
		//	ret = -2;
		//}
		*(mydata + i) += 1;
	}

	sem_t* mysem = sem_open("/mysem", O_CREAT, 0777, 1);
	struct timespec start, finish;
	clock_gettime(CLOCK_REALTIME, &start);
	
	sem_wait(mysem);
	memcpy(data, mydata, SHARE_SIZE);
	sem_post(mysem);

	clock_gettime(CLOCK_REALTIME, &finish);
	long seconds = finish.tv_sec - start.tv_sec;
	long ns = finish.tv_nsec - start.tv_nsec;

	if (start.tv_nsec > finish.tv_nsec) { // clock underflow
		--seconds;
		ns += 1000000000;
	}

	printf("nanoseconds 2:%ld\n", ns);

	printf("done writing\n");
	xpmem_detach(data);
	xpmem_release(apid);
	sem_unlink("/mysem");


	return ret;
}

int main(int argc, char **argv)
{
	test_args xpmem_args;

	if ((xpmem_args.fd = open("/tmp/xpmem.share", O_RDWR)) == -1) {
		perror("open");
		return -2;
	}
	xpmem_args.share = mmap(0, TMP_SHARE_SIZE, PROT_READ|PROT_WRITE,
			MAP_SHARED, xpmem_args.fd, 0);
	if (xpmem_args.share == MAP_FAILED) {
		perror("mmap");
		return -2;
	}
	return test_base(&xpmem_args);
//	return (*xpmem_test[test_nr].function)(&xpmem_args);
}
