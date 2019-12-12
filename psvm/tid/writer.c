#include<stdio.h>
#define _GNU_SOURCE
#include <sys/syscall.h>
#include<stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

#define gettid() syscall(SYS_gettid)

#define Pthread_create(thread, attr, start_routine, arg) assert(pthread_create(thread, attr, start_routine, arg) == 0);
#define Pthread_join(thread, value_ptr)                  assert(pthread_join(thread, value_ptr) == 0);

int max;
volatile int counter = 0; // shared global variable

void *mythread(void *arg) {
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

    char *data = calloc(mgrow, col);
    memset(data, 'a', mgsize);
    printf("%ld %p %lu \n", gettid(), data, mgsize);
    while (1);

    return NULL;
}

int main()
{       
        pthread_t p1, p2;
        Pthread_create(&p1, NULL, mythread, "A"); 
        
        Pthread_join(p1, NULL); // join waits for the threads to finish
        
        return 0;
}