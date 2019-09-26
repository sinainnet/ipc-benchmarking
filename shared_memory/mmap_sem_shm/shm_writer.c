
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: mmap_sem_shm/shm_writer.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <stdio.h> 
#include <string.h>

#include "mmap_shm.h"

struct timeval start, end;
const unsigned long int memSize = ( SHM_ROW_SIZE * SHM_COL_SIZE );

// 
/*
 *  Lorem Ipsum
 */
int 
main() {
    int fd, fdSync, fdTime;
    char *str;
    char **buf;
    pid_t otherChildId;
    test_args shm_args;

    ipcb_xpmem_arg_generator(&shm_args);
    shm_args.buf = ipcb_fake_data_generator(SHM_ROW_SIZE, SHM_COL_SIZE);

    ipcb_test_base_one(&shm_args);

    return 0;
}



int 
ipcb_test_base_one (test_args *shm_args) {
	int i, ret=0, expected;
    char* data; 

    data = shm_args->mmapd_area;
    
	printf("   \n\n==== WRITER: STARTS ====\n");
	
	printf("1- shm_proc_writer: mypid = %d\n", getpid());
	printf("2- shm_proc_writer: sharing %lld bytes\n", TMP_SHARE_SIZE);
	
	ipcb_get_time(&start, "\ntest_base_one:start: "); /* Start. */
	
	/* Copy data to mmap share */
    for (i = 0; i < SHM_ROW_SIZE; i++){
        memcpy((data), shm_args->buf[i],
				SHM_COL_SIZE);
	}

	printf("   \n\n==== WRITER: Ends ====\n");

	int id = ipcb_get_semaphore(shared_sem_key, 1, 0666);
    ipcb_operate_semaphore(id, &increase, 1);


	int id_2 = ipcb_get_semaphore(shared_ums_key, 1, 0666);
    ipcb_operate_semaphore(id_2, &decrease, 1);
	ipcb_operate_semaphore(id_2, &increase, 1);

	printf("writer: I'm gonna unmake what i've shared before.\n");

	return ret;
}