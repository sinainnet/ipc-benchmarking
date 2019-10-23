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

    int mgrow = 1024;
    int gigrow = 1048576;
    long int two_gigrow = 2*gigrow;
    long int three_gigrow = 3*gigrow;
    long int four_gigrow = 4*gigrow;
    long int eight_gigrow = 8*gigrow;
    int col = 1024;

    unsigned long int mgsize = mgrow * col;
    unsigned long int gigsize = gigrow * col;
    unsigned long int two_gigsize = two_gigrow * col;
    unsigned long int three_gigsize = three_gigrow * col;
    unsigned long int four_gigsize = four_gigrow * col;
    unsigned long int eight_gigsize = eight_gigrow * col;

    int id_sem = ipcb_get_semaphore(shared_sem_key, 1, 0666 | IPC_CREAT);
    ipcb_control_semaphore(id_sem, 0, SETVAL, u);
    ipcb_operate_semaphore(id_sem, &decrease, 1);

    char *data = calloc(gigrow, col);
    memset(data, 'a', gigsize);
    printf("%d %p %lu \n", getpid(), data, gigsize);

    // while (1);
    ipcb_operate_semaphore(id_sem, &increase, 1); 

    int id_wrt = ipcb_get_semaphore(shared_wrt_key, 1, 0666 | IPC_CREAT);
    ipcb_control_semaphore(id_wrt, 0, SETVAL, j);
    ipcb_operate_semaphore(id_wrt, &decrease, 1);
    ipcb_operate_semaphore(id_wrt, &increase, 1);

    return 0;
}