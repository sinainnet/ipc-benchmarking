
/****************************************************************************
 * (C) 2019-2020 - Amir Hossein Sorouri - Sina Mahmoodi
 ****************************************************************************
 *
 *      File: shared_memory/shared_memory.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <errno.h>
#include "shared_memory.h"



/*
 *  Lorem Ipsum
 */
const char*
ipcb_attach_shm (char* ftokPathName, size_t shmSize) {
    // ftok to generate unique key                       
    key_t key = ftok(ftokPathName, 65); 
  
    // shmget returns an identifier in shmid 
    int shmId = shmget(key, shmSize, 0666|IPC_CREAT); 
  
    // shmat to attach to shared memory 
    const char* str = (char*) shmat(shmId, (void*)0, 0); 
    return str;
}


/*
 *  Lorem Ipsum
 */
int 
ipcb_dettach_shm (const char* shmAddr) {
    int res = shmdt(shmAddr);
    if( ON_ERROR == res )
        return ipcb_print_error("ipcShmDt");
    return ON_SUCCESS;
}



int 
ipcb_cntrl_shm (int shmId) {
    int res = shmctl(shmId,IPC_RMID,NULL); 
    if( ON_ERROR == res )
        return ipcb_print_error("ipcShmCtl");
    return ON_SUCCESS;
}
