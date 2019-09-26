/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: mmap_sem_shm/mmap_shm.h
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */



#include "../shared_memory.h"
#include"../../semaphore_lib/ipcb_semaphore.h"

#define SEM_KEY                   0x1111
#define UMS_KEY					  0x2222
#define SHM_ROW_SIZE        ((1ull) << 20)
#define SHM_COL_SIZE        ((1ull) << 20)
#define TMP_SHARE_SIZE      SHM_ROW_SIZE*SHM_COL_SIZE
#define SHM_FILE            "/tmp/ipcb_shm_file"
#define SHM_FILE_NAME       "ipcb_shm_file"
#define FIFO_FILE           "/tmp/fifo_twoway-shm"
#define FIFO_FILE_TIME      "/tmp/fifo_starttime-shm"


key_t shared_sem_key = SEM_KEY;
key_t shared_ums_key = UMS_KEY;
struct sembuf decrease = DECREASE_SEM;
struct sembuf increase = INCREASE_SEM;


/* Structs for test functions */
typedef struct {
	int fd;
	char *mmapd_area, *share;
	char** buf;
	char** emptyAlloc;
	int add;
} test_args;

typedef struct {
	char *name;
	int (*function)(test_args*);
} test_struct;

/* Test function prototypes:
 * 	each function is implemented by both xpmem_proc1.c and xpmem_proc2.c
 */
int test_base(test_args*);
int ipcb_test_base_one(test_args*);
int ipcb_test_base_two(test_args*);
int test_two_attach(test_args*);
int test_two_shares(test_args*);
int test_fork(test_args*);

/* Create an array of test functions structs:
 * 	allows xpmem_master.c to loop over all the tests
 */
#define add_test(name) { #name, name }

test_struct xpmem_test[] = {
	add_test(ipcb_test_base_one),
	{ NULL }
};


/*
 *  Lorem Ipsum
 */
int
ipcb_xpmem_arg_generator (test_args* xpmem_args) {
    if ((xpmem_args->fd = open(SHM_FILE, O_RDWR)) == -1) {
		perror("open ipcb_shm_file");
		return -1;
	}
    xpmem_args->share = mmap(0, TMP_SHARE_SIZE, PROT_READ|PROT_WRITE,
			MAP_SHARED, xpmem_args->fd, 0);
    if (xpmem_args->share == MAP_FAILED) {
		perror("mmap");
		return -1;
	}

	xpmem_args->fd = ipcb_open_shm(SHM_FILE_NAME, TMP_SHARE_SIZE);
    xpmem_args->mmapd_area = ipcb_map_memory_to_fd(TMP_SHARE_SIZE, xpmem_args->fd, 0);
    return 1;
}