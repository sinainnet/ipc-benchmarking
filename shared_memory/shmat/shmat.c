
/****************************************************************************
 * (C) 2019-2020 - Amir Hossein Sorouri - Sina Mahmoodi
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
    const char* shm = ipcShmAt("shmfile", SHMSIZE);

    printf("Data written in memory: %s\n","done");
    
    ipcShmDt(shm);
    
    return 0;
}
