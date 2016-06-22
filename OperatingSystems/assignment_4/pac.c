#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "buffer.h"

#define RANDOM_MAX 100

void* producer(void* param)
{
    int id = *((int *) param) + 1;
    free(param);
    buffer_item item;
    while(1)
    {
        /* Sleep for a random period of time. */
        sleep(rand()%3);
        item = rand()%RANDOM_MAX;
        insert_item(item,id);
    }

}

void* consumer(void* param)
{
    int id = *((int *) param) + 1;
    free(param);
    buffer_item item;
    while(1)
    {
        sleep(rand()%3);
        remove_item(&item,id);
    }
}


int main(int argc, char const *argv[])
{
    /* 1. Get command line arguments argv 1 2 and 3
     * * 1) How long to sleep before terminating
     * * 2) The number of producer threads
     * * 3) The number of consumer threads*/
    char *end;
    char selection;
    if(argc!=4)
    {
        fprintf(stderr,"Invalid number of arguments.\n");
        fprintf(stderr, "Please restart the program with the appropriate values.\n");
        fprintf(stderr, "Integer arguements for: producer consumer runtime.\n");
        exit(EXIT_FAILURE);
    }


    long num_of_pro = strtol(argv[1],&end,10);
    long num_of_con = strtol(argv[2],&end,10);
    long sleep_time = strtol(argv[3],&end,10);

    printf("You've selected %ld Producers, %ld Consumers, ",num_of_pro,num_of_con);
    printf("and the program will run for %ld seconds.\n",sleep_time);
    printf("Are these values correct? (y/n)");
    scanf("%c",&selection);
    if(selection!='y')
    {
        fprintf(stderr,"Please restart the program with the appropriate values.\n");
        fprintf(stderr,"Integer arguements for: producer consumer runtime.\n");
        exit(EXIT_FAILURE);
    }

    /* 2. Initialize buffer*/
    init_buffer();

    /* 3. Create producer thread(s)*/
    pthread_t pro;
    for(int i=0;i<num_of_pro;i++)
    {
        int *id = malloc(sizeof(*id));
        if(id==NULL)
        {
            fprintf(stderr,"Failed to allocate memory for thread id.\n");
            exit(EXIT_FAILURE);
        }
        *id = i;
        pthread_create(&pro,NULL,producer,id);
    }

    /* 4. Create consumer thread(s)*/
    pthread_t con;
    for(int i=0;i<num_of_con;i++)
    {
        int *id = malloc(sizeof(*id));
        if(id==NULL)
        {
            fprintf(stderr,"Failed to allocate memory for thread id.\n");
            exit(EXIT_FAILURE);
        }
        *id = i;
        pthread_create(&con,NULL,consumer,id);
    }

    /* 5. Sleep */
    sleep(sleep_time);
    /* 6. Exit */
    return 0;
}
