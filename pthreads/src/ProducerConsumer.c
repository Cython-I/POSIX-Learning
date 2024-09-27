#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define BUF_SIZE 3 /*size of shared buffer*/
int buffer[BUF_SIZE]; /*shared buffer*/

/*Buffer Pointers - Shared Vars*/
int add = 0; /*place to add next element*/
int rem = 0; /*place to remove next element*/
int num = 0; /*number of elements in buffer*/

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; /*mutex lock for buffer*/
pthread_cond_t c_cons = PTHREAD_COND_INITIALIZER; /*consumer waits on cv*/
pthread_cond_t c_prod = PTHREAD_COND_INITIALIZER; /*producer waits on cv*/

void* producer (void* param)
{
    int i;
    for(i = 1;i <= 20; i++){
        pthread_mutex_lock(&mutex);
        if(num > BUF_SIZE){
            exit(EXIT_FAILURE);/*Overflow*/
        }

        while(num == BUF_SIZE){
            /*Blocks if buffer is full*/
            pthread_cond_wait(&c_prod,&mutex);
        }
        buffer[add] = i; /*Buffer is not full, add element*/
        add = (add+1) % BUF_SIZE;
        num++;
        pthread_mutex_unlock(&mutex);

        pthread_cond_signal(&c_cons);
        printf("producer inserted %d\n", i); fflush (stdout);
    } 
    printf("producer exiting\n"); fflush(stdout);
    return 0;
}
void* consumer (void* param){
    int i;

    while(true)
    {
        pthread_mutex_lock(&mutex);
        if(num < 0){
            /*Underflow*/
            exit(EXIT_FAILURE);
        }
        while(num == 0){
            /*Block if buffer is empty*/
            pthread_cond_wait(&c_cons, &mutex);
        }
        i = buffer[rem];
        rem = (rem+1) % BUF_SIZE;
        num--;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&c_prod);
        printf("Consume value %d\n",i); fflush(stdout);
        
    }
   // printf("consumer exiting\n"); fflush(stdout);
}

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

    pthread_join(tid1,NULL);/*Wait for producer to exit*/
    pthread_join(tid2,NULL);/*Wait for consumer to exit*/

    return 0;
}

