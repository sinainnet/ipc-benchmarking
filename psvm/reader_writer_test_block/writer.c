#define _GNU_SOURCE
#include <sys/uio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string.h>
#include "helper.h"
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


        // Call process_vm_readv - handle any error codes
        // int id_srt = ipcb_get_semaphore(shared_sem_key, 1, 0666);
        
        struct timespec start, finish;
        ssize_t nread[local_iov_num];
        ssize_t nread_res = 0;
        memset(nread, 0, local_iov_num*sizeof(ssize_t));
        int id_wrt = ipcb_get_semaphore(shared_wrt_key, 1, 0666); // key to inform reader.

        clock_gettime(CLOCK_REALTIME, &start);

        // ipcb_operate_semaphore(id_srt, &decrease, 1);
        for (int i = 0; i < local_iov_num; i++)
        {
                nread[i] = process_vm_writev(inputs.pid, (struct iovec *)&local[i], 1, remote, 1, 0);
                nread_res+=nread[i];
        }
        // ipcb_operate_semaphore(id_srt, &increase, 1);
        ipcb_operate_semaphore(id_wrt, &increase, 1);

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