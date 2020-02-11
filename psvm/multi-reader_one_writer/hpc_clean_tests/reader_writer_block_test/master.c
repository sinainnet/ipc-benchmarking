/*
 * master: controller process for various IPCB tests
 *
 * Copyright (c) 2019 IUST-DSLab, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#define _GNU_SOURCE
#include <sched.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define TEST_NO		2
#define program1	"./reader"
#define program2	"./writer"
#define middleware	"./middleware.txt"


void 
ipcb_test() {
	pid_t program1_id, program2_id, p1, p2;
	int  status[2];
	FILE *file = fopen(middleware, "w+");
	fclose(file);

	p1 = fork();
	if (p1 == -1) {
		/* Error */
		perror("fork program_1");
		exit(1);
	}
	else if (p1 == 0) {
		/* Parent */
		program1_id = getpid();
		p2 = fork();
		if (p2 == -1) {
			/* Error */
			perror("fork program_2");
			exit(1);
		}
		else if (p2 == 0) {
			/* Child Runs Program 2*/
			program2_id = getpid();
			
			/* Trying to read required data to run
			 * program 2 from the middleware file. */
			FILE *file_res = fopen(middleware, "r");
			while (fgetc(file_res) == EOF){
				fclose(file_res);
				file_res = fopen(middleware, "r");
			}

			fclose(file_res);
			file_res = fopen(middleware, "r");
			
			char executive_data[50] = {};
			char *p = fgets((char*)&executive_data, 46, file_res);
			fclose(file_res);

			// Running Program 2
			printf("In child going to run %s.\n", executive_data);
			int s = system(executive_data);
			exit(0);
		}
		else {
			/* Parent Runs Program 1*/
			printf("In parent going to run ./reader: my pid is = %d\n", program1_id);
			if (execl(program1, "", NULL) == -1) {
				perror("execl program_1");
				exit(1);
			}

			/* Wait for the program 2*/
			waitpid(p2, &status[1], 0);
			status[1] = WEXITSTATUS(status[1]);
			return;
		}

	}
	else {
		/* Parent */
		waitpid(p1, &status[0], 0);
		status[0] = WEXITSTATUS(status[0]);
		return;
	}
}


int 
main(int argc, char** argv) {
	int cpu_no = 0;
	cpu_set_t set;
        CPU_ZERO(&set);
        CPU_SET(cpu_no, &set);
        if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
        {
                printf("master:setaffinity error");
                exit(0);
        }

	for (size_t i = 0; i < TEST_NO; i++)
	{
		printf("\n-------------------- %ld --------------------\n", i);
		ipcb_test();
	}
	return 0;
}
