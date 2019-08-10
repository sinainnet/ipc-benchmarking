#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
// #include <string.h>
#include <sys/types.h>
// #include <sys/ipc.h>
#include <wait.h>
#include "sem.h"

#include "../../commons/commons.h"


int 
main()
{
    int pid, lock, status[2];
    pid =  fork();
    srand(pid);
    if ((lock = open("/tmp/xpmem.lock", O_RDWR | O_CREAT)) == -1) {
        perror("open xpmem.lock");
        return -1;
	}
    if(pid < 0)
	{
        perror("fork"); exit(1);
    }
    else if(pid)
    {   
		lockf(lock, F_LOCK, 0);
        // if (execl("./ipcb_xpmem_writer", "1", "proc1", (char*)NULL) == -1) {
        //     perror("execl p1");
        //     return -1;
        // }
		int id = semget(KEY, 1, 0666 | IPC_CREAT);
		if(id < 0)
		{
			perror("semget"); exit(11);
		}
		union semun u;
		u.val = 1;
		if(semctl(id, 0, SETVAL, u) < 0)
		{
			perror("semctl"); exit(12);
		}
		if(semop(id, &p, 1) < 0){
			perror("semop p"); exit(15);
		}
		system("./ipcb_xpmem_writer 1 proc1");
		if(semop(id, &v, 1) < 0){
			perror("semop p"); exit(16);
		}
    }
    else
    {
		int pid2;
			pid2 =  fork();
		srand(pid2);
		if(pid2 < 0)
    	{
        	perror("fork"); exit(1);
    	}
    	else if(pid2)
    	{
        	// if (execl("./ipcb_xpmem_reader", "2", "proc2", (char*)NULL) == -1) {
            // 		perror("execl p2");
            // 		return -1;
        	// }
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
