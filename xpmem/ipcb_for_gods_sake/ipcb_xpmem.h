#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include "xpmem_test.h"
#include "../../commons/commons.h"
#include"../../semaphore_lib/ipcb_semaphore.h"

#define KEY                   0x1111
#define ull                   unsigned long long
#define MW_LOCK_FILE             "/tmp/ipcb_mw_xpmem.lock"
#define RW_LOCK_FILE             "/tmp/ipcb_rw_xpmem.lock"
#define XPMEM_ROW_SIZE        ((1ull) << 3)
#define XPMEM_COL_SIZE        ((1ull) << 3)

 
key_t shared_sem_key = KEY;
struct sembuf decrease = DECREASE_SEM;
struct sembuf increase = INCREASE_SEM;

