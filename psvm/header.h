#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <time.h>
#include <fcntl.h>
#include <errno.h>          // errno
#include <unistd.h>
#include <sys/mman.h>       // mmap
#include <sys/time.h>
#include <sys/types.h>

struct Data {
  int state;
};

struct spinlock {
    int locked;
};

#define SPINLOCK_INIT { 0 };

typedef struct main_inputs {
        pid_t pid;
        void *remote_ptr;
        ssize_t buffer_length;
} data_input;

#define DATA_SIZE (sizeof(struct Data))


typedef enum {true, false} bool;

static pthread_barrier_t barrier;
static int numBarriers;

/*
 *  Threads Data Structure to keep track of each one
 */
typedef struct thread_tags {
	pthread_t		thread_id;
	int         		thread_num;
	int			thread_chunks;
	struct iovec		*local;
	struct iovec		*remote;
	data_input 		input;
	struct Data		*shm;
	int 			*lock_count;
	struct spinlock		*lock;
} thread_tracker;

typedef struct thread_return_data {
	struct timespec 	start;
	struct timespec 	finish;
	ssize_t 		nread;
	bool        		printed;
	bool 			status;
	int			min_offset;
	int 			max_offset;
} thread_result;

#endif  /* PROTOCOL_H */

#define two			2
#define seven			7
#define ten			10
#define col			1024
#define meg_row			1024
#define gig_row			1048576
#define two_gig_row		(long long int)(two * gig_row)
#define twenty_gig_row		(long long int)(ten * two * gig_row)
#define four_gig_row		(long long int)(two*two_gig_row)
#define seven_gig_row		(long long int)(seven*gig_row)
#define eight_gig_row		(long long int)(two*four_gig_row)
#define fourteen_gig_row	(long long int)(two*seven_gig_row)
#define sixteen_gig_row		(long long int)(two*eight_gig_row)
#define lli			(long long int)
#define gig_size		(gig_row*col)
#define two_gig_size    	((long)two*(long)gig_size)
#define twenty_gig_size    	((long)two * (long)ten *(long)gig_size)
#define four_gig_size		((long)two * (long)two_gig_size)
#define seven_gig_size		((long)seven * (long)gig_size)
#define eight_gig_size  	((long long)two*(long long)four_gig_size)
#define fourteen_gig_size  	((long long)two*(long long)seven_gig_size)
#define sixteen_gig_size  	((long long)two*(long long)eight_gig_size)
#define shm_writer_file		"Write_finish"
#define shm_reader_file		"Read_finish"
#define shm_file_creat_mod	(O_CREAT | O_RDWR)
#define shm_file_use_mod	(O_RDWR)
#define shm_prov_prot		(PROT_READ | PROT_WRITE) 	// Provider is the 2nd program to run
#define shm_prov_flags		MAP_SHARED			// Provider is the 2nd program to run
#define shm_cons_prot						// Provider is the first program to run
#define shm_cons_flags						// Provider is the first program to run
#define psvm_writer		"process_vm_writev"
#define psvm_reader		"process_vm_readv"
#define one_gig_file  		"results/one_gig.txt"
#define two_gig_file  		"results/two_gig.txt"
#define four_gig_file  		"results/four_gig.txt"
#define seven_gig_file  	"results/seven_gig.txt"
#define eight_gig_file  	"results/eight_gig.txt"
#define fourteen_gig_file  	"results/fourteen_gig.txt"
#define sixteen_gig_file  	"results/sixteen_gig.txt"
#define twenty_gig_file  	"results/twenty_gig.txt"
#define middleware		"./middleware.txt"


int psvm_error_handler (ssize_t nread2);

void set_cpu_scheduler (int cpu_no, int priority);

int* calc_max_clock (void **thread2, int thread_no);

void get_inputs (data_input *input_var, int argc, char **argv);

void* shm_builder (int file_mode, int prot, int flags, char *shm_file_name);

void print_results (char* read_write, char* psvm, ssize_t nread, struct timespec start, struct timespec finish, char *filename);
