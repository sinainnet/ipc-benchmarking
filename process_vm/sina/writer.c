#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "sina.h"

int ipcb_test_base_one(test_args* t) { return 0; }


int main()
{
    union semun u, j;
	u.val = 1;
    j.val = 0;

    int id_sem = ipcb_get_semaphore(shared_sem_key, 1, 0666 | IPC_CREAT);
    ipcb_control_semaphore(id_sem, 0, SETVAL, u);
    ipcb_operate_semaphore(id_sem, &decrease, 1);

    char *data = calloc(1024, 1024);
    memset(data, 'a', 8191);
    printf("%d %p %u \n", getpid(), data, 1024 * 1024);

    // while (1);
    ipcb_operate_semaphore(id_sem, &increase, 1); 

    int id_wrt = ipcb_get_semaphore(shared_wrt_key, 1, 0666 | IPC_CREAT);
    ipcb_control_semaphore(id_wrt, 0, SETVAL, j);
    ipcb_operate_semaphore(id_wrt, &decrease, 1);
    ipcb_operate_semaphore(id_wrt, &increase, 1);

    return 0;
}