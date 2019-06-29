
/****************************************************************************
 * (C) 2019-2020 - Iran University of Science and Technology@DSLab
 ****************************************************************************
 *
 *      File: shmat/shmat.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <stdio.h> 
#include "../shared_memory.h"


#define SHMSIZE     1024*100000

/*
 *  Lorem Ipsum
 */
int 
main() {
    const char* shm = ipcb_attach_shm("shmfile", SHMSIZE);

    printf("Data written in memory: %s\n","done");
    
    ipcb_dettach_shm(shm);
    
    return 0;
}
