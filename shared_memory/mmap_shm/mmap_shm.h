
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: mmap_shm/mmap_shm.h
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */



#include "../shared_memory.h"


#define SHM_ROW_SIZE        ((1ull) << 10)
#define SHM_COL_SIZE        ((1ull) << 20)
#define SHM_FILE            "ipcb-shm-file"
#define FIFO_FILE           "/tmp/fifo_twoway-shm"
#define FIFO_FILE_TIME      "/tmp/fifo_starttime-shm"
