# Producer and Consumer Problem
This program solves the "Producer and Consumer" problem with the use of pthreads, mutexs, and semaphores. Producer threads generate a resource (in this case an integer randomly picked between 0 and 99),input it into a circular-array and sleep randomly from 0-2 seconds. Consumer threads remove the next resource from the array and sleep randomly from 0-2 seconds.  

The program can be run via CLI:
```bash
# <p> <c> and <r> are all integers representing the number of (p)roducer and (c)onsumer
# threads created, and the (r)untime of the program in seconds.
./pac <p> <c> <r>
```
The program can be built with make:

```bash
# Builds pac and removes .o files
make

# Builds and runs pac with arguments 10 1 15
make test

# Removes all built files
make clean
```
