CC=gcc
CFLAGS=-g -Wall
all: myShell
test: myShell
	./myShell
	$(RM) myShell
myShell:
	$(CC) $(CFLAGS) myShell.c -o myShell -lpthread
clean:
	$(RM) myShell
