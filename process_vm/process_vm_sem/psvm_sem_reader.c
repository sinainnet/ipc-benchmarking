/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: process_vm_sem/psvm_sem_reader.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <stdio.h> 
#include <string.h>
#include <unistd.h>

#include "psvm_sem.h"

struct timeval start, end;
const unsigned long int memSize = ( PSVM_ROW_SIZE * PSVM_COL_SIZE );


/*
 *  Lorem Ipsum
 */
int 
main(int argc, char *argv[]) {
    int fd, fdSync, fdTime;
    char *str;
    char **buf;
    pid_t otherChildId;
    test_args psvm_args;
    union semun u;
    u.val = 1;

    /* Initializating Data */
    ipcb_psvm_arg_generator(&psvm_args);
    // psvm_args.emptyAlloc = ipcb_fake_data_generator(PSVM_ROW_SIZE, PSVM_COL_SIZE);
    psvm_args.emptyAlloc = ipcb_empty_allocator(PSVM_ROW_SIZE, PSVM_COL_SIZE);
    // for (int i = 0; i<PSVM_ROW_SIZE; i++)
    //     printf("%s", psvm_args.emptyAlloc[i]);

    printf("readerrrrr: '%d'", *(*(psvm_args.emptyAlloc+1023) + 10));

    printf("reader: 1 and my pid is %ld:%p. and is = %s\n", (long)getppid(), psvm_args.emptyAlloc, psvm_args.share);
    /* writing my ID into shared FD. */
    sprintf(psvm_args.share, "%ld:%p", (long)getppid(), psvm_args.emptyAlloc);

    /* getting the semaphore required to forbid writer be exited
     after writing becausme data is in its address space and it 
     must not be closed */
    int id_wrt = ipcb_get_semaphore(shared_wrt_key, 1, 0666 | IPC_CREAT);
    ipcb_control_semaphore(id_wrt, 0, SETVAL, u);
    ipcb_operate_semaphore(id_wrt, &decrease, 1);

    /* releasing the semaphore have been get to make writer able to read my ID. */
    int id_otcid = ipcb_get_semaphore(shared_otcid_key, 1, 0666);
    ipcb_operate_semaphore(id_otcid, &increase, 1);  

    /* try to get semaphore before trying to read */
    int id_sem = ipcb_get_semaphore(shared_sem_key, 1, 0666);
    ipcb_operate_semaphore(id_sem, &decrease, 1); 
    
    /* running the test function */
    ipcb_test_base_one(&psvm_args);

    ipcb_operate_semaphore(id_sem, &increase, 1); 

    /* releasing the wrt semaphore to make writer able to exit. */
    ipcb_operate_semaphore(id_wrt, &increase, 1);

    return 0;
}

int 
ipcb_test_base_one (test_args *ipcb_psvm_args) {
	int i, ret=0, expected;
    char* data; 

    printf("reader: test_base_one\n");
    for(i = 0; i < 100000; i++);

    return ret;
}