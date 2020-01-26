#include <pthread.h>
#include "barrier.h"
#include "errors.h"

#define THREADS		2

typedef enum {true, false} bool;

/*
 *  Threads Data Structure to keep track of each one
 */
typedef struct thread_tags {
	pthread_t	thread_id;
	int         thread_num;
	bool        printed;
} thread_tracker;

barrier_t barrier;
thread_tracker thread[THREADS];


void *
thread_routine (void *arg) {
	thread_tracker *self = (thread_tracker*) arg;
	int status;
	printf("Thread (%d). I am gonna barrier.\n", self->thread_num);
	status = barrier_wait (&barrier);
	if (status > 0)
            err_abort(status, "Wait on barrier");
	else
	{
		printf("Thread (%d). I woke up after barrier done.\n", self->thread_num);

	}
	
	return self;
}


int 
main (int argc, void *argv[]) {
	int thread_count, array_count;
	int status;

	barrier_init(&barrier, THREADS);

	/*
	 * Create a set of threads that will use the barrier.
	 */
	for (thread_count = 0; thread_count < THREADS; thread_count++) {
		thread[thread_count].thread_num = thread_count;
		thread[thread_count].printed = false;
		status = pthread_create (&thread[thread_count].thread_id,
			NULL, thread_routine, (void*)&thread[thread_count]);

		if (status != 0)
			err_abort (status, "Create thread");
	}

	/*
	 * Now join with each of the threads.
	 */
	for (thread_count = 0; thread_count < THREADS; thread_count++) {
		status = pthread_join (thread[thread_count].thread_id, NULL);
		if (status != 0)
			err_abort (status, "Join thread");

		printf("%02d: (%d) ", thread_count, thread[thread_count].thread_num);
		printf ("\n");
	}
	
	/*
	 * To be thorough, destroy the barrier.
	 */
	barrier_destroy (&barrier);
	return 0;
}

