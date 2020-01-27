#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/uio.h>
#include <sys/resource.h>
#include "../header.h"
#include "helper.h"

int main(int argc, char **argv) {
        // Changing the process scheduling queue into real-time and set its priority using <sched.h>.
        set_cpu_scheduler(2,99);
        
        union semun u, j;
	u.val = 1;
        j.val = 0;

        char *data = calloc(gig_row, col);
        printf("reader: sudo ./writer %d %p %lu \n", getpid(), data, gig_size);

        int id_wrt = ipcb_get_semaphore(shared_wrt_key, 1, 0666 | IPC_CREAT);
        ipcb_control_semaphore(id_wrt, 0, SETVAL, j);

        // trying to get lock(after writing operation done.)
        ipcb_operate_semaphore(id_wrt, &decrease, 1);
        ipcb_operate_semaphore(id_wrt, &increase, 1);

        return 0;
}
