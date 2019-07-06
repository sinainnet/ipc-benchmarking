/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: writev/writv.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "../process_vm.h"

#define PS_VM_ROW_SIZE     ((1ull) << 10)
#define PS_VM_COL_SIZE     ((1ull) << 20)

const unsigned long int memSize = ( PS_VM_ROW_SIZE * PS_VM_COL_SIZE );

void sighup();

/*
 *  Lorem Ipsum
 */
int
main() {
	int fd, nread;
	char *str;
        char **buf, **emptyBuf;
	pid_t childPid, childOnePid;
        struct timeval start, end;
	int pidPipe[2], bufPipe[2];

	ipcb_pipe(pidPipe);
	ipcb_pipe(bufPipe);

	childPid = ipcb_fork();
	if (childPid == 0) {
		/* First Child process does the ps_vm process of 
		 * writing into another process. */
                char* name = "First Child Process";
		pid_t otherChildId;

		buf = ipcb_fake_data_generator(PS_VM_ROW_SIZE, PS_VM_COL_SIZE);

		read(pidPipe[0], &otherChildId, sizeof(pid_t));
		read(bufPipe[0], &emptyBuf, sizeof(char**));
	
		ipcb_get_time(&start, "\nfirst_child:start: "); /* Start. */
		
		nread = ipcb_process_vm_writev(otherChildId, buf, 
						PS_VM_ROW_SIZE, PS_VM_COL_SIZE,
						emptyBuf, PS_VM_ROW_SIZE, PS_VM_COL_SIZE);

		ipcb_get_time(&end, "\nfirst_child:end: ");  /* End. */


		printf("\nWriting Data into memory is done.\n");
    		printf("Time in microseconds: %ld microseconds\n",
            		((end.tv_sec - start.tv_sec)*1000000L
	           	+end.tv_usec) - start.tv_usec
          		); // Added semicolon
		kill(otherChildId, SIGHUP);
	}
	else {
		childOnePid = ipcb_fork();
		if (childOnePid == 0) {
			char* name = "Second Child Process";
                        pid_t myPid;
			emptyBuf = ipcb_empty_allocator(PS_VM_ROW_SIZE, PS_VM_COL_SIZE);

			myPid = getpid();
                        write(pidPipe[1], &myPid, sizeof(pid_t));
                        write(bufPipe[1], &emptyBuf, sizeof(char**));

			signal(SIGHUP, sighup);
	                pause();
		}
		else {
			char* name = "Parent Process";
			int childStatus, childOneStatus;

			waitpid(childPid, &childStatus, 0);
			waitpid(childOnePid, &childOneStatus, 0);

			if (childStatus == 0 && childOneStatus == 0)  // Verify child process terminated without error. 
                	{
                        	printf("The child process terminated normally.\n");
                	}
                	else{printf("The child process terminated with an error!.\n");}
                	exit(0);
		}
	}
	return 0;
}
		

void sighup()
{
    signal(SIGHUP, sighup); /* reset signal */
    printf("CHILD: I have received a SIGHUP\n");
}

