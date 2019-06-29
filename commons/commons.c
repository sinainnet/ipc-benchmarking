
/****************************************************************************
 * (C) 2019-2020 - DSLab @ Iran University of Science and Technology
 ****************************************************************************
 *
 *      File: shmat/commons.c
 *      Authors: Amir Hossein Sorouri - Sina Mahmoodi
 *
 * Description: 
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "commons.h"


/*
 *  Lorem Ipsum
 */
static int 
__ipcb_check_pipe (int pfd) {
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
ipcb_check_input_pipe (void) {
	if (!__ipcb_check_pipe(STDIN_FILENO))
		return 0;
	fprintf(stderr, "stdin must be a pipe\n");
	return 1;
}


/*
 *  Lorem Ipsum
 */
static int 
ipcb_check_output_pipe (void) {
	if (!__ipcb_check_pipe(STDOUT_FILENO))
		return 0;
	fprintf(stderr, "stdout must be a pipe\n");
	return 1;
}


/*
 *  Lorem Ipsum
 */
void 
ipcb_test_string_to_askii () {
    int a = 'a';
    int A = 'A';
    int z = 'z';
    int Z = 'Z';
    printf("a = %d, A = %d, z = %d, Z = %d\n", a,A,z,Z);
    return;
}


/*
 *  Lorem Ipsum
 */
void 
ipcb_random_char_generator (char* ch)
{
    time_t t;
    int chars [58];

    srand((unsigned) time(&t));
    for (int i = 0; i < 58; i++){
        chars[i] = (rand() % 58) + 65;
        ch[i] = chars[i];
    }
    return;
}


/*
 *  Lorem Ipsum
 */
void 
ipcb_initializer (char** buffer, unsigned long int row, unsigned long int col) {
    char ch[58];
    
    ipcb_random_char_generator(ch);
    for (int i = 0; i < row; i++){
	memset(buffer[i], ch[(rand() % 58)], col);
    }
    return;
}


/*
 *  Lorem Ipsum
 */
char** 
ipcb_empty_allocator (unsigned long int row, unsigned long int col) {
    char** buf;
	
    buf =  malloc(sizeof(char*) * row);
    for(int i = 0; i < row ;i++)
        buf[i] = malloc(sizeof(char) * col);
    return buf;
}


/*
 *  Lorem Ipsum
 */
char** 
ipcb_fake_data_generator (unsigned long int row, unsigned long int col) {
    char** buf;

    buf = ipcb_empty_allocator(row, col);
    ipcb_initializer(buf, row, col);
	return buf;
}
