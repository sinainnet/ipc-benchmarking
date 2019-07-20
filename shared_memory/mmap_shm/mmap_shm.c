
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
    fd = ipcb_open_shm("myshm", memSize);
    str = ipcb_map_memory_to_fd(memSize, fd, 0);

    ipcb_get_time(&start, "\nparent:start: "); /* Start. */

    for (int i = 0; i < SHM_ROW_SIZE; i++)
        memcpy(str, buf[1], SHM_COL_SIZE);
    
    ipcb_get_time(&end, "\nparent:end: ");  /* End. */
    
    printf("\nWriting Data into memory is done.\n");
    
    printf("Time in microseconds: %ld microseconds\n",
            ((end.tv_sec - start.tv_sec)*1000000L
           +end.tv_usec) - start.tv_usec
          ); // Added semicolon

    ipcb_unlink_shm("myshm");
    return 0;
}
