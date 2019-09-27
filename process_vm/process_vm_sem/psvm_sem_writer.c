/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: process_vm_sem/psvm_sem_writer.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

#include "psvm_sem.h"

struct timeval start, end;
const unsigned long int memSize = ( PSVM_ROW_SIZE * PSVM_COL_SIZE );


/*
 *  Lorem Ipsum
 */
int 
main() {
    int fd, fdSync, fdTime;
    char *str;
    char **buf;
    pid_t otherChildId;
    test_args psvm_args;

    /* Initializating Data */
    ipcb_psvm_arg_generator(&psvm_args);
    psvm_args.buf = ipcb_fake_data_generator(PSVM_ROW_SIZE, PSVM_COL_SIZE);
    
    /* try to get semaphore before trying to read other child ID. */
    int id_otcid = ipcb_get_semaphore(shared_otcid_key, 1, 0666 | IPC_CREAT);
    ipcb_operate_semaphore(id_otcid, &decrease, 1);    
    
    /* reading other child data from shared memory */
    char tmp [50] = {},
         *strtmp[2] = {};
    sprintf(tmp, "%s", psvm_args.share);
    ipcb_parse_otcData_to_char(tmp, strtmp);
    printf ("%s\n%s\n", strtmp[0], strtmp[1]);

    psvm_args.otcmem = ipcb_char_to_pointer(strtmp[1]);
    psvm_args.otcid = atoi(strtmp[0]);
    printf("writer: other child id is = %d:%p.\n", psvm_args.otcid, psvm_args.otcmem);

    ipcb_operate_semaphore(id_otcid, &increase, 1);

    /* running the test function */
    ipcb_test_base_one(&psvm_args);

    /* releasing the semaphore have been get to 
       by me to make reader able to read its data. */
    int id_sem = ipcb_get_semaphore(shared_sem_key, 1, 0666 | IPC_CREAT);
    ipcb_operate_semaphore(id_sem, &increase, 1); 

    /* try to get semaphore to know where i can exit. */
    int id_wrt = ipcb_get_semaphore(shared_wrt_key, 1, 0666);
    ipcb_operate_semaphore(id_wrt, &decrease, 1);
    ipcb_operate_semaphore(id_wrt, &increase, 1);

    return 0;
}

int 
ipcb_test_base_one (test_args *ipcb_psvm_args) {
	int i, ret=0, expected;
    char* data; 
    printf("writer: test_base_one:%s\n", "*ipcb_psvm_args->otcmem");
    // printf("readerrrrr: '%d'", *(*(ipcb_psvm_args->otcmem+1023) + 10));


    long long int nread = ipcb_process_vm_writev(ipcb_psvm_args->otcid,  
						ipcb_psvm_args->buf, PSVM_ROW_SIZE, PSVM_COL_SIZE,
						ipcb_psvm_args->otcmem, PSVM_ROW_SIZE, PSVM_COL_SIZE);

    return ret;
}