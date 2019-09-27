/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: process_vm_sem/psvm_sem_master.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */
// 
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <wait.h>

#include "psvm_sem.h"


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
        char command[50] = {};
        sprintf(command, "./psvm_sem_writer %ld", (long)getpid()); 
		
        id = ipcb_get_semaphore(shared_sem_key, 1, 0666 | IPC_CREAT);
		ipcb_control_semaphore(id, 0, SETVAL, u);
		ipcb_operate_semaphore(id, &decrease, 1);
		
		system(command);

		printf("writer: Goodbye. %ld\n", (long)getpid());
		exit(1);
    }
    else
    {			
		pid2 =  ipcb_fork();
		srand(pid2);
		if(pid2) {
            char command[50] = {};
            sprintf(command, "./psvm_sem_reader %ld", (long)getpid()); 
            
            int id = ipcb_get_semaphore(shared_otcid_key, 1, 0666 | IPC_CREAT);
            ipcb_control_semaphore(id, 0, SETVAL, u);
            ipcb_operate_semaphore(id, &decrease, 1);  

			system(command);
			
            printf("reader: Goodbye. %ld\n", (long)getpid());
			exit(1);
    	}
		else{
			int st0, st1;
            if(waitpid(pid2, &st1, 0) == 0 && waitpid(pid, &st0, 0) == 0){
                st1 = WEXITSTATUS(st1);
                st0 = WEXITSTATUS(st0);
                
                if ( WIFEXITED(st1) && WIFEXITED(st0) ) {
                    printf("Both have Exited normally with status %d\n", status[1]);
                }
                printf("master: Goodbye.\n");

                return 0;
            }
            else{
                return 0;

            }
			
            
		}
    }
}
