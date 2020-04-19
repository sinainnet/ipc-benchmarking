// C program to demonstrate use of fork() and pipe() 
#include <time.h>
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

struct timespec start, finish;

void print_result(struct timespec start, struct timespec finish) {
        long seconds = finish.tv_sec - start.tv_sec;
        long ns = finish.tv_nsec - start.tv_nsec;
        if (start.tv_nsec > finish.tv_nsec) { 
				// clock underflow
                --seconds;
                ns += 1000000000;
        }
        printf("seconds without ns: %ld\n", seconds);
        printf("nanoseconds: %ld\n", ns);
        printf("total seconds: %e\n", (double)seconds + (double)ns/(double)1000000000);

        return;
}

int main() 
{ 
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
	int fd2[2];  
	char *data  = calloc(2048576, 1024);
	char *data2 = calloc(2048576, 1024);
	memset(data, 'a', 1048576*1024);

    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 

	if (pipe(fd2)==-1)
	{
		fprintf(stderr, "Pipe Failed" );
		return 1;
	}

    p = fork(); 
  
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
        close(fd1[0]);  // Close reading end of first pipe 
  
        // Write input string and close writing end of first 
        // pipe. 
		printf("1\n");
		clock_gettime(CLOCK_REALTIME, &start);
        write(fd1[1], data, 1048576*1024 + 1); 
		printf("1\n");
		clock_gettime(CLOCK_REALTIME, &finish);
		printf("1\n");
		print_result(start, finish);
		printf("1\n");
        close(fd1[1]); 
  
        // Wait for child to send a string 
        wait(NULL); 
		char sd[100]; 
		read(fd2[0], sd, 100);
    } 
  
    // child process 
    else
    {
		for(int i = 0; i< 10000000000;i++);
		printf("2\n");
        close(fd1[1]);  // Close writing end of first pipe 
		printf("2\n");
        // Read a string using first pipe 
        read(fd1[0], data2, 100); 
		printf("2\n");
        // Close both reading ends 
        close(fd1[0]); 
  		printf("2\n");
		write(fd2[1], "end", 1048576*1024+1);

        exit(0); 
    } 
}
