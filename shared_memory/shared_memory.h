
/****************************************************************************
 * (C) 2019-2020 - Amir Hossein Sorouri - Sina Mahmoodi
 ****************************************************************************
 *
 *      File: shared_memory/shared_memory.h
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#ifndef SHMSIZE_H
#define SHMSIZE_H

#include <stdio.h>

#endif /* SHMSIZE_H */

#include <sys/ipc.h> 
#include <sys/shm.h> 

/*
 *  Lorem Ipsum
 */
const char* ipcShmAt (char* ftokPathName, size_t shmSize);


/*
 *  Lorem Ipsum
 */
int ipcShmDt (char* shmAddr);


