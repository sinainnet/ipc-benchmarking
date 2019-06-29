
/****************************************************************************
 * (C) 2019-2020 - Amir Hossein Sorouri - Sina Mahmoodi
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


#endif /* COMMONS_H */

#define    ON_ERROR      (-1)
#define    ON_SUCCESS    ( 0)


//inline int printError(const char* n) __attribute__((always_inline));

static inline int
ipcb_print_error(const char* n) {
        perror(n);
        return -1;
}

static int __ipcb_check_pipe(int pfd);

static int ipcb_check_input_pipe(void);

static int ipcb_check_output_pipe(void);

static void ipcb_string_to_askii();
