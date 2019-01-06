#include <stdio.h>
#include <stdlib.h>
#include <endian.h>

#include <arpa/inet.h>


int main(void)
{
	unsigned int big = 167772;
	unsigned int little = 0;

	little = ntohl(big);
	printf("ntohl: big[%08x] little[%08x]\n", big, little);

	little = htonl(big);
	printf("htonl: big[%08x] little[%08x]\n", big, little);

	little = htobe32(big);
	printf("htobe32: big[%08x] little[%08x]\n", big, little);

	little = htole32(big);
	printf("htole32: big[%08x] little[%08x]\n", big, little);

#if 0
 uint32_t htobe32(uint32_t host_32bits);
       uint32_t htole32(uint32_t host_32bits);
#endif

	exit(EXIT_SUCCESS);;
}
