/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: process_vm_sem/psvm_sem.h
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#define _GNU_SOURCE
#include "../process_vm.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/uio.h>
#include <sys/types.h>       
#include <unistd.h>
#include <time.h>
#include <string.h>

#include <sys/mman.h>

#include"../../semaphore_lib/ipcb_semaphore.h"

#define SEM_KEY                   0x1111
#define WRT_KEY					  0x2222
#define ChildId_KEY				  0x3333
#define PSVM_ROW_SIZE        ((1ull) << 10)
#define PSVM_COL_SIZE        ((1ull) << 10)
#define TMP_SHARE_SIZE       PSVM_ROW_SIZE*PSVM_COL_SIZE
#define PSVM_FILE_ONE        "/tmp/ipcb_psvm_file_1"
#define PSVM_FILE_TWO        "/tmp/ipcb_psvm_file_2"
#define FIFO_FILE            "/tmp/fifo_twoway-shm"
#define FIFO_FILE_TIME       "/tmp/fifo_starttime-shm"


key_t shared_sem_key = SEM_KEY;
key_t shared_wrt_key = WRT_KEY;
key_t shared_otcid_key = ChildId_KEY;
struct sembuf decrease = DECREASE_SEM;
struct sembuf increase = INCREASE_SEM;


/* Structs for test functions */
typedef struct {
	int fd, shareFd;
	char *mmapd_area, *share;
	char* buf;
	char* emptyAlloc;
	int add;
    pid_t otcid;
    char* otcmem;

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
ipcb_psvm_arg_generator (test_args* ipcb_psvm_args) {
    if ((ipcb_psvm_args->fd = open(PSVM_FILE_ONE, O_RDWR)) == -1) {
		perror("open ipcb_psvm_file");
		return -1;
	}

    ipcb_psvm_args->share = (char*)mmap(0, TMP_SHARE_SIZE, PROT_READ|PROT_WRITE,
			MAP_SHARED, ipcb_psvm_args->fd, 0);
    if (ipcb_psvm_args->share == MAP_FAILED) {
		perror("mmap");
		return -1;
	}

    if ((ipcb_psvm_args->shareFd = open(PSVM_FILE_TWO, O_RDWR)) == -1) {
		perror("open ipcb_shareFd_file");
		return -1;
	}
    
    return 1;
}

void* 
ipcb_char_to_pointer(char* input){
	unsigned long l;
    sscanf(input, "%lx", &l);
    void* otcidp = (void*)(uintptr_t)l;
	return otcidp;
}

void
ipcb_parse_otcData_to_char(char* input, char* output[2]){
	char *p = strtok (input," :");
    int i = 0;
    while (p!= NULL)
    {
		output[i] = p;
        p = strtok (NULL, " :");
        i++;
    }
	return;
}


int
ipcb_psvm_empty_allocator(char* space, long long memSize, 
		long bitSize, char* method){
	// printf("ipcb_psvm_empty_allocator\n");
	if("calloc" == method){
		space = (char*)calloc(memSize, bitSize);
		return ON_SUCCESS;
	}
	else{
		space = (char*)calloc(memSize, bitSize);		
		return ON_SUCCESS;
	}
	return ON_ERROR;
}


int 
ipcb_psvm_fakeData_generator(char* space, long long memSize, 
		long bitSize, char* method){
	int res, freq;
    time_t t;
	
	// res = ipcb_psvm_empty_allocator(space, memSize, bitSize, method);
	space = (char*)calloc(memSize, bitSize);
	if(res < 0){
		printf("ahhhh\n");
		return ON_ERROR;
	}

    srand((unsigned) time(&t));

	freq = memSize/1024;
	char* pos = space;
	int cnt = 0;
	while(cnt < freq){
		// printf("%d ", cnt);
		char a = (rand() % 58) + 65;
		memset(pos, a, 1024);
		pos+=1024;
		cnt++;
	}
	
	return ON_SUCCESS;
}