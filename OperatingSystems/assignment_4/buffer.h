#include <semaphore.h>
#include <pthread.h>

typedef int buffer_item;
#define BUFFER_SIZE 5

struct
{
    int head;
    int tail;
    int inbuf;
    sem_t empty;
    sem_t full;
    pthread_mutex_t mutex;
    buffer_item buffer[BUFFER_SIZE];
} buf;

int init_buffer();
int insert_item(buffer_item,int);
int remove_item(buffer_item*,int);
