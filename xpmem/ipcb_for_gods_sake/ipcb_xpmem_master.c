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
        if (execl("./ipcb_xpmem_writer", "1", "proc1", (char*)NULL) == -1) {
            perror("execl p1");
            return -1;
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
        	if (execl("./ipcb_xpmem_reader", "2", "proc2", (char*)NULL) == -1) {
            		perror("execl p2");
            		return -1;
        	}
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
