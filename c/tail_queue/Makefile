all: tail_queue

tail_queue.o: tail_queue.c
	gcc -Wall -g -c tail_queue.c

tail_queue: tail_queue.o
	gcc -Wall -g -o tail_queue tail_queue.o

clean:
	rm -rf tail_queue tail_queue.o
