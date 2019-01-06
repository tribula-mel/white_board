#include <stdio.h>
#include <math.h>

int main(void) {
        double latitude  = 45.316553f;

	float latSeconds = fabs(latitude) * 3600;
	int latDegrees = latSeconds / 3600;
	latSeconds = abs(fmod(latSeconds, 3600));
	float latMinutes = (float) latSeconds / 60;

	printf("latiude(%2.6f) degrees(%d) minutes(%2.6f) seconds(%2.6f)\n",
		latitude, latDegrees, latMinutes, latSeconds);
	return 0;
}
