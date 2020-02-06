#define _GNU_SOURCE
#include <pthread.h>
#include <sys/uio.h>
#include <sys/resource.h>
// #include "barrier.h"
#include "errors.h"
#include <sched.h>
#include <pthread.h>

#include "../../header.h"
#include "../../helper.h"

#define THREADS		9

typedef enum {true, false} bool;

static pthread_barrier_t barrier;
static int numBarriers;

/*
 *  Threads Data Structure to keep track of each one
 */
typedef struct thread_tags {
	pthread_t		thread_id;
	int         		thread_num;
	struct iovec		*local;
	struct iovec		*remote;
	data_input 		input;
	struct Data		*shm;
	int 			*lock_count;
	struct spinlock		*lock;
} thread_tracker;

// barrier_t barrier;
thread_tracker thread[THREADS];

typedef struct thread_return_data {
	struct timespec 	start;
	struct timespec 	finish;
	ssize_t 		nread;
	bool        		printed;
	bool 			status;
	int			min_offset;
	int 			max_offset;
} thread_result;


void* thread_routine (void *arg) {
	thread_tracker *self = (thread_tracker*) arg;
	thread_result *thread_res = \
		(struct thread_return_data*)calloc(1, sizeof(struct thread_return_data));
	cpu_set_t set;
	CPU_ZERO(&set);
	long long int cpu_no = (self->thread_num%9) + 3;
	CPU_SET(cpu_no, &set);
	if (pthread_setaffinity_np(self->thread_id, sizeof(cpu_set_t), &set) == -1)
	{
			printf("Thread setaffinity error");
			exit(0);
	}

	thread_res->status = false;
	thread_res->nread = 0;
	int status, s;
	int id_wrt = ipcb_get_semaphore(shared_wrt_key, 1, 0666); // key to inform reader.
	
	// printf("Thread (%d). I am gonna barrier.\n", self->thread_num);
	// status = barrier_wait (&barrier);
	s = pthread_barrier_wait(&barrier);

	clock_gettime(CLOCK_REALTIME, &thread_res->start);

	thread_res->nread = process_vm_writev(self->input.pid, \
		(struct iovec *)&self->local[self->thread_num], 1, self->remote, 1, 0);	
	ipcb_operate_semaphore(id_wrt, &increase, 1);

	clock_gettime(CLOCK_REALTIME, &thread_res->finish);
	
	return (void*)thread_res;
}

int* calc_max_clock (void **thread2) {
	thread_result **results = (struct thread_return_data **)thread2;
	double times [THREADS][2]; 	// [0] for start_time and [1] for end_time
	
	for (int i = 0; i < THREADS; i++)
	{
		printf("---------- Thread %d --------------\n", i);
		printf("\t\t start_time_second = %ld,    start_time_nano = %ld\n", results[i]->start.tv_sec, results[i]->start.tv_nsec);
		printf("\t\t end_time_second = %ld,    end_time_nano = %ld\n", results[i]->finish.tv_sec, results[i]->finish.tv_nsec);
		times[i][0] = (double)results[i]->start.tv_sec + \
			((double)results[i]->start.tv_nsec/(double)1000000000);
		times[i][1] = (double)results[i]->finish.tv_sec + \
			((double)results[i]->finish.tv_nsec/(double)1000000000);
	}

	int 	finish_offset_max = 0;
	int 	start_offset_min = 0;
	double 	finish = times[0][1];
	double 	start = times[0][0];
	printf("\n\n");
	printf("---------- Thread %d --------------\n", 0);
	printf("\t\t start_time = %f,    end_time = %f\n", times[0][0], times[0][1]);
	for (int i = 1; i < THREADS; i++)
	{
		printf("---------- Thread %d --------------\n", i);
		printf("\t\t start_time = %f,    end_time = %f\n", times[i][0], times[i][1]);
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
	printf("min_offset = %d, max_offset = %d\n", offset[0], offset[1]);

	return offset;
}

int main (int argc, char **argv) {
	set_cpu_scheduler(2, 99);

        // PARSE CLI ARGS
        data_input inputs;
        get_inputs(&inputs, argc, argv);

	int thread_count, array_count;
	int status, s;
	
	// barrier_init(&barrier, THREADS);
	s = pthread_barrier_init(&barrier, NULL, THREADS);
	if (s != 0)
	{
		/* code */
		perror("pthread_init\n");
	}
	
	// Build iovec structs
        int local_iov_num = THREADS;
        int data_len = inputs.buffer_length/local_iov_num;
	
        struct iovec local[local_iov_num];
        for (int i = 0; i < local_iov_num; i++)
        {
                char *data = calloc(data_len, sizeof(char));
                memset(data, 'a' + i, data_len);
                local[i].iov_base = data;
                local[i].iov_len = data_len;
	}
        
        struct iovec remote[1];
        remote[0].iov_base = inputs.remote_ptr;
        remote[0].iov_len = inputs.buffer_length;

	/*
	 * Create a set of threads that will use the barrier.
	 */
	for (thread_count = 0; thread_count < THREADS; thread_count++) {
		thread[thread_count].thread_num = thread_count;
		thread[thread_count].local = local;
		thread[thread_count].remote = remote;
		thread[thread_count].input = inputs;
		status = pthread_create (&thread[thread_count].thread_id,
			NULL, thread_routine, (void*)&thread[thread_count]);
		if (status != 0)
			err_abort (status, "Create thread");
	}
	
	/*
	 * Now join with each of the threads.
	 */
	thread_result **all_threads = (struct thread_return_data**) \
		calloc(THREADS, sizeof(struct thread_return_data));

	for (size_t i = 0; i < THREADS; i++)
	{
		all_threads[i] = (struct thread_return_data*) \
			calloc(1, sizeof(struct thread_return_data));
	}
	
	for (thread_count = 0; thread_count < THREADS; thread_count++) {
		thread_result *thread_res = NULL;
		status = pthread_join (thread[thread_count].thread_id, (void**)&thread_res);
		if (status != 0)
			err_abort (status, "Join thread");

		if (thread_res->printed == true)
		{
			// printf("%03d: (%d):true \n", thread_count, \
				thread[thread_count].thread_num);
			all_threads[thread_count] = thread_res;
		}
		else {
			printf("%03d: (%d):false \n", thread_count, \
				thread[thread_count].thread_num);
		}
	}

	int *offsets = calc_max_clock((void **)all_threads);
	struct timespec start = all_threads[offsets[0]]->start;
	struct timespec finish = all_threads[offsets[1]]->finish;
	ssize_t nreads = 0;
	for (int i = 0; i < THREADS; i++)
	{
		if (psvm_error_handler(all_threads[i]->nread) < 0)
			exit(1);
		nreads += all_threads[i]->nread;
	}
	print_results("write", psvm_writer, nreads, start, finish, fourteen_gig_file);
	
	/*
	 * To be thorough, destroy the barrier.
	 */
	// barrier_destroy (&barrier);
	return 0;
}

