#include <stdio.h>
#include "buffer.h"

/* the buffer */

int init_buffer()
{
    buf.head=0;
    buf.tail=0;
    buf.inbuf=0;
    sem_init(&buf.empty,0,0);
    sem_init(&buf.full,0,BUFFER_SIZE);
    pthread_mutex_init(&buf.mutex,NULL);
    return 0;
}
int insert_item(buffer_item item,int id)
{
    printf("Producer %04d is waiting.\n",id);
    sem_wait(&buf.full);
    printf("Producer %04d has been signaled.\n",id);
    pthread_mutex_lock(&buf.mutex);
    buf.buffer[buf.head] = item;
    printf("Producer %04d produced %d\n",id,item);
    buf.head+=1;
    buf.inbuf+=1;
    printf("%d items in buffer.\n",buf.inbuf);
    if(buf.head==BUFFER_SIZE)
        buf.head = 0;
    pthread_mutex_unlock(&buf.mutex);
    sem_post(&buf.empty);
    return 0;
    /* insert item into buffer
     * return 0 is successful */
}

int remove_item(buffer_item *item,int id)
{
    printf("Consumer %04d is waiting.\n",id);
    sem_wait(&buf.empty);
    printf("Consumer %04d has been signaled.\n",id);
    pthread_mutex_lock(&buf.mutex);
    *item = buf.buffer[buf.tail];
    printf("Consumer %04d consumed %d\n",id,buf.buffer[buf.tail]);
    buf.tail+=1;
    buf.inbuf-=1;
    printf("%d items in buffer.\n",buf.inbuf);
    if(buf.tail==BUFFER_SIZE)
        buf.tail = 0;
    pthread_mutex_unlock(&buf.mutex);
    sem_post(&buf.full);
    return 0;
    /* remove an object from buffer
     * placing it in item
     * return 0 if successful */
}
