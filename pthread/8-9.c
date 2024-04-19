#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<string.h>
#include<pthread.h>

sem_t sem;
void* thread_function1(void *p)
{
    printf("thread_function1------------------sem_wait\n");
    sem_wait(&sem);
    printf("sem_wait\n");
    while (1)
    {
        
    }
    
}

void* thread_function2(void *p)
{
    printf("thread_function2-------------------sem_post\n");
    sem_post(&sem);
    printf("sem_post\n");
    while(1)
    {

    }
}

int main()
{
    int res;
    pthread_t a_thread;
    void *thread_result;
    res = sem_init(&sem,0,0);
    if(res!=0)
    {
        perror("Semaphore initializathon failed!\n");
    }
    printf("sem_init\n");
    res = pthread_create(&a_thread,NULL,thread_function1,NULL);
    if (res!=0)
    {
        perror("pthread_create failed!\n");
    }
    printf("thread_function1 !\n");
    
    sleep(4);
    printf("sleep\n");

    res = pthread_create(&a_thread,NULL,thread_function2,NULL);
    if(res!=0)
    {
        perror("pthread_create failed\n");
    }
    while (1)
    {
        
    }
    return 0;
}