#define _GNU_SOURCE
#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <unistd.h>
#include <sched.h>

int main(int argc, char **argv) {

        cpu_set_t set;
        CPU_ZERO(&set);
        CPU_SET(0, &set);
        if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
        {
                printf("setaffinity error");
                return 0;
        }
        int mgrow = 1024;
        int gigrow = 1048576;
        long int two_gigrow = 2*gigrow;
        int col = 1024;
        unsigned long int mgsize = mgrow * col;
        unsigned long int gigsize = gigrow * col;
        unsigned long int two_gigsize = two_gigrow * col;

        // Build iovec structs
        size_t bufferLength = two_gigsize;
        struct iovec local[1];
        local[0].iov_base = calloc(bufferLength, sizeof(char));
        local[0].iov_len = bufferLength;

        struct iovec remote[1];
        remote[0].iov_base = calloc(bufferLength, sizeof(char));;
        remote[0].iov_len = bufferLength;

	// changing the process scheduling queue into real-time and set its priority using <sched.h>.
	struct sched_param sch_p;
	sch_p.sched_priority = 99;
	int re = sched_setscheduler(getpid(), SCHED_RR, &sch_p);
	if(re >= 0)
	        printf("reader: %d %p %lu \n", getpid(), local[0].iov_base, two_gigsize);
	else {
		printf("sched_setscheduler returned error code.\n");
		exit(0);
	}

	// in case we wanna use chrt command instead of sched.h library
	// printf("press any key to continue.\n");
	// getchar();

        // Call process_vm_readv - handle any error codes
        ssize_t nread2 = process_vm_readv(getpid(), local, 2, remote, 1, 0);

        if (nread2 < 0) {
                switch (errno) {
                        case EINVAL:
                                printf("ERROR: INVALID ARGUMENTS.\n");
                                break;
                        case EFAULT:
                                printf("ERROR: UNABLE TO ACCESS TARGET MEMORY ADDRESS.\n");
                                break;
                        case ENOMEM:
                                printf("ERROR: UNABLE TO ALLOCATE MEMORY.\n");
                                break;
                        case EPERM:
                                printf("ERROR: INSUFFICIENT PRIVILEGES TO TARGET PROCESS.\n");
                                break;
                        case ESRCH:
                                printf("ERROR: PROCESS DOES NOT EXIST.\n");
                                break;
                        default:
                                printf("ERROR: AN UNKNOWN ERROR HAS OCCURRED.\n");
                }

                return -1;
        }

        while (1)
	{
		printf("z\n");
	}

        return 0;
}
