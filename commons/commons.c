
/****************************************************************************
 * (C) 2019-2020 - Amir Hossein Sorouri - Sina Mahmoodi
 ****************************************************************************
 *
 *      File: shmat/commons.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include "commons.h"


/*
 *  Lorem Ipsum
 */
static inline int 
printError(const char* n) {
	perror(n);
	return -1;
}


/*
 *  Lorem Ipsum
 */
static int 
__CheckPipe(int pfd) {
	struct stat sb;

	if (fstat(pfd, &sb) < 0)
		return printError("stat");
	if (!S_ISFIFO(sb.st_mode))
		return 1;

	return 0;
}


/*
 *  Lorem Ipsum
 */
static inline int 
checkInputPipe(void) {
	if (!__CheckPipe(STDIN_FILENO))
		return 0;

	fprintf(stderr, "stdin must be a pipe\n");
	return 1;
}



/*
 *  Lorem Ipsum
 */
static inline int 
checkOutputPipe(void) {
	if (!__CheckPipe(STDOUT_FILENO))
		return 0;

	fprintf(stderr, "stdout must be a pipe\n");
	return 1;
}



/*
 *  Lorem Ipsum
 */
static inline void 
testStringAskii() {
    int a = 'a';
    int A = 'A';
    int z = 'z';
    int Z = 'Z';
    printf("a = %d, A = %d, z = %d, Z = %d\n", a,A,z,Z);
    return;
}