#define _GNU_SOURCE
#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string.h>
#include "../header.h"


int main(int argc, char **argv) {
        set_cpu_scheduler(1, 99);

        // PARSE CLI ARGS
        data_input inputs;
        get_inputs(&inputs, argc, argv);

        // Build iovec structs
        int local_iov_num = inputs.buffer_length / gig_size;
        struct iovec local[local_iov_num];
        for (int i = 0; i < local_iov_num; i++)
        {
                char *data = calloc(inputs.buffer_length, sizeof(char));
                memset(data, 'a' + i, gig_size);
                local[i].iov_base = data;
                local[i].iov_len = gig_size;
        }
        
        struct iovec remote[1];
        remote[0].iov_base = inputs.remote_ptr;
        remote[0].iov_len = inputs.buffer_length;

        // Create Shared Memory
        struct Data *shm = (struct Data*)shm_builder(shm_file_use_mod, shm_prov_prot, shm_prov_flags, shm_writer_file);

        // Call process_vm_readv - handle any error codes
        struct timespec start, finish;
        ssize_t nread[local_iov_num];
        ssize_t nread_res = 0;
        memset(nread, 0, local_iov_num*sizeof(ssize_t));
        clock_gettime(CLOCK_REALTIME, &start);

        for (int i = 0; i < local_iov_num; i++)
        {
                nread[i] = process_vm_writev(inputs.pid, (struct iovec *)&local[i], 1, remote, 1, 0);
                nread_res+=nread[i];
        }
        atomic_store(&shm->state, 2);

        clock_gettime(CLOCK_REALTIME, &finish);

        // Error Checking
        for (int i = 0; i < local_iov_num; i++)
        {
                if(psvm_error_handler(nread[i]) < 0)
                        exit(1);
        }

        // Print Result
        print_results(psvm_writer, nread_res, start, finish);
        return 0;
}
