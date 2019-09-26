
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: mmap_sem_shm/shm_reader.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <stdio.h> 
#include <string.h>

#include "mmap_shm.h"

struct timeval start, end;
const unsigned long int memSize = ( SHM_ROW_SIZE * SHM_COL_SIZE );


/*
 *  Lorem Ipsum
 */
int 
main() {
    int fd, fdTime,
        fdSync;
    char *str;
    char **buf;
    long int sec, usec;
    test_args shm_args;
    union semun u;
    u.val = 1;

    int id_2 = ipcb_get_semaphore(shared_ums_key, 1, 0666 | IPC_CREAT);
	ipcb_control_semaphore(id_2, 0, SETVAL, u);
	ipcb_operate_semaphore(id_2, &decrease, 1);

    ipcb_xpmem_arg_generator(&shm_args);
    shm_args.emptyAlloc = ipcb_empty_allocator(SHM_ROW_SIZE, SHM_COL_SIZE);

    int id = ipcb_get_semaphore(shared_sem_key, 1, 0666);
    ipcb_operate_semaphore(id, &decrease, 1);

	ipcb_test_base_one(&shm_args);

    ipcb_operate_semaphore(id, &increase, 1);
    ipcb_operate_semaphore(id_2, &increase, 1);
    return 0;
}


// 
int ipcb_test_base_one(test_args* shm_args) { 
    int i, ret=0, expected;
    char* data; 
    data = shm_args->mmapd_area;
    
	printf("   \n\n==== Reader: STARTS ====\n");    
    printf("1- xpmem_proc_reader: mypid = %d\n", getpid());


	/* Copy data to mmap share */
    for (int i = 0; i < SHM_ROW_SIZE; i++)
        memcpy(shm_args->emptyAlloc[i], data + (i*SHM_COL_SIZE), SHM_COL_SIZE);
    
    ipcb_get_time(&end, "\ntest_base_one:end: "); /* End. */

    printf("   \n\n==== Reader: Ends ====\n");

    return ret;
}
