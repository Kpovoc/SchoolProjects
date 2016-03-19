CC=gcc
CFLAGS=-g -Wall -lpthread
all: myShell
run: myShell
	./myShell
	$(RM) myShell
myShell:
	$(CC) $(CFLAGS) myShell.c -o myShell
clean:
	$(RM) myShell
