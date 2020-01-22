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
#include <string.h>
#include <sched.h>

void set_cpu_scheduler(int cpu_no, int priority) {
        cpu_set_t set;
        CPU_ZERO(&set);
        CPU_SET(cpu_no, &set);
        if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
        {
                printf("setaffinity error");
                exit(0);
        }
        struct sched_param sch_p;
        sch_p.sched_priority = priority;
        int re = sched_setscheduler(getpid(), SCHED_RR, &sch_p);
        if(re < 0) {
                printf("sched_setscheduler returned error code %d.\n", re);
                exit(0);
        }
        return;
}

void psvm_error_handler(ssize_t nread2) {
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
                exit(1);
        }
}

int main(int argc, char **argv) {

        int mgrow = 1024;
        int gigrow = 1048576;
        long int two_gigrow = 2*gigrow;
        int col = 1024;
        unsigned long int mgsize = mgrow * col;
        unsigned long int gigsize = gigrow * col;
        unsigned long int two_gigsize = two_gigrow * col;
        
        // Changing the process scheduling queue into real-time and set its priority using <sched.h>.
        set_cpu_scheduler(0,99);
        
        char *data = calloc(gigrow, col);
        memset(data, 'a', gigsize);
        printf("writer: %d %p %lu \n", getpid(), data, gigsize);

        // Build iovec structs
        struct iovec local[1];
        local[0].iov_base = data;
        local[0].iov_len = gigsize;
        
        struct iovec remote[1];
        remote[0].iov_base = calloc(gigsize, sizeof(char));
        remote[0].iov_len = gigsize;

        /** in case we wanna use chrt command instead of sched.h library */
	/** printf("press any key to continue.\n"); */
	/** getchar(); */
        
        printf("writer: %d %p %lu \n", getpid(), local[0].iov_base, gigsize);

        // Call process_vm_readv - handle any error codes
        ssize_t nread2 = process_vm_writev(getpid(), local, 10, remote, 1, 0);

        psvm_error_handler(nread2);
        
        printf("readers wrote their messages. I'm done.\n");
        return 0;
}
