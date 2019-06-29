
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
printError(const char* n) {
        perror(n);
        return -1;
}

static int __CheckPipe(int pfd);

static int checkInputPipe(void);

static int checkOutputPipe(void);

static void testStringAskii();
