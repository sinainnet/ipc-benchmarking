#include <sys/sem.h>

#define DECREASE_SEM { 0, -1, SEM_UNDO}
#define INCREASE_SEM { 0, +1, SEM_UNDO}

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};


int ipcb_get_semaphore(key_t key, int nSems, int flag);

int ipcb_control_semaphore(int semId, int semNum, int cmd, 
        union semun semun);

int ipcb_operate_semaphore(int semId, struct sembuf *sOps,
        size_t nSOps);
