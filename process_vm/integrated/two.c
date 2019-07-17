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
#define PS_VM_COL_SIZE     ((1ull) << 21)

const unsigned long int memSize = ( PS_VM_ROW_SIZE * PS_VM_COL_SIZE );



/*
 *  Lorem Ipsum
 */
int
main() {
        int fdOne,
            fdTwo, 
            nread;
        int pidPipe[2], 
            bufPipe[2];
        char *str, 
             **buf, 
             **emptyBuf;
        pid_t childPid, 
              childOnePid;
        struct timeval start, 
                       end;

        mkfifo(FIFO_FILE_ONE, 0666);
        mkfifo(FIFO_FILE_TWO, 0666);

        fdOne = open(FIFO_FILE_ONE, O_CREAT|O_TRUNC|O_WRONLY);
        fdTwo = open(FIFO_FILE_TWO, O_CREAT|O_TRUNC|O_WRONLY);

	pid_t myPid;
	emptyBuf = ipcb_fake_data_generator(PS_VM_ROW_SIZE, PS_VM_COL_SIZE);
        // strcpy(emptyBuf[0], "end");
        // emptyBuf[0][0] = 'e';
        // emptyBuf[0][1] = 'n';
        // emptyBuf[0][2] = '\0';
        //printf("%s\n", emptyBuf[0]);

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
