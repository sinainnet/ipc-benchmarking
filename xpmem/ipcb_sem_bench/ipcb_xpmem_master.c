#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

#include "ipcb_xpmem.h"

int ipcb_test_base_one(test_args* t) { return 0; }
int ipcb_test_base_two(test_args* t) { return 0; }

int 
main(int argc, char **argv) {
    int pid, pid2,
		id, fd,
		lock,
		status[2] = {0,0};
	union semun u;
	u.val = 1;

	test_args xpmem_args;
	ipcb_xpmem_arg_generator(TMP_SHARE_SIZE, &xpmem_args);
	
	pid =  ipcb_fork();
    srand(pid);

	if(pid)
    {   
		id = ipcb_get_semaphore(shared_sem_key, 1, 0666 | IPC_CREAT);

		ipcb_control_semaphore(id, 0, SETVAL, u);
		ipcb_operate_semaphore(id, &decrease, 1);
		
		system("./ipcb_xpmem_writer 1 proc1");
		printf("writer: Goodbye.\n");
		exit(1);
    }
    else
    {			
		pid2 =  ipcb_fork();
		srand(pid2);
		if(pid2) {
			system("./ipcb_xpmem_reader 2 proc2");
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
