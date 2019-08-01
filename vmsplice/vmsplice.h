/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: vmsplice/vmsplice.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

//#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include "../commons/commons.h"


/*
 *  Lorem Ipsum
 */
int
ipcb_vmsplicer(int fd, char** data, int rowSize, int colSize, int flag);


/*
 *  Lorem Ipsum
 */
int
ipcb_splicer(int fdIn, int fdOut, int rowSize, int colSize, int flag);
