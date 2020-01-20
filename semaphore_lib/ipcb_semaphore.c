
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include "../commons/commons.h"
#include "ipcb_semaphore.h"


int 
ipcb_get_semaphore(key_t key, int nSems, int flag) {
    int id = semget(key, nSems, flag);
    if(id < 0) {
        ipcb_print_error("ipcb_get_semaphore"); 
        exit(11);
    }
    return id;
}

int 
ipcb_control_semaphore(int semId, int semNum, int cmd, 
                        union semun semun) {

    int id = semctl(semId, semNum, cmd, semun);
    if(id < 0) {
        ipcb_print_error("ipcb_control_semaphore"); 
        exit(11);
    }
    return id;
}


int 
ipcb_operate_semaphore(int semId, struct sembuf *sOps, size_t nSOps) {
    if(semop(semId, sOps, nSOps) < 0) {
        if (sOps->sem_op == +1) {    
            ipcb_print_error("ipcb_operate_semaphore:increase"); 
            exit(15);
        }
        else if(sOps->sem_op == -1) {
            ipcb_print_error("ipcb_operate_semaphore:decrease"); 
            exit(16);
        }
        else{
            ipcb_print_error("ipcb_operate_semaphore"); 
            exit(17);
        }
    }
    return EXIT_SUCCESS;
} 