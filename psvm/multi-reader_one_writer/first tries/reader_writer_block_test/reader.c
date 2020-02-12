#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/uio.h>
#include <sys/resource.h>

#include "../../../header.h"
#include "../../../helper.h"

#define THREADS		8
#define data_len        fourteen_gig_size
#define DECREASE_SEM_THREADS { 0, -THREADS, SEM_UNDO}
#define INCREASE_SEM_THREADS { 0, +THREADS, SEM_UNDO}

struct sembuf decrease_threads = DECREASE_SEM_THREADS;
struct sembuf increase_threads = INCREASE_SEM_THREADS;

int main(int argc, char **argv) {
        // Changing the process scheduling queue into real-time 
        // and set its priority using <sched.h>.
        set_cpu_scheduler(1,99);
        
        union semun j;
        j.val = 0;

        char *data = calloc(fourteen_gig_row, col);
        printf("reader: sudo ./writer %d %p %llu \n", getpid(), data, data_len);

        int id_wrt = ipcb_get_semaphore(shared_wrt_key, 1, 0666 | IPC_CREAT);
        ipcb_control_semaphore(id_wrt, 0, SETVAL, j);

        // trying to get lock(after writing operation done.)
        ipcb_operate_semaphore(id_wrt, &decrease_threads, 1);

        // ipcb_operate_semaphore(id_wrt, &increase_threads, 1);

        return 0;
}
