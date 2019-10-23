#define _GNU_SOURCE
#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <limits.h>
#include <sys/resource.h>
#include "sina.h"


int ipcb_test_base_one(test_args* t) { return 0; }

int main(int argc, char **argv) {
        if (argc < 3) {
                printf("usage: %s <pid> <mem address> [len]\n", argv[0]);
                printf("  <pid> - PID of process to target\n");
                printf("  <mem> - Memory address to target\n");
                printf("  [len] - Length (in bytes) to dump\n");
                return -1;
        }

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
        long long ss = 2147479552;
        long long s = 2*ss;
        struct iovec remote[5];
        remote[0].iov_base = remotePtr;
        remote[0].iov_len = bufferLength;
        // remote[1].iov_base = remotePtr + 2147479552;
        // remote[1].iov_len = bufferLength - 2147479552;
        // remote[2].iov_base = remotePtr + s;
        // remote[2].iov_len = bufferLength - s;
        // remote[3].iov_base = remotePtr + 3*ss;
        // remote[3].iov_len = bufferLength - 3*ss;
        // remote[4].iov_base = remotePtr + 4*ss;
        // remote[4].iov_len = bufferLength - 4*ss;
        // Call process_vm_readv - handle any error codes
        // int id_srt = ipcb_get_semaphore(shared_sem_key, 1, 0666);
        struct timespec start, finish;
        clock_gettime(CLOCK_REALTIME, &start);

        // ipcb_operate_semaphore(id_srt, &decrease, 1);

        ssize_t nread = process_vm_readv(pid, local, 2, &remote[0], 1, 0);
        // ssize_t nread2 = process_vm_readv(pid, local, 2, &remote[1], 1, 0);
        // ssize_t nread3 = process_vm_readv(pid, local, 2, &remote[2], 1, 0);
        // ssize_t nread4 = process_vm_readv(pid, local, 2, &remote[3], 1, 0);
        // ssize_t nread5 = process_vm_readv(pid, local, 2, &remote[4], 1, 0);

        // ipcb_operate_semaphore(id_srt, &increase, 1);

        clock_gettime(CLOCK_REALTIME, &finish);

        if (nread < 0) {
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
        }

       
        
        
        printf(" * Executed process_vm_ready, read %d:%llu bytes.\n", 2, nread /*+ nread2 + nread3+ nread4+ nread5*/);
        // printf("%s\n", (char *)(local[0].iov_base));

        long seconds = finish.tv_sec - start.tv_sec;
        long ns = finish.tv_nsec - start.tv_nsec;

        if (start.tv_nsec > finish.tv_nsec) { // clock underflow
                printf("overflowed.\n");
                --seconds;
                ns += 1000000000;
        }
        printf("seconds without ns: %ld\n", seconds);
        printf("nanoseconds: %ld\n", ns);
        printf("total seconds: %e\n", (double)seconds + (double)ns/(double)1000000000);

        int id_wrt = ipcb_get_semaphore(shared_wrt_key, 1, 0666);
        ipcb_operate_semaphore(id_wrt, &increase, 1);

        return 0;
}