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
#include <sched.h>
#include <sched.h>
#include <string.h>
#include "helper.h"

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

int main(int argc, char **argv) {

        int mgrow = 1024;
        int gigrow = 1048576;
        long int two_gigrow = 2*gigrow;
        long int three_gigrow = 3*gigrow;
        long int four_gigrow = 4*gigrow;
        long int eight_gigrow = 8*gigrow;
        int col = 1024;

        unsigned long int mgsize = mgrow * col;
        unsigned long int gigsize = gigrow * col;
        unsigned long int two_gigsize = two_gigrow * col;
        unsigned long int three_gigsize = three_gigrow * col;
        unsigned long int four_gigsize = four_gigrow * col;
        unsigned long int eight_gigsize = eight_gigrow * col;
        union semun u, j;
	u.val = 1;
        j.val = 0;

        // Changing the process scheduling queue into real-time and set its priority using <sched.h>.
        set_cpu_scheduler(0,99);

        char *data = calloc(gigrow, col);
        printf("reader: sudo ./writer %d %p %lu \n", getpid(), data, gigsize);

        // Build iovec structs
        size_t bufferLength = two_gigsize;
        struct iovec local[1];
        local[0].iov_base = data;
        local[0].iov_len = bufferLength;

        struct iovec remote[1];
        remote[0].iov_base = calloc(bufferLength, sizeof(char));;
        remote[0].iov_len = bufferLength;

        int id_wrt = ipcb_get_semaphore(shared_wrt_key, 1, 0666 | IPC_CREAT);
        ipcb_control_semaphore(id_wrt, 0, SETVAL, j);

        // trying to get lock(after writing operation done.)
        ipcb_operate_semaphore(id_wrt, &decrease, 1);
        ipcb_operate_semaphore(id_wrt, &increase, 1);

        return 0;
}
