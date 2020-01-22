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

int main(int argc, char **argv)
{
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
        
        int mgrow = 1024;
        int gigrow = 1048576;
        long int two_gigrow = 2*gigrow;
        int col = 1024;
        unsigned long int mgsize = mgrow * col;
        unsigned long int gigsize = gigrow * col;
        unsigned long int two_gigsize = two_gigrow * col;
        
        char *data1 = calloc(bufferLength, sizeof(char));
        memset(data1, 'a', bufferLength);

        char *data2 = calloc(bufferLength, sizeof(char));
        memset(data2, 'b', bufferLength);

        char *data3 = calloc(bufferLength, sizeof(char));
        memset(data3, 'c', bufferLength);

        char *data4 = calloc(bufferLength, sizeof(char));
        memset(data4, 'd', bufferLength);
        
        // Build iovec structs
        bufferLength = gigsize;
        struct iovec local1[1];
        local1[0].iov_base = data1;
        local1[0].iov_len = bufferLength;

        struct iovec local2[1];
        local2[0].iov_base = data2;
        local2[0].iov_len = bufferLength;

        struct iovec local3[1];
        local3[0].iov_base = data3;
        local3[0].iov_len = bufferLength;

        struct iovec local4[1];
        local4[0].iov_base = data4;
        local4[0].iov_len = bufferLength;
        
        struct iovec remote[1];
        remote[0].iov_base = remotePtr;
        remote[0].iov_len = bufferLength;

        ssize_t nread1, nread2 = 0, nread3 = 0, nread4 = 0;

        // Call process_vm_readv - handle any error codes
        struct timespec start, finish;
        clock_gettime(CLOCK_REALTIME, &start);

        nread1 = process_vm_writev(pid, local1, 1, remote, 1, 0);
        nread2 = process_vm_writev(pid, local2, 1, remote, 1, 0);
        nread3 = process_vm_writev(pid, local3, 1, remote, 1, 0);
        nread4 = process_vm_writev(pid, local4, 1, remote, 1, 0);

        clock_gettime(CLOCK_REALTIME, &finish);

        psvm_error_handler(nread2);

        printf(" * Executed process_vm_ready, read %zd bytes.\n", nread2+nread1);

        long seconds = finish.tv_sec - start.tv_sec;
        long ns = finish.tv_nsec - start.tv_nsec;

        if (start.tv_nsec > finish.tv_nsec) { // clock underflow
                --seconds;
                ns += 1000000000;
        }
        printf("seconds without ns: %ld\n", seconds);
        printf("nanoseconds: %ld\n", ns);
        printf("total seconds: %e\n", (double)seconds + (double)ns/(double)1000000000);

        return 0;
}