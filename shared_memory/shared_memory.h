
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
const char* ipcb_attach_shm (char* ftokPathName, size_t shmSize);


/*
 *  Lorem Ipsum
 */
int ipcb_dettach_shm (const char* shmAddr);


/*
 *  Lorem Ipsum
 */
int ipcb_cntrl_shm (int shmId);
