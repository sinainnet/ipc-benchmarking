
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: mmap_sem_shm/shm_mastesr.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */
// 
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <wait.h>

#include "mmap_shm.h"


int ipcb_test_base_one(test_args* t) { return 0; }

int 
main(int argc, char **argv) {
    int pid, pid2,
		id, fd,
		lock,
		status[2] = {0,0};
	union semun u;
	u.val = 1;
	
	pid =  ipcb_fork();
    srand(pid);

	if(pid)
    {   
		id = ipcb_get_semaphore(shared_sem_key, 1, 0666 | IPC_CREAT);

		ipcb_control_semaphore(id, 0, SETVAL, u);
		ipcb_operate_semaphore(id, &decrease, 1);
		
		system("./shm_writer 1 proc1");
		printf("writer: Goodbye.\n");
		exit(1);
    }
    else
    {			
		pid2 =  ipcb_fork();
		srand(pid2);
		if(pid2) {
			system("./shm_reader 2 proc2");
			printf("reader: Goodbye.\n");
			exit(1);
    	}
		else{
			waitpid(pid2, &status[1], 0);
			status[1] = WEXITSTATUS(status[1]);

			waitpid(pid, &status[0], 0);
			status[0] = WEXITSTATUS(status[0]);
			if ( WIFEXITED(status[1]) && WIFEXITED(status[0]) ) {
                printf("Both have Exited normally with status %d\n", status[1]);
            }
			printf("master: Goodbye.\n");

			return 0;
		}
    }
}
