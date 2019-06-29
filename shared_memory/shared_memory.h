
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: shared_memory/shared_memory.h
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */


#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
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


/*
 *  Lorem Ipsum
 */
int ipcb_open_shm (const char* name, unsigned long long truncationSize);


/*
 *  Lorem Ipsum
 */
char* ipcb_map_memory_to_fd (unsigned long long memorySize, int fd, off_t offset);


/*
 *  Lorem Ipsum
 */
int ipcb_unmapp_memory (void *addr, size_t length);


/*
 *  Lorem Ipsum
 */
int ipcb_unlink_shm (const char* name);

