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
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h> 
#include <sys/shm.h> 
#include "header.h"

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

void psvm_error_handler(ssize_t nread2) {
        if (nread2 < 0) {
                switch (errno) {
                        case EINVAL:
                                printf("ERROR: INVALID ARGUMENTS.\n");
                                break;
                        case EFAULT:
                                printf("ERROR: UNABLE TO ACCESS TARGET MEMORY ADDRESS.\n");
                                break;
                        case ENOMEM:
                                printf("ERROR: UNABLE TO ALLOCATE MEMORY.\n");
                                break;
                        case EPERM:
                                printf("ERROR: INSUFFICIENT PRIVILEGES TO TARGET PROCESS.\n");
                                break;
                        case ESRCH:
                                printf("ERROR: PROCESS DOES NOT EXIST.\n");
                                break;
                        default:
                                printf("ERROR: AN UNKNOWN ERROR HAS OCCURRED.\n");
                }
                exit(1);
        }
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

        // Changing the process scheduling queue into real-time and set its priority using <sched.h>.
        set_cpu_scheduler(2,99);

        char *data = calloc(two_gigrow, col);

        /* shared memory file descriptor */
        int shm_fd; 
        
        /* pointer to shared memory obect */
        // void* shm; 
        
        /* create the shared memory object */
        shm_fd = shm_open("Write_finish", O_CREAT | O_RDWR, 0666); 
        if (shm_fd < 0)
                perror("shm_fd.\n");
        
        /* configure the size of the shared memory object */
        ftruncate(shm_fd, 1); 
        struct Data *shm = (struct Data *) mmap(0, DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        // shm = mmap(NULL, 1, PROT_WRITE|PROT_READ, MAP_SHARED, shm_fd, 0);//

        if (shm == MAP_FAILED)
        {
                perror("mmap error.\n");
                return 1;
        }
        printf("reader: sudo ./writer %d %p %lu\n", getpid(), data, gigsize);
        atomic_store(&shm->state, 1);
        while (atomic_load(&shm->state) != 2);
        // while (strcmp(shm, "b") != 0);

        printf("writer just wrote data. I'm done.\n");

        /* remove the shared memory object */
        munmap(shm, 1);     
        shm_unlink("Write_finish");  
        return 0;
}
