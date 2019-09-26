#include <sys/sem.h>

#define DECREASE_SEM { 0, -1, SEM_UNDO}
#define INCREASE_SEM { 0, +1, SEM_UNDO}

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};


/*
 * Function:   ipcb_get_semaphore === semget()
 * --------------------------------------------
 * The semget() system call returns the System V semaphore set
 * identifier associated with the argument key.  It may be used either
 * to obtain the identifier of a previously created semaphore set (when
 * semflg is zero and key does not have the value IPC_PRIVATE), or to
 * create a new set.
 * 
 * The argument nsems can be 0 (a don't care) when a semaphore set is
 * not being created.  Otherwise, nsems must be greater than 0 and less
 * than or equal to the maximum number of semaphores per semaphore set
 * (SEMMSL).
 * 
 * returns:     
 *      If successful, the return value will be the semaphore set identifier
 *      (a nonnegative integer), otherwise, -1 is returned, with errno
 *      indicating the error.
 */
int ipcb_get_semaphore(key_t key, int nSems, int flag);


/*
 * Function:   ipcb_control_semaphore === semctl()
 * --------------------------------------------
 * semctl() performs the control operation specified by cmd on the
 * System V semaphore set identified by semid, or on the semnum-th
 * semaphore of that set.  (The semaphores in a set are numbered
 * starting at 0.)
 * 
 * This function has three or four arguments, depending on cmd.  When
 * there are four, the fourth has the type union semun.  The calling
 * program must define this union as follows ...
 * 
 * returns:     
 *      On failure, semctl() returns -1 with errno indicating the error.
 *      Otherwise, the system call returns a nonnegative value depending on
 *      cmd
 */
int ipcb_control_semaphore(int semId, int semNum, int cmd, 
        union semun semun);


/*
 * Function:   ipcb_operate_semaphore === semop()
 * --------------------------------------------
 * semop() performs operations on selected semaphores in the set indi‐
 * cated by semid.  Each of the nsops elements in the array pointed to
 * by sops is a structure that specifies an operation to be performed on
 * a single semaphore.
 * 
 * Flags recognized in sem_flg are IPC_NOWAIT and SEM_UNDO.  If an oper‐
 * ation specifies SEM_UNDO, it will be automatically undone when the
 * process terminates.

 * The set of operations contained in sops is performed in array order,
 * and atomically, that is, the operations are performed either as a
 * complete unit, or not at all.  The behavior of the system call if not
 * all operations can be performed immediately depends on the presence
 * of the IPC_NOWAIT flag in the individual sem_flg fields, as noted
 * below.
 *
 * Each operation is performed on the sem_num-th semaphore of the sema‐
 * phore set, where the first semaphore of the set is numbered 0.  There
 * are three types of operation, distinguished by the value of sem_op.
 * 
 * returns:     
 *      If successful, semop() and semtimedop() return 0; otherwise they
 *      return -1 with errno indicating the error.
 */
int ipcb_operate_semaphore(int semId, struct sembuf *sOps,
        size_t nSOps);
