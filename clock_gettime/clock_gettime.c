#include <stdio.h>
#include <unistd.h>
#include <time.h>

int c_gettime()
{
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	printf("c_gettime = %ld\n", start.tv_nsec);
	return 0;
}

void print_monotonic()
{
	struct timespec tsp;
	clock_gettime(CLOCK_MONOTONIC, &tsp);
	printf("CLOCK_MONOTONIC : tsp.tv_sec = %d, tsp.tv_nsec = %ld\n", tsp.tv_sec, tsp.tv_nsec);
}

void print_realtime()
{
	struct timespec tsp;
	clock_gettime(CLOCK_REALTIME, &tsp);
	printf("CLOCK_REALTIME : tsp.tv_sec = %d, tsp.tv_nsec = %ld\n", tsp.tv_sec, tsp.tv_nsec);
}

void print_monotonic_raw()
{
	struct timespec tsp;
	clock_gettime(CLOCK_MONOTONIC_RAW, &tsp);
	printf("CLOCK_MONOTONIC_RAW : tsp.tv_sec = %d, tsp.tv_nsec = %ld\n", tsp.tv_sec, tsp.tv_nsec);
}

int main(int argc, char** argv)
{
	if(argc == 1) {
		print_realtime();
		print_monotonic();
		print_monotonic_raw();
	}
	else if(strcmp(argv[1], "--CLOCK_REALTIME") == 0 || strcmp(argv[1], "--realtime") == 0) {
		print_realtime();
	}
	else if(strcmp(argv[1], "--CLOCK_MONOTONIC") == 0 || strcmp(argv[1], "--monotonic") == 0) {
		print_monotonic();
	}
	else if(strcmp(argv[1], "--CLOCK_MONOTONIC_RAW") == 0 || strcmp(argv[1], "--monotonic") == 0) {
		print_monotonic_raw();
	}


	return 0;
}
