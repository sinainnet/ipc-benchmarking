#define _GNU_SOURCE
#include <sched.h>
#include <pthread.h>
#include <sys/uio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <sys/resource.h>

#include "errors.h"
#include "../../../header.h"

#define THREADS		2

thread_tracker thread[THREADS];

void* thread_routine (void *arg) {
	thread_tracker *self = (thread_tracker*) arg;
	thread_result *thread_res = (struct thread_return_data*) \
		calloc(1, sizeof(struct thread_return_data));

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

	// printf("Thread (%d). I am gonna barrier.\n", self->thread_num);
	s = pthread_barrier_wait(&barrier);

	clock_gettime(CLOCK_REALTIME, &thread_res->start);

	thread_res->nread = process_vm_writev(self->input.pid, 
		(struct iovec *)&self->local[self->thread_num], \
		1, self->remote, 1, 0);

	int x = __sync_add_and_fetch(&self->shm->state, 1);

	clock_gettime(CLOCK_REALTIME, &thread_res->finish);
	
	return (void*)thread_res;
}


int main (int argc, char **argv) {
	set_cpu_scheduler(2, 99);

        // PARSE CLI ARGS
        data_input inputs;
        get_inputs(&inputs, argc, argv);

	int thread_count, array_count;
	int status, s;
	int lock_count = 0;

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

	// Create Shared Memory
        struct Data * volatile shm = (struct Data* volatile)shm_builder(shm_file_use_mod, shm_prov_prot, shm_prov_flags,shm_writer_file);

	/*
	 * Create a set of threads that will use the barrier.
	 */
	for (thread_count = 0; thread_count < THREADS; thread_count++) {
		thread[thread_count].thread_num = thread_count;
		thread[thread_count].local = local;
		thread[thread_count].remote = remote;
		thread[thread_count].input = inputs;
		thread[thread_count].shm  = shm;
		status = pthread_create (&thread[thread_count].thread_id, NULL, thread_routine, (void*)&thread[thread_count]);
		if (status != 0)
			err_abort (status, "Create thread");
	}
	
	/*
	 * Now join with each of the threads.
	 */
	thread_result **all_threads = (struct thread_return_data**) calloc(THREADS, sizeof(struct thread_return_data));

	for (size_t i = 0; i < THREADS; i++)
	{
		all_threads[i] = (struct thread_return_data*)calloc(1, sizeof(struct thread_return_data));
	}
	
	for (thread_count = 0; thread_count < THREADS; thread_count++) {
		thread_result *thread_res = NULL;
		status = pthread_join (thread[thread_count].thread_id,(void**)&thread_res);
		if (status != 0)
			err_abort (status, "Join thread");

		if (thread_res->printed == true)
		{
			// printf("%03d: (%d):true \n", thread_count,thread[thread_count].thread_num);
			all_threads[thread_count] = thread_res;
		}
		else {
			printf("%03d: (%d):false \n", thread_count,thread[thread_count].thread_num);
		}
	}

	int *offsets = calc_max_clock((void **)all_threads, THREADS);
	struct timespec start = all_threads[offsets[0]]->start;
	struct timespec finish = all_threads[offsets[1]]->finish;
	ssize_t nreads = 0;
	for (int i = 0; i < THREADS; i++)
	{
		if (psvm_error_handler(all_threads[i]->nread) < 0)
			exit(1);
		nreads += all_threads[i]->nread;
	}
	print_results("write", psvm_writer, nreads, start, finish, two_gig_file);
	
	return 0;
}

