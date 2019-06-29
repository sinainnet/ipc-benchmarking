
/****************************************************************************
 * (C) 2019-2020 - Amir Hossein Sorouri - Sina Mahmoodi
 ****************************************************************************
 *
 *      File: shared_memory/shared_memory.h
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */


#include <sys/ipc.h> 
#include <sys/shm.h> 
#include "../commons/commons.h"

/*
 *  Lorem Ipsum
 */
const char* ipcShmAt (char* ftokPathName, size_t shmSize);


/*
 *  Lorem Ipsum
 */
int ipcShmDt (const char* shmAddr);

/*
 *  Lorem Ipsum
 */
int ipcShmCtl (int shmId);
