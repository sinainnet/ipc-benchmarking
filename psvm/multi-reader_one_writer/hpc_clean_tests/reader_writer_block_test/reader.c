#define _GNU_SOURCE
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/uio.h>
#include <sys/resource.h>

#include "../../../header.h"
#include "../../../helper.h"

#define THREADS		3
#define data_len    two_gig_size
#define DECREASE_SEM_THREADS { 0, -THREADS, SEM_UNDO}
#define INCREASE_SEM_THREADS { 0, +THREADS, SEM_UNDO}

struct sembuf decrease_threads = DECREASE_SEM_THREADS;
struct sembuf increase_threads = INCREASE_SEM_THREADS;

struct timespec 	start;
struct timespec 	finish;

int main(int argc, char **argv) {
        // Changing the process scheduling queue into real-time 
        // and set its priority using <sched.h>.
        set_cpu_scheduler(0,99);
        
        union semun j;
        j.val = 0;

        char    *data = calloc(two_gig_row, col), 
                *executor_data = calloc(512, sizeof(char));

        sprintf(executor_data, "./writer %d %p %llu", getpid(), data, data_len);
        
        FILE *file_res = fopen(middleware, "w+");
        fputs(executor_data, file_res);

        printf("reader: sudo ./writer %d %p %llu \n", getpid(), data, data_len);

		clock_gettime(CLOCK_REALTIME, &start);

        int id_wrt = ipcb_get_semaphore(shared_wrt_key, 1, 0666 | IPC_CREAT);
        ipcb_control_semaphore(id_wrt, 0, SETVAL, j);
        fclose(file_res);

        // trying to get lock(after writing operation done.)
        ipcb_operate_semaphore(id_wrt, &decrease_threads, 1);

		clock_gettime(CLOCK_REALTIME, &finish);


		long seconds = finish.tv_sec - start.tv_sec;
        long ns = finish.tv_nsec - start.tv_nsec;

        if (start.tv_nsec > finish.tv_nsec) { // clock underflow
                --seconds;
                ns += 1000000000;
        }
        printf("reader: seconds without ns: %ld\n", seconds);
        printf("reader: nanoseconds: %ld\n", ns);
        printf("reader: total seconds: %e\n", (double)seconds + (double)ns/(double)1000000000);


        return 0;
}
