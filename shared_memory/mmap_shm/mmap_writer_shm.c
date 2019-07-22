
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: mmap_shm/mmap_writer_shm.c
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
    int fd, fdSync, fdTime;
    char *str;
    char **buf;
    pid_t otherChildId;
    struct timeval start, end;

    buf = ipcb_fake_data_generator(SHM_ROW_SIZE, SHM_COL_SIZE);

    fd = ipcb_open_shm(SHM_FILE, memSize);
    str = ipcb_map_memory_to_fd(memSize, fd, 0);

    mkfifo(FIFO_FILE, 0666);
    fdSync = open(FIFO_FILE, O_RDONLY);

    // mkfifo(FIFO_FILE_TIME, 0666);
    // fdTime = open(FIFO_FILE_TIME, O_CREAT|O_TRUNC|O_WRONLY);

    sem_t *mutex = ipcb_open_semaphore();
    ipcb_wait_semaphore(mutex);
	
    read(fdSync, &otherChildId, sizeof(pid_t));

    ipcb_get_time(&start, "\n shm_writer:start: "); /* Start. */
    for (int i = 0; i < SHM_ROW_SIZE; i++)
        memcpy(str, buf[i], SHM_COL_SIZE);
    ipcb_get_time(&end, "\n shm_writer:end: ");  /* End. */
    
    ipcb_post_semaphore(mutex);

    // write(fdTime, &start.tv_sec, sizeof(long int));
    // write(fdTime, &start.tv_usec, sizeof(long int));
    
    printf("\nWriting Data into memory is done.\n");
    printf("Time in microseconds: %ld microseconds\n",
            ((end.tv_sec - start.tv_sec)*1000000L
           +end.tv_usec) - start.tv_usec
          ); // Added semicolon
    ipcb_unlink_semaphore("alaki");
    ipcb_close_semaphore(mutex);
    // ipcb_unlink_shm(SHM_FILE);

    return 0;
}
