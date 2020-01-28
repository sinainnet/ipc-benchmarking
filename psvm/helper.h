/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: process_vm_sem/psvm_sem.h
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#define _GNU_SOURCE
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

#include <sys/mman.h>

#include"../semaphore_lib/ipcb_semaphore.h"

#define SEM_KEY                   0x1111
#define WRT_KEY					  0x2223
#define ChildId_KEY				  0x3333
#define PSVM_ROW_SIZE        ((1ull) << 10)
#define PSVM_COL_SIZE        ((1ull) << 10)
#define TMP_SHARE_SIZE       PSVM_ROW_SIZE*PSVM_COL_SIZE
#define PSVM_FILE_ONE        "/tmp/ipcb_psvm_file_1"
#define PSVM_FILE_TWO        "/tmp/ipcb_psvm_file_2"
#define FIFO_FILE            "/tmp/fifo_twoway-shm"
#define FIFO_FILE_TIME       "/tmp/fifo_starttime-shm"


key_t shared_sem_key = SEM_KEY;
key_t shared_wrt_key = WRT_KEY;
key_t shared_otcid_key = ChildId_KEY;
struct sembuf decrease = DECREASE_SEM;
struct sembuf increase = INCREASE_SEM;
struct sembuf decrease_two = DECREASE_SEM_TWO;
struct sembuf increase_two = INCREASE_SEM_TWO;

