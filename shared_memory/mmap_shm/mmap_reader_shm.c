
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: mmap_shm/mmap_reader_shm.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <stdio.h> 
#include <string.h>

#include "mmap_shm.h"

const unsigned long int memSize = ( SHM_ROW_SIZE * SHM_COL_SIZE );


/*
 *  Lorem Ipsum
 */
int 
main() {
    int fd, fdTime,
        fdSync;
    char *str;
    char **buf;
    long int sec, usec;
    struct timeval start, end;

    buf = ipcb_empty_allocator(SHM_ROW_SIZE, SHM_COL_SIZE);

    fd = ipcb_open_shm(SHM_FILE, memSize);
    str = ipcb_map_memory_to_fd(memSize, fd, 0);

    mkfifo(FIFO_FILE, 0666);
    fdSync = open(FIFO_FILE, O_CREAT|O_TRUNC|O_WRONLY);
    
    pid_t mypid = getpid();
    sem_t *mutex = ipcb_open_semaphore_other();
    
    write(fdSync, &mypid, sizeof(pid_t));
    ipcb_wait_semaphore(mutex);

    ipcb_get_time(&start, "\n shm_reader:start: "); /* Start. */
    
    for (int i = 0; i < SHM_ROW_SIZE; i++)
        memcpy(buf[i], str + (i*SHM_COL_SIZE), SHM_COL_SIZE);
    
    ipcb_get_time(&end, "\n shm_reader:end: ");  /* End. */
    
    ipcb_post_semaphore(mutex);
    
    // read(fdTime, &sec, sizeof(long int));
    // read(fdTime, &usec, sizeof(long int));
    
    printf("\nWriting Data into memory is done.\n");
    
    printf("Time in microseconds: %ld microseconds\n",
            ((end.tv_sec - start.tv_sec)*1000000L
           +end.tv_usec) - start.tv_usec
          ); // Added semicolon

    ipcb_unlink_shm(SHM_FILE);
    ipcb_close_semaphore(mutex);
    ipcb_destroy_semaphore(mutex);
    return 0;
}
