#include <stdatomic.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>

#define semaphore_iteration	50000000
#define atomic_iteration	150000000
sem_t mutex; // declaration of mutex
int volatile atomicv;
struct timespec start, finish;

void atomic_bench() {
	while(__sync_add_and_fetch(&atomicv, 1) < atomic_iteration);
	return;
}

void semaphor_bench(int i) {
	while(i++ < semaphore_iteration){
		sem_wait(&mutex); // ~Dijkstras p()
		//CS
		sem_post(&mutex); // ~Dijkstras v()
	}
	return;
}

void print_result(struct timespec start, struct timespec finish) {
	long seconds = finish.tv_sec - start.tv_sec;
	long ns = finish.tv_nsec - start.tv_nsec;
	if (start.tv_nsec > finish.tv_nsec) { // clock underflow
		--seconds;
		ns += 1000000000;
	}
	printf("seconds without ns: %ld\n", seconds);
	printf("nanoseconds: %ld\n", ns);
	printf("total seconds: %e\n", (double)seconds + (double)ns/(double)1000000000);

	return;
}

int main () {
	printf("--------------- Semaphore Test Starting -------------------\n");
	sem_init(&mutex, 0, 1); // initialize mutex
	int i = 0;

	clock_gettime(CLOCK_REALTIME, &start);	
	semaphor_bench(i);
	clock_gettime(CLOCK_REALTIME, &finish);
	print_result(start, finish);

	printf("--------------- Atomic Test Starting -------------------\n");
	atomic_store(&atomicv, 0);	
	clock_gettime(CLOCK_REALTIME, &start);	
	atomic_bench(&atomicv);
	clock_gettime(CLOCK_REALTIME, &finish);
	print_result(start, finish);

	return 0;
}
