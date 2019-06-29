
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology 
 ****************************************************************************
 *
 *      File: shared_memory/shared_memory.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <errno.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/mman.h>        /* For POSIX memory map constants */
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


/*
 *  Lorem Ipsum
 */
int 
ipcb_cntrl_shm (int shmId) {
    int res;
    
    res = shmctl(shmId,IPC_RMID,NULL); 
    if( ON_ERROR == res )
        return ipcb_print_error("ipcShmCtl");
    return ON_SUCCESS;
}


/*
 *  Lorem Ipsum
 */
int
ipcb_open_shm (const char* name, unsigned long long truncationSize) {
    int fd;
    
    fd = shm_open("myshm", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) ipcb_print_error("ipcb_open_shm:shm_open");
    
    if((ftruncate(fd, 2ull * truncationSize)<0)) {
            printf("Ftruncate failed\n");
            return -1;
    }
    else{ return 1; }
}


/*
 *  Lorem Ipsum
 */
char*
ipcb_map_memory_to_fd (unsigned long long memorySize, int fd, off_t offset) {
    char* str;
    str = mmap(NULL, 2ull * memorySize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, offset);
    if (!str) ipcb_print_error("ipcb_map_memory_to_fd: mmap");
    return str;
}


/*
 *  Lorem Ipsum
 */
int 
ipcb_unmapp_memory (void *addr, size_t length) {
    int res;
    res = munmap(addr, length);
    if (0 != res) ipcb_print_error("ipcb_unmapp_memory:munmap");
    return res;    
}


