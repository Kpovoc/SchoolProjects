CC=gcc
CFLAGS=-c -Wall
LFLAGS=-lpthread

all: pac
	$(RM) *.o

test: pac
	./pac 10 1 15

pac: buffer.o pac.o
	$(CC) buffer.o pac.o -o pac $(LFLAGS)
pac.o: pac.c
	$(CC) $(CFLAGS) pac.c
buffer.o: buffer.c
	$(CC) $(CFLAGS) buffer.c

clean:
	$(RM) *.o pac
