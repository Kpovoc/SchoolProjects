CC=gcc
CFLAGS=-g -Wall
file=test.txt
all: threadSort
test: threadSort
	./threadSort $(file)
	$(RM) threadSort
threadSort:
	$(CC) $(CFLAGS) threadSort.c -o threadSort -lpthread
clean:
	$(RM) threadSort
