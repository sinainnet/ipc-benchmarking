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
#include "helper.h"

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
        if (argc < 3) {
                printf("usage: %s <pid> <mem address> [len]\n", argv[0]);
                printf("  <pid> - PID of process to target\n");
                printf("  <mem> - Memory address to target\n");
                printf("  [len] - Length (in bytes) to dump\n");
                return -1;
        }

        set_cpu_scheduler(1, 99);

        // PARSE CLI ARGS
        pid_t pid = strtol(argv[1], NULL, 10);
        printf(" * Launching with a target PID of: %d\n", pid);

        void *remotePtr = (void *)strtol(argv[2], NULL, 0);
        printf(" * Launching with a target address of 0x%llx\n", (long long unsigned)remotePtr);

        size_t bufferLength = (argc > 3) ? strtol(argv[3], NULL, 10) : 20;
        printf(" * Launching with a buffer size of %lu bytes.\n", bufferLength);

        // Build iovec structs
        struct iovec local[1];
        local[0].iov_base = calloc(bufferLength, sizeof(char));
        local[0].iov_len = bufferLength;

        struct iovec remote[1];
        remote[0].iov_base = remotePtr;
        remote[0].iov_len = bufferLength;

        // Call process_vm_readv - handle any error codes
        int id_srt = ipcb_get_semaphore(shared_sem_key, 1, 0666);

        struct timespec start, finish;
        clock_gettime(CLOCK_REALTIME, &start);
        
        ipcb_operate_semaphore(id_srt, &decrease, 1);
        ssize_t nread2 = process_vm_readv(pid, local, 2, remote, 1, 0);
        ipcb_operate_semaphore(id_srt, &increase, 1);

        clock_gettime(CLOCK_REALTIME, &finish);

        psvm_error_handler(nread2);

        printf(" * Process_vm_readv Executed, read %zd bytes.\n", nread2);
        
        long seconds = finish.tv_sec - start.tv_sec;
        long ns = finish.tv_nsec - start.tv_nsec;

        if (start.tv_nsec > finish.tv_nsec) { // clock underflow
                --seconds;
                ns += 1000000000;
        }
        printf("seconds without ns: %ld\n", seconds);
        printf("nanoseconds: %ld\n", ns);
        printf("total seconds: %e\n", (double)seconds + (double)ns/(double)1000000000);
        
//        int id_wrt = ipcb_get_semaphore(shared_wrt_key, 1, 0666);
//        ipcb_operate_semaphore(id_wrt, &increase, 1);
        
        return 0;
}
