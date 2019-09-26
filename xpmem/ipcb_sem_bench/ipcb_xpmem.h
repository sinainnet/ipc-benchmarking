/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: xpmem/ipcb_sem_bench/ipcb_xpmem_.h
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include "xpmem_test.h"
#include "../../commons/commons.h"
#include"../../semaphore_lib/ipcb_semaphore.h"

#define SEM_KEY                   0x1111
#define UMS_KEY					  0x2222
#define ull                   unsigned long long
#define MW_LOCK_FILE          "/tmp/ipcb_mw_xpmem.lock"
#define RW_LOCK_FILE          "/tmp/ipcb_rw_xpmem.lock"
#define RW_SHARE_FILE         "/tmp/ipcb_xpmem.share"
 
key_t shared_sem_key = SEM_KEY;
key_t shared_ums_key = UMS_KEY;
struct sembuf decrease = DECREASE_SEM;
struct sembuf increase = INCREASE_SEM;



/*
 *  Lorem Ipsum
 */
int
ipcb_xpmem_arg_generator (ull memorySize, test_args* xpmem_args) {
    if ((xpmem_args->fd = open(RW_SHARE_FILE, O_RDWR)) == -1) {
		perror("open xpmem.share");
		return -1;
	}
    xpmem_args->share = mmap(0, TMP_SHARE_SIZE, PROT_READ|PROT_WRITE,
			MAP_SHARED, xpmem_args->fd, 0);
    if (xpmem_args->share == MAP_FAILED) {
		perror("mmap");
		return -1;
	}

    if ((xpmem_args->MWLock = open(MW_LOCK_FILE, O_RDWR | O_CREAT)) == -1) {
		perror("open xpmem.lock");
		return -1;
	}

    if ((xpmem_args->RWLock = open(RW_LOCK_FILE, O_RDWR | O_CREAT)) == -1) {
		perror("open xpmem.lock");
		return -1;
	}

    return 1;
}