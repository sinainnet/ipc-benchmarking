
/****************************************************************************
 * (C) 2019-2020 - Amir Hossein Sorouri - Sina Mahmoodi
 ****************************************************************************
 *
 *      File: shared_memory/shared_memory.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#define _GNU_LINUX
#include <errno.h>
#include "shared_memory.h"

#define    ONERROR      (-1)
#define    ONSUCCESS    ( 0)


/*
 *  Lorem Ipsum
 */
static inline int 
error(const char* n)
{
	perror(n);
	return -1;
}


/*
 *  Lorem Ipsum
 */
const char*
ipcShmAt (char* ftokPathName, size_t shmSize) {
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
ipcShmDt (char* shmAddr) {
    int res = shmdt(shmAddr);
    if( ONERROR == res )
        return error("ipcShmDt");
    return res;
}