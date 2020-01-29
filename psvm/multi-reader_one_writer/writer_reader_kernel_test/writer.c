#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/uio.h>
#include <sys/resource.h>
#include "../../header.h"

#define THREADS		40
#define data_len        eight_gig_size

int main(int argc, char **argv) {

        // Changing the process scheduling queue into 
        // real-time and set its priority using <sched.h>.
        set_cpu_scheduler(2,99);

        char *data = calloc(eight_gig_row, col);
        // Build iovec structs
        int local_iov_num = THREADS;
        int data_leng = data_len/local_iov_num;

        for (int i = 0; i < local_iov_num; i++)
        {
                memset(data + (i*data_leng), 'a' + i, data_leng);
	}

        struct iovec local[1];
        local[0].iov_base = data;
        local[0].iov_len = data_len;
        
        struct iovec remote[1];
        remote[0].iov_base = calloc(gig_row, col);
        remote[0].iov_len = gig_size;

        
        printf("writer: sudo ./reader %d %p %lu\n", \
                getpid(), data, data_len);

        ssize_t nread = process_vm_writev(getpid(), \
                local, THREADS, remote, 1, 0);

        printf("readers just read data. I'm done.\n");
        return 0;
}
