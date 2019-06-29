
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: shmat/shmat.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <stdio.h> 
#include <string.h>
#include "../shared_memory.h"

#define SHM_ROW_SIZE     ((1ull) << 10)
#define SHM_COL_SIZE     ((1ull) << 20)

const unsigned long int memSize = ( SHM_ROW_SIZE * SHM_COL_SIZE );


/*
 *  Lorem Ipsum
 */
int 
main() {
    int fd;
    char* str;
    char** buf;
    struct timeval start, end;

    buf = ipcb_fake_data_generator(SHM_ROW_SIZE, SHM_COL_SIZE);
    fd = ipcb_open_shm("shmfile", (SHM_ROW_SIZE*SHM_COL_SIZE));
    str = ipcb_map_memory_to_fd(memSize, fd, 0);

    ipcb_get_time(&start, "parent:start: ");
    for (int i = 0; i < SHM_ROW_SIZE; i++)
        memcpy(str, buf[i], SHM_COL_SIZE);
    ipcb_get_time(&end, "parent:end: ");
    
    printf("Data written in memory: %s\n","done");
    printf("Time in microseconds: %ld microseconds\n",
            ((start.tv_sec - end.tv_sec)*1000000L
           +start.tv_usec) - end.tv_usec
          ); // Added semicolon

    ipcb_unlink_shm("shmfile");
    return 0;
}
