/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: writev/writv.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include "psvm.h"

#define PS_VM_ROW_SIZE     ((1ull) << 11)
#define PS_VM_COL_SIZE     ((1ull) << 20)

const unsigned long int memSize = ( PS_VM_ROW_SIZE * PS_VM_COL_SIZE );



/*
 *  Lorem Ipsum
 */
int
main() {
        int fd,
            fdOne,
            fdTwo;
        int pidPipe[2], 
            bufPipe[2];
        char *str, 
             **buf, 
             *emptyBuf;
        pid_t childPid, 
              childOnePid;
        struct timeval start, 
                       end;

        mkfifo(FIFO_FILE_ONE, 0666);
        mkfifo(FIFO_FILE_TWO, 0666);

        fdOne = open(FIFO_FILE_ONE, O_CREAT|O_TRUNC|O_WRONLY);
        fdTwo = open(FIFO_FILE_TWO, O_CREAT|O_TRUNC|O_WRONLY);

	pid_t myPid;
	// emptyBuf = ipcb_fake_data_generator(PS_VM_ROW_SIZE, PS_VM_COL_SIZE);
    	fd = ipcb_open_shm("mypsvm", memSize);
   	emptyBuf = ipcb_map_memory_to_fd(memSize, fd, 0);

	myPid = getpid();
        printf("MPID: %d, pointer: %p\n", myPid, emptyBuf);

        write(fdOne, &myPid, sizeof(pid_t));
        write(fdTwo, &emptyBuf, sizeof(char**));

	signal(SIGHUP, sighup);
	pause();
	return 0;
}
	
void sighup()
{
    signal(SIGHUP, sighup); /* reset signal */
    printf("CHILD: I have received a SIGHUP\n");
}

/*
 *  Lorem Ipsum
 */
char*
ipcb_map_memory_to_fd (unsigned long long memorySize, int fd, off_t offset) {
    char* str;

    str = mmap(NULL, 2ull * memorySize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, offset);
    if (MAP_FAILED == str) ipcb_print_error("ipcb_map_memory_to_fd: mmap");
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


/*
 *  Lorem Ipsum
 */
int
ipcb_open_shm (const char* name, unsigned long long truncationSize) {
    int fd;
    
    fd = shm_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) ipcb_print_error("ipcb_open_shm:shm_open");
    
    if((ftruncate(fd, 10ull * truncationSize)<0)) {
            printf("Ftruncate failed\n");
            return -1;
    }
    else{ return fd; }
}
