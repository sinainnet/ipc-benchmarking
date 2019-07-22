/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: integrated/psvm.h
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "../process_vm.h"
// #include "../../shared_memory/shared_memory.h"

#define FIFO_FILE_ONE   "/tmp/fifo_twoway1"
#define FIFO_FILE_TWO   "/tmp/fifo_twoway2"


void sighup();


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
int ipcb_open_shm (const char* name, unsigned long long truncationSize);
