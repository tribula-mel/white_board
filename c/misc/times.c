#include <stdio.h>
#include <time.h>

int main(void) {
	struct timespec t;

	t.tv_sec = t.tv_nsec = 0;
	clock_gettime(CLOCK_REALTIME, &t);
	printf("REALTIME sec(%ld) nano(%ld)\n", t.tv_sec, t.tv_nsec);

	t.tv_sec = t.tv_nsec = 0;
	clock_gettime(CLOCK_MONOTONIC, &t);
	printf("MONOTONIC sec(%ld) nano(%ld)\n", t.tv_sec, t.tv_nsec);

	t.tv_sec = t.tv_nsec = 0;
	clock_gettime(CLOCK_MONOTONIC_RAW, &t);
	printf("MONOTONIC_RAW sec(%ld) nano(%ld)\n", t.tv_sec, t.tv_nsec);

	return 0;
}
