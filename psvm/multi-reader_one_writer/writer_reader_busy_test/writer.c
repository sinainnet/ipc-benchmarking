#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/uio.h>
#include <sys/resource.h>
#include "../../header.h"

#define THREADS		80
#define data_len        fourteen_gig_size


int main(int argc, char **argv) {

        // Changing the process scheduling queue into 
        // real-time and set its priority using <sched.h>.
        set_cpu_scheduler(2,99);

        char *data = calloc(fourteen_gig_row, col);
        
        // Build iovec structs
        int local_iov_num = THREADS;
        long long int data_leng = data_len/local_iov_num;
        for (long long int i = 0; i < local_iov_num; i++)
        {
                memset(data + (i*data_leng), 'a' + i, data_leng);
	}

        struct iovec local[1];
        local[0].iov_base = data;
        local[0].iov_len = data_len;
        
        struct iovec remote[1];
        remote[0].iov_base = calloc(gig_row, col);
        remote[0].iov_len = gig_size;

        // Create Shared Memory
        struct Data *shm = (struct Data*)shm_builder( \
                shm_file_creat_mod, shm_prov_prot, \
                shm_prov_flags, shm_writer_file);
        
        printf("writer: sudo ./reader %d %p %llu\n", \
                getpid(), data, data_len);

        atomic_store(&shm->state, 0);
        while (atomic_load(&shm->state) != THREADS);

        printf("readers just read data. I'm done.\n");

        /* remove the shared memory object */
        munmap(shm, 1);     
        shm_unlink("Write_finish");  
        return 0;
}
