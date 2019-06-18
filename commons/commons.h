
/****************************************************************************
 * (C) 2019-2020 - Amir Hossein Sorouri - Sina Mahmoodi
 ****************************************************************************
 *
 *      File: shmat/commons.h
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


static inline int printError(const char* n);

static int __CheckPipe(int pfd);

static inline int checkInputPipe(void);

static inline int checkOutputPipe(void);

static inline void testStringAskii();