
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: commons/commons.h
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#ifndef COMMONS_H
#define COMMONS_H

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/time.h>


#endif /* COMMONS_H */

#define    ON_ERROR      (-1)
#define    ON_SUCCESS    ( 0)


//inline int printError(const char* n) __attribute__((always_inline));

static inline int
ipcb_print_error (const char* n) {
        perror(n);
        return -1;
}


static inline void 
ipcb_get_time (struct timeval* timeVal,char* whoAmI) {
        gettimeofday(timeVal, NULL);
        printf("%s: \t %lu : %lu ", whoAmI, timeVal->tv_sec, timeVal->tv_usec);
        return;
}

static int __ipcb_check_pipe(int pfd);


static int ipcb_check_input_pipe(void);


static int ipcb_check_output_pipe(void);


static void ipcb_string_to_askii();


void ipcb_random_char_generator (char* ch);


void ipcb_initializer (char** buffer, unsigned long int row, unsigned long int col);


char** ipcb_empty_allocator (unsigned long row, unsigned long col);


char** ipcb_fake_data_generator (unsigned long row, unsigned long col);


pid_t ipcb_fork();


int ipcb_pipe(int* pip);


void ipcb_free_memory(char** mem, unsigned long int row);
