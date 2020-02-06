#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/uio.h>
#include <stdatomic.h>
#include <sys/resource.h>

#include "../../header.h"


#define THREADS		9
#define data_len        fourteen_gig_size


int main(int argc, char **argv) {

        // Changing the process scheduling queue into 
        // real-time and set its priority using <sched.h>.
        set_cpu_scheduler(1,99);

        char *data = calloc(fourteen_gig_row, col);

        // Create Shared Memory
        struct Data *shm = (struct Data*)shm_builder( \
                shm_file_creat_mod, shm_prov_prot, \
                shm_prov_flags, shm_writer_file);
        
        printf("reader: sudo ./writer %d %p %llu\n", \
                getpid(), data, data_len);

        atomic_store(&shm->state, 0);
        while (atomic_load(&shm->state) != THREADS)
        {
                // if (shm->state >= 2)
                // {
                //         /* code */
                //         printf("%d\n", shm->state);
                // }
                continue;
        }
        printf("writer just wrote data. I'm done.\n");

        /* remove the shared memory object */
        munmap(shm, 1);     
        shm_unlink("Write_finish");  
        return 0;
}
