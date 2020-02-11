#define _GNU_SOURCE
#include <sched.h>          // CPU
#include <stdio.h>
#include <stdlib.h>
#include "header.h"


void set_cpu_scheduler (int cpu_no, int priority) {
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

int psvm_error_handler (ssize_t nread2) {
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
                return -1;
        }
        return 1;
}

void get_inputs (data_input *input_var, int argc, char **argv) {
        if (argc < 3) {
                printf("usage: %s <pid> <mem address> [len]\n", argv[0]);
                printf("  <pid> - PID of process to target\n");
                printf("  <mem> - Memory address to target\n");
                printf("  [len] - Length (in bytes) to dump\n");
                exit(1);
        }

        input_var->pid = strtol(argv[1], NULL, 10);
        printf(" * Launching with a target PID of: %d\n", input_var->pid);

        input_var->remote_ptr = (void *)strtol(argv[2], NULL, 0);
        printf(" * Launching with a target address of 0x%llx\n", (long long unsigned)input_var->remote_ptr);

        input_var->buffer_length = (argc > 3) ? strtol(argv[3], NULL, 10) : 20;
        printf(" * Launching with a buffer size of %lu bytes.\n", input_var->buffer_length);

        return;
}

void* shm_builder (int file_mode, int prot, int flags, char *shm_file_name) {
        /* shared memory file descriptor */
        int shm_fd; 
                
        /* open the shared memory object */
        shm_fd = shm_open(shm_file_name, file_mode, 0666); 
        if (shm_fd == -1)
        {
                perror("shm_fd.\n");
        }

        /* configure the size of the shared memory object */
        if(file_mode == O_CREAT | O_RDWR)
                ftruncate(shm_fd, 1);
        
        /* memory map the shared memory object */
        struct Data *shm = (struct Data *) mmap(0, DATA_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

        if (shm == MAP_FAILED)
        {
                perror("mmap error.\n");
                exit(1);
        }
        return shm;
}

void print_results (char* read_write, char* psvm, ssize_t nread, struct timespec start, struct timespec finish, char* filename) {
        printf(" * Executed %s, %s %zd bytes.\n", psvm, read_write, nread);

        long seconds = finish.tv_sec - start.tv_sec;
        long ns = finish.tv_nsec - start.tv_nsec;

        if (start.tv_nsec > finish.tv_nsec) { // clock underflow
                --seconds;
                ns += 1000000000;
        }
        printf("seconds without ns: %ld\n", seconds);
        printf("nanoseconds: %ld\n", ns);
        printf("total seconds: %e\n", (double)seconds + (double)ns/(double)1000000000);

        FILE *file_res = fopen(filename, "a");
        fprintf(file_res, "%ld.%ld\n", seconds, ns);
}



int* calc_max_clock (void **thread2, int thread_no) {
	thread_result **results = (struct thread_return_data **)thread2;
	double times [thread_no][2]; 	// [0] for start_time and [1] for end_time
	
	for (int i = 0; i < thread_no; i++)
	{
		// printf("---------- Thread %d --------------\n", i);
		// printf("\t\t start_time_second = %ld,    start_time_nano = %ld\n", results[i]->start.tv_sec, results[i]->start.tv_nsec);
		// printf("\t\t end_time_second = %ld,    end_time_nano = %ld\n", results[i]->finish.tv_sec, results[i]->finish.tv_nsec);
		times[i][0] = (double)results[i]->start.tv_sec + \
			((double)results[i]->start.tv_nsec/(double)1000000000);
		times[i][1] = (double)results[i]->finish.tv_sec + \
			((double)results[i]->finish.tv_nsec/(double)1000000000);
	}

	int 	finish_offset_max = 0;
	int 	start_offset_min = 0;
	double 	finish = times[0][1];
	double 	start = times[0][0];
	// printf("\n\n");
	// printf("---------- Thread %d --------------\n", 0);
	// printf("\t\t start_time = %f,    end_time = %f\n", times[0][0], times[0][1]);
	for (int i = 1; i < thread_no; i++)
	{
		// printf("---------- Thread %d --------------\n", i);
		// printf("\t\t start_time = %f,    end_time = %f\n", times[i][0], times[i][1]);
		if (times[i][0] < start)
		{
			start_offset_min = i;
			start = times[i][0];
		}
		if (times[i][1] > finish)
		{
			finish_offset_max = i;
			finish = times[i][1];
		}
	}
	int *offset = (int *)calloc(2, sizeof(int));
	offset[0] = start_offset_min;		// [0]  for start_time
	offset[1] = finish_offset_max;		// [1]	for finish_time
	// printf("min_offset = %d, max_offset = %d\n", offset[0], offset[1]);

	return offset;
}
