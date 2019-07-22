
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: shmat/commons.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "commons.h"


#define SEM_NAME "/semaphore_example25"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define INITIAL_VALUE 1


/*
 *  Lorem Ipsum
 */
static int 
__ipcb_check_pipe (int pfd) {
struct stat sb;

if (fstat(pfd, &sb) < 0)
	return ipcb_print_error("stat");
	if (!S_ISFIFO(sb.st_mode))
	return 1;

	return 0;
}


/*
 *  Lorem Ipsum
 */
static int 
ipcb_check_input_pipe (void) {
	if (!__ipcb_check_pipe(STDIN_FILENO))
		return 0;
	fprintf(stderr, "stdin must be a pipe\n");
	return 1;
}


/*
 *  Lorem Ipsum
 */
static int 
ipcb_check_output_pipe (void) {
	if (!__ipcb_check_pipe(STDOUT_FILENO))
		return 0;
	fprintf(stderr, "stdout must be a pipe\n");
	return 1;
}


/*
 *  Lorem Ipsum
 */
void 
ipcb_test_string_to_askii () {
    int a = 'a';
    int A = 'A';
    int z = 'z';
    int Z = 'Z';
    printf("a = %d, A = %d, z = %d, Z = %d\n", a,A,z,Z);
    return;
}


/*
 *  Lorem Ipsum
 */
void 
ipcb_random_char_generator (char* ch)
{
    time_t t;
    int chars [58];

    srand((unsigned) time(&t));
    for (int i = 0; i < 58; i++){
        chars[i] = (rand() % 58) + 65;
        ch[i] = chars[i];
    }
    return;
}


/*
 *  Lorem Ipsum
 */
void 
ipcb_initializer (char** buffer, unsigned long int row, unsigned long int col) {
    char ch[58];
    
    ipcb_random_char_generator(ch);
    for (int i = 0; i < row; i++){
	memset(buffer[i], ch[(rand() % 58)], col);
    }
    return;
}


/*
 *  Lorem Ipsum
 */
char** 
ipcb_empty_allocator (unsigned long int row, unsigned long int col) {
    char** buf;
	
    buf =  malloc(sizeof(char*) * row);
    for(int i = 0; i < row ;i++)
        buf[i] = malloc(sizeof(char) * col);
    return buf;
}


/*
 *  Lorem Ipsum
 */
char** 
ipcb_fake_data_generator (unsigned long int row, unsigned long int col) {
    char** buf;

    buf = ipcb_empty_allocator(row, col);
    ipcb_initializer(buf, row, col);
	return buf;
}


/*
 *  Lorem Ipsum
 */
pid_t 
ipcb_fork() {
	pid_t childPid;
	
	if((childPid = fork()) == ON_ERROR) {
                ipcb_print_error("fork");
		exit(1);
	}
	return childPid;
}


/*
 *  Lorem Ipsum
 */
int 
ipcb_pipe(int* pip) {
	if (pipe(pip) < 0) {
                ipcb_print_error("pipe");
		exit(1);
	}
	return ON_SUCCESS;
}


/*
 *  Lorem Ipsum
 */
void 
ipcb_free_memory(char** mem, unsigned long int row) {
    for(int i = 0; i < row; i++){
        mem[i];
    }
    free(mem);
    return;
}

 
/*
 *  Lorem Ipsum
 */
sem_t*
ipcb_open_semaphore() {
    /* We initialize the semaphore counter to 1 (INITIAL_VALUE) */
    sem_t *semaphore = sem_open(SEM_NAME, O_CREAT | O_EXCL, SEM_PERMS, INITIAL_VALUE);
    if (semaphore == SEM_FAILED) {
        ipcb_print_error("ipcb_open_semaphore:sem_open(3) error\n");
        exit(EXIT_FAILURE);
    }
    return semaphore;
}


/*
 *  Lorem Ipsum
 */
sem_t*
ipcb_open_semaphore_other() {
    /* We initialize the semaphore counter to 1 (INITIAL_VALUE) */
    sem_t *semaphore = sem_open(SEM_NAME, O_CREAT, SEM_PERMS, INITIAL_VALUE);
    if (semaphore == SEM_FAILED) {
        ipcb_print_error("ipcb_open_semaphore:sem_open(3) error\n");
        exit(EXIT_FAILURE);
    }
    return semaphore;
}


/*
 *  Lorem Ipsum
 */
int 
ipcb_close_semaphore(sem_t* semaphore) {
    /* Close the semaphore as we won't be using it in the parent process */
    if (sem_close(semaphore) < 0) {
        /* We ignore possible sem_unlink(3) errors here */
        sem_unlink(SEM_NAME);

        return ipcb_print_error("ipcb_open_semaphore:sem_close(3) failed\n");
    }
    return ON_SUCCESS;
}


/*
 *  Lorem Ipsum
 */
int
ipcb_unlink_semaphore(char* semName) {
    if (sem_unlink(SEM_NAME) < 0)
        return ipcb_print_error("ipcb_unlink_semaphore:sem_unlink(3) failed\n");
    return ON_SUCCESS;
}


/*
 *  Lorem Ipsum
 */
int
ipcb_post_semaphore(sem_t* semaphore) {
    if (sem_post(semaphore) < 0) {
        return ipcb_print_error("ipcb_post_semaphore:sem_post(3) error on child\n");
    }
    return ON_SUCCESS;
}


/*
 *  Lorem Ipsum
 */
int
ipcb_wait_semaphore(sem_t* semaphore) {
    if (sem_wait(semaphore) < 0) {
        return ipcb_print_error("ipcb_wait_semaphore:sem_wait(3) failed on child\n");
    }
    return ON_SUCCESS;
}


/*
 *  Lorem Ipsum
 */
int 
ipcb_destroy_semaphore(sem_t *semaphore) {
    if (sem_destroy(semaphore) < 0) {
        return ipcb_print_error("ipcb_destory_semaphore:sem_destroy(3) failed on child\n");
    }
    return ON_SUCCESS;
}