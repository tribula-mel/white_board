#include <stdio.h>
#include <math.h>

int main(void) {
        double latitude  = 40.302665;
	double decimals = 0.0;

	double latDegrees;
	decimals = modf(fabs(latitude), &latDegrees);
	double latMinutes = decimals * 60;
	//decimals = modf(decimals * 60, &latMinutes);
	//double latSeconds = decimals * 60;

	printf("latiude(%09.6lf) degrees(%09.6lf) minutes(%09.6lf)\n",
		latitude, latDegrees, latMinutes);
	printf("heading (%c)\n", latitude > 0 ? 'N':'S');
	return 0;
}
