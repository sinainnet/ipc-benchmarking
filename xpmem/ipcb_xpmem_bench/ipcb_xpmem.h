
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: ipcb_xpmem_bench/ipcb_xpmem.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <xpmem_test.h>

#define XPMEM_ROW_SIZE        ((1ull) << 10)
#define XPMEM_COL_SIZE        ((1ull) << 20)
#define XPMEM_FILE            "ipcb-xpmem-file"
#define FIFO_FILE           "/tmp/fifo_twoway-shm"
#define FIFO_FILE_TIME      "/tmp/fifo_starttime-shm"

#define SHARE_FILE          "/tmp/ipcb_xpmem.share"
#define LOCK_FILE           "/tmp/ipcb_xpmem.lock"


/*
 *  Lorem Ipsum
 */
char* ipcb_map_memory_to_fd (unsigned long long memorySize, int fd, off_t offset);


int ipcb_test_base_one (test_args *xpmem_args);