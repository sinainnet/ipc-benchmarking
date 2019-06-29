
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
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
static int 
__ipcb_check_pipe(int pfd) {
	struct stat sb;

	if (fstat(pfd, &sb) < 0)
		return ipcb_print_error("stat");
	if (!S_ISFIFO(sb.st_mode))
		return 1;

	return 0;
}


/*
 *  Lorem Ipsum
 */
static int 
ipcb_check_input_pipe(void) {
	if (!__ipcb_check_pipe(STDIN_FILENO))
		return 0;
	fprintf(stderr, "stdin must be a pipe\n");
	return 1;
}



/*
 *  Lorem Ipsum
 */
static int 
ipcb_check_output_pipe(void) {
	if (!__ipcb_check_pipe(STDOUT_FILENO))
		return 0;
	fprintf(stderr, "stdout must be a pipe\n");
	return 1;
}



/*
 *  Lorem Ipsum
 */
void 
ipcb_test_string_to_askii() {
    int a = 'a';
    int A = 'A';
    int z = 'z';
    int Z = 'Z';
    printf("a = %d, A = %d, z = %d, Z = %d\n", a,A,z,Z);
    return;
}
