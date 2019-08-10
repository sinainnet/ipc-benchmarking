#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
// #include <string.h>
#include <sys/types.h>
// #include <sys/ipc.h>
#include <wait.h>

#include "ipcb_xpmem.h"

int ipcb_test_base_one(test_args* t) { return 0; }

int 
main(int argc, char **argv) {
    int pid, pid2,
		id,
		lock,
		status[2];
	union semun u;
	u.val = 1;

    if ((lock = open(MW_LOCK_FILE, O_RDWR | O_CREAT)) == -1) {
        perror("open xpmem.lock");
        return -1;
	}
	
	pid =  ipcb_fork();
    srand(pid);

	if(pid)
    {   
		lockf(lock, F_LOCK, 0);
		id = ipcb_get_semaphore(shared_sem_key, 1, 0666 | IPC_CREAT);

		ipcb_control_semaphore(id, 0, SETVAL, u);
		ipcb_operate_semaphore(id, &decrease, 1);
		
		system("./ipcb_xpmem_writer 1 proc1");

		ipcb_operate_semaphore(id, &increase, 1);
    }
    else
    {
		pid2 =  ipcb_fork();
		srand(pid2);
		if(pid2) {
			// printf("hellowwwww");
			system("./ipcb_xpmem_reader 2 proc2");
    	}
		else{
			waitpid(pid2, &status[1], 0);
			status[1] = WEXITSTATUS(status[1]);

			waitpid(pid, &status[0], 0);
			status[0] = WEXITSTATUS(status[0]);
			return 0;
		}
    }
}
