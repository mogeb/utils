#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KB	1024
#define MB	(1024 * KB)
#define GB 	(1024 * MB)

#define BUFF_SIZE (200 * MB)
#define TIMES			1

int main()
{
	int i;
	char *buf;

	for(i = 0; i < TIMES; i++) {
		printf("Allocating and touching %d bytes\n", BUFF_SIZE);
		buf = (char*) malloc(BUFF_SIZE);
		memset(buf, 0, BUFF_SIZE);
	}

	printf("Enter to continue\n");
	scanf("%s", buf);
	return 0;
}
