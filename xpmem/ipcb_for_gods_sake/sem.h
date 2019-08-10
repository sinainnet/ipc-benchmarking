// #if __STDC_VERSION__ >= 199901L
// #define _XOPEN_SOURCE 600
// #else
// #define _XOPEN_SOURCE 500
// #endif /* __STDC_VERSION__ */
// #include <sys/sem.h>

// #define KEY 0x1111

// union semun {
//     int val;
//     struct semid_ds *buf;
//     unsigned short  *array;
// };

// struct sembuf p = { 0, -1, SEM_UNDO};
// struct sembuf v = { 0, +1, SEM_UNDO};
