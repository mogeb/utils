#include <stdio.h>
#include <unistd.h>
#include <time.h>

inline volatile long long rdtsc()
{
	long lo, hi;
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
	return (long long)hi << 32 | lo;
}

int main()
{
	printf("tsc = %lld\n", rdtsc());

	return 0;
}
