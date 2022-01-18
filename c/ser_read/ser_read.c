#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <termios.h>

#define START_SEQ "!mELe!"

static void help(void)
{
	printf ("Transfers a file over the serial line. This could be used\n");
	printf ("for testing purposes. The systems should be connected\n");
	printf ("over the serial interface. One node should run this code\n");
	printf ("as sender (default), the other as receiver (-r option).\n");
	printf ("The sender opens the file (-f option) and starts sending\n");
	printf ("it, while receiver saves the data localy (-f option).\n");
	printf ("Sender needs to be started first.\n\n");
	printf ("Options:\n");
	printf ("   -r (run in receiver mode)\n");
	printf ("   -d <interface> (serial interface to use)\n");
	printf ("   -f <file> (the file to save/load data to/from)\n");
	printf ("   -h (this help)\n");
}

static void receiver(int si, int fi)
{
	uint8_t buffer[100];
	ssize_t tsize = 0;
	ssize_t csize = 0;
	ssize_t size = 0;

	printf ("receiver: about to send the start sequence\n");

	/* send the start sequence */
	if (-1 == write (si, START_SEQ, sizeof (START_SEQ) - 1)) {
		fprintf (stderr, "1 write error(%s)\n", strerror (errno));
		fprintf (stderr, "aborting the operation\n");
		exit (EXIT_FAILURE);
	}

	printf ("receiver: about to read the transfer size\n");

	/* read the size of the transfer */
	size = read (si, &tsize, sizeof (ssize_t));
	if ((size == -1) || (size != sizeof (ssize_t))) {
		fprintf (stderr, "1 read error(%s)\n", strerror (errno));
		fprintf (stderr, "aborting the operation\n");
		exit (EXIT_FAILURE);
	}
#if 0
	tsize = (buffer[7] << 56) | (buffer[6] << 48) |
		(buffer[5] << 40) | (buffer[4] << 32) |
		(buffer[3] << 24) | (buffer[2] << 16) |
		(buffer[1] << 8)  | (buffer[0] << 0);
#endif
	printf("receiver: the size(%ld, %lx)\n", tsize, tsize);

	/* read the data and write to a file */
	while (csize != tsize) {
		size = read (si, &buffer, sizeof (buffer));
		if (-1 == size) {
			fprintf (stderr, "2 write error(%s)\n", strerror (errno));
			fprintf (stderr, "aborting the operation\n");
			exit (EXIT_FAILURE);
		}
		csize += size;
		if ( -1 == write (fi, &buffer, size)) {
			fprintf (stderr, "3 write error(%s)\n", strerror (errno));
			fprintf (stderr, "aborting the operation\n");
			exit (EXIT_FAILURE);
		}
	}

}

static void sender(int si, int fio)
{
	uint8_t buffer[100];
	ssize_t tsize = 0; /* total size */
	ssize_t csize = 0; /* current size */
	ssize_t size = sizeof (buffer);

	printf ("sender: waiting for the start sequence\n");

	/* wait for the start sequence */
	while (true) {
		if (-1 == read (si, &buffer, sizeof (START_SEQ) - 1)) {
			fprintf (stderr, "2 read error(%s)\n", strerror (errno));
			fprintf (stderr, "aborting the operation\n");
			exit (EXIT_FAILURE);
		}
		if (0 == strncmp (START_SEQ, (char *) &buffer, sizeof (START_SEQ) - 1)) {
			break;
		}
	}
	printf ("sender: received the start sequence\n");

	/* send the overall size of the transfer */
	tsize = (ssize_t) lseek (fio, 0, SEEK_END);
	if (tsize == -1) {
		fprintf (stderr, "lseek error(%s)\n", strerror (errno));
		fprintf (stderr, "aborting the operation\n");
		exit (EXIT_FAILURE);
	}
	printf ("sender: the size(%ld)\n", tsize);
	size = write (si, &tsize, sizeof (ssize_t));
	if ((size == -1) || (size != sizeof ( ssize_t))) {
		fprintf (stderr, "5 write error(%s)\n", strerror (errno));
		fprintf (stderr, "aborting the operation\n");
		exit (EXIT_FAILURE);
	}
	lseek (fio, 0, SEEK_SET);
	csize = tsize;

	while (csize != 0) {
		if (size > csize)
			size = csize;

		if (-1 == read (fio, &buffer, size)) {
			fprintf (stderr, "3 read error(%s)\n", strerror (errno));
			fprintf (stderr, "aborting the operation\n");
			exit (EXIT_FAILURE);
		}
		if (-1 == write (si, &buffer, size)) {
			fprintf (stderr, "4 write error(%s)\n", strerror (errno));
			fprintf (stderr, "aborting the operation\n");
			exit (EXIT_FAILURE);
		}
		csize -= size;
	}
	printf ("sender: sent total size(%ld)\n", tsize);
}

int main (int argc, char *argv[])
{
	int seri = -1;
	int file_io = -1;
	int opt = -1;
	bool recv = false;

	while ((opt = getopt (argc, argv, "hrd:f:")) != -1) {
		switch (opt) {
		case 'h':
			help ();
			exit (EXIT_SUCCESS);
		case 'r':
			recv = true;;
			break;
		case 'd':
			seri = open (optarg, O_RDWR);
			if (seri == -1) {
				fprintf (stderr, "1 open error(%s)\n", strerror (errno));
				exit (EXIT_FAILURE);
			}
			break;
		case 'f':
			file_io = open (optarg, O_RDWR);
			if (file_io == -1) {
				fprintf (stderr, "2 open error(%s)\n", strerror (errno));
				exit (EXIT_FAILURE);
			}

			break;
		default: /* '?' */
			fprintf (stderr, "Usage: %s [-h] [-r] [-d <serial>] [-f <file>]\n", argv[0]);
			help ();
			exit (EXIT_FAILURE);
		}
	}

	if ((seri == -1) || (file_io == -1)) {
		fprintf (stderr, "Usage: %s [-h] [-r] [-d <serial>] [-f <file>]\n", argv[0]);
		help ();
		exit (EXIT_FAILURE);
	}

	tcflush(seri, TCIOFLUSH);
	sleep(2);

	if (recv) {
		printf ("starting in the receiver mode\n");
		receiver (seri, file_io);
	}
	else {
		printf ("starting in the sender mode\n");
		sender (seri, file_io);
	}

	close (seri);
	close (file_io);

	exit(EXIT_SUCCESS);
}
