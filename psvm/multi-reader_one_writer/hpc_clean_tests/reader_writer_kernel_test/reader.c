#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/uio.h>
#include <sys/resource.h>

#include "../../../header.h"

#define THREADS		1
#define data_len    two_gig_size

int main(int argc, char **argv) {
        // Changing the process scheduling queue into 
        // real-time and set its priority using <sched.h>.
        set_cpu_scheduler(1,99);

        char    *data = calloc(two_gig_row, col),
                *executor_data = calloc(512, sizeof(char));

        struct iovec local[1];
        local[0].iov_base = data;
        local[0].iov_len = data_len;

        struct iovec remote[1];
        remote[0].iov_base = calloc(data_len, col);
        remote[0].iov_len = data_len;

        sprintf(executor_data, "./writer %d %p %llu", getpid(), data, data_len);
        
        FILE *file_res = fopen(middleware, "w+");
        fputs(executor_data, file_res);

        printf("reader: sudo ./writer %d %p %llu\n", \
                getpid(), data, data_len);

        fclose(file_res);        

        ssize_t nread = process_vm_readv(getpid(), local, THREADS, remote, 1, 0);

        printf("writer just wrote data. I'm done.\n");
        return 0;
}
