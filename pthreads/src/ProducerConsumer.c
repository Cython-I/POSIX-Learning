#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUF_SIZE 3 /*size of shared buffer*/
int buffer[BUF_SIZE]; /*shared buffer*/

/*Buffer Pointers - Shared Vars*/
int add = 0; /*place to add next element*/
int rem = 0; /*place to remove next element*/
int num = 0; /*number of elements in buffer*/

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; /*mutex lock for buffer*/
pthread_cond_t c_cons = PTHREAD_COND_INITIALIZER; /*consumer waits on cv*/
pthread_cond_t c_prod = PTHREAD_COND_INITIALIZER; /*producer waits on cv*/

void* producer (void* param);
void* consumer (void* param);

int main(int argc, char *argv [])
{
    pthread_t tid1, tid2; /* thread identifiers*/
    int i;

    if(pthread_create(&tid1,NULL, producer,NULL) != 0)
    {
        fprintf(stderr, "Unable to create producer thread");
        exit(EXIT_FAILURE);
    }

    if(pthread_create(&tid2,NULL,consumer,NULL) != 0)
    {
        fprintf(stderr,"Unable to create consumer thread");
        exit(EXIT_FAILURE);
    }

    //pthread_join
    return 0;
}