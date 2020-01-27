#define _GNU_SOURCE
#include <pthread.h>
#include <sys/uio.h>
#include <sys/resource.h>
#include "barrier.h"
#include "errors.h"
#include "../../header.h"

#define THREADS		2

typedef enum {true, false} bool;

/*
 *  Threads Data Structure to keep track of each one
 */
typedef struct thread_tags {
	pthread_t	thread_id;
	int         	thread_num;
	struct iovec	*local;
	struct iovec	*remote;
} thread_tracker;

barrier_t barrier;
thread_tracker thread[THREADS];

typedef struct thread_return_data {
	struct timespec 	start;
	struct timespec 	finish;
	ssize_t 		nread;
	bool        		printed;
	bool 			status;
} thread_result;


void* thread_routine (void *arg) {
	thread_tracker *self = (thread_tracker*) arg;
	thread_result *thread_res = \
		(struct thread_return_data*)calloc(1, sizeof(struct thread_return_data));
	thread_res[0].status = false;
	thread_res[0].nread = 0;
	int status;

	printf("Thread (%d). I am gonna barrier.\n", self->thread_num);
	status = barrier_wait (&barrier);

	clock_gettime(CLOCK_REALTIME, &thread_res[0].start);

	if (status > 0)
        	err_abort(status, "Wait on barrier");
	else {
        	printf("Thread (%d). I woke up after barrier done.\n", self->thread_num);
		    thread_res[0].printed = true;
	}

	clock_gettime(CLOCK_REALTIME, &thread_res[0].finish);
	
	return (void*)thread_res;
}

int main (int argc, char **argv) {
	set_cpu_scheduler(1, 99);

        // PARSE CLI ARGS
        data_input inputs;
        get_inputs(&inputs, argc, argv);

	int thread_count, array_count;
	int status;
	
	barrier_init(&barrier, THREADS);
	
	// Build iovec structs
        int local_iov_num = inputs.buffer_length/gig_size;
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

	/*
	 * Create a set of threads that will use the barrier.
	 */
	for (thread_count = 0; thread_count < THREADS; thread_count++) {
		thread[thread_count].thread_num = thread_count;
		thread[thread_count].local = local;
		thread[thread_count].remote = remote;
		status = pthread_create (&thread[thread_count].thread_id,
			NULL, thread_routine, (void*)&thread[thread_count]);
		if (status != 0)
			err_abort (status, "Create thread");
	}
	
	/*
	 * Now join with each of the threads.
	 */
	for (thread_count = 0; thread_count < THREADS; thread_count++) {
		thread_result *thread_res = NULL;
		status = pthread_join (thread[thread_count].thread_id, (void**)&thread_res);
		if (status != 0)
			err_abort (status, "Join thread");

		if (thread_res->printed == true)
		{
			printf("%02d: (%d):true \n", thread_count, thread[thread_count].thread_num);
		}
		else {
			printf("%02d: (%d):false \n", thread_count, thread[thread_count].thread_num);
		}
	}
	
	/*
	 * To be thorough, destroy the barrier.
	 */
	barrier_destroy (&barrier);
	return 0;
}

