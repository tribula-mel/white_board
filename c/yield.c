#include <sched.h>

int main(void)
{
	for(;;) {
		sched_yield();
	}

	return 0;
}
