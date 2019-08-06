
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: ipcb_xpmem_bench/ipcb_xpmem.h
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include "xpmem_test.h"
#include "../../commons/commons.h"

#define ull                   unsigned long long
#define XPMEM_ROW_SIZE        ((1ull) << 3)
#define XPMEM_COL_SIZE        ((1ull) << 3)
#define FIFO_FILE_TIME        "/tmp/fifo_starttime-shm"
#define XPMEM_FILE            "ipcb-xpmem-file"
#define FIFO_FILE             "/tmp/fifo_twoway-shm"
#define LOCK_FILE             "/tmp/ipcb_xpmem.lock"
#define SHARE_FILE            "/tmp/ipcb_xpmem.share"

extern struct timeval start, end;


/*
 *  Lorem Ipsum
 */
char* ipcb_map_memory_to_fd (ull memorySize, int fd, off_t offset);


/*
 *  Lorem Ipsum
 */
int ipcb_test_base_one (test_args *xpmem_args);


/*
 *  Lorem Ipsum
 */
int ipcb_xpmem_arg_generator (ull memorySize, test_args* xpmem_args);


// /*
//  *  Lorem Ipsum
//  */
// int
// ipcb_xpmem_arg_generator (ull memorySize, test_args* xpmem_args) {

// 	if ((xpmem_args->fd = open(SHARE_FILE, O_RDWR)) == -1)
// 		return ipcb_print_error("open ipcb_xpmem.share");

// 	if ((xpmem_args->lock = open(LOCK_FILE, O_RDWR)) == -1)
// 		return ipcb_print_error("open ipcb_xpmem.lock");

// 	xpmem_args->share = ipcb_map_memory_to_fd(memSize, xpmem_args->fd, 0); 
//     return 1;
// }
