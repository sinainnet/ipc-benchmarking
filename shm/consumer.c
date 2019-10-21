#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <sys/mman.h> 
#include <sys/stat.h> 
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <semaphore.h>


int main() 
{ 
	/* the size (in bytes) of shared memory object */
	unsigned long long int SIZE = 1llu*8*1<<30; 
	char* data = malloc(SIZE);

	/* name of the shared memory object */
	const char* name = "OS"; 

	/* shared memory file descriptor */
	int shm_fd; 

	/* pointer to shared memory object */
	void* ptr; 

	/* open the shared memory object */
	shm_fd = shm_open(name, O_RDONLY, 0777); 

	/* memory map the shared memory object */
	ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0); 

	sem_t* mysem = sem_open("/mysem", O_CREAT, 0777, 1);
	sem_wait(mysem);
	printf("Before copy\n");
	struct timespec start, finish;
	clock_gettime(CLOCK_REALTIME, &start);

	memcpy(data, ptr, SIZE);

	clock_gettime(CLOCK_REALTIME, &finish);
	long seconds = finish.tv_sec - start.tv_sec;
	long ns = finish.tv_nsec - start.tv_nsec;

	if (start.tv_nsec > finish.tv_nsec) { // clock underflow
		--seconds;
		ns += 1000000000;
	}

	printf("nanoseconds consumer: %ld.%ld\n",seconds, ns);

	printf("End copy\n");
	/* remove the shared memory object */
	shm_unlink(name); 
	sem_unlink("/mysem");
	return 0; 
} 

