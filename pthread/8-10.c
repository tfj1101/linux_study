#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include<unistd.h>
#include <stdlib.h>

sem_t full, empty;
pthread_mutex_t mutex;
#define BUFFERSIZE 5
struct msgbuf
{
    pid_t id;
    time_t mytime;
};

struct msgbuf msg[BUFFERSIZE];
int in = 0;
int out = 0;
void *producer(void *arg)
{
    time_t rt;
    for (int i = 1; i <= 10; i++)
    {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        msg[in].id = pthread_self();//获取线程id，写入缓冲区
        time(&msg[in].mytime);
        printf("生产者%d第%d次写消息--,id = %u,time is :%s\n",arg,i,(unsigned)(msg[in].id),ctime(&(msg[in].mytime)));
        in = (++in)%5;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        srand((unsigned)time(&rt));
        sleep(rand()%5);
    }
}
void* consumer(void* arg)
{
    time_t rt;
    for (int i = 1; i <= 10; i++)
    {
            sem_wait(&full);
            pthread_mutex_lock(&mutex);
            printf("消费者%d 第 %d次读取消息--,id = %u,time is: %s\n",arg,i,(unsigned)(msg[out].id),ctime(&(msg[out].mytime)));
            out = (++out)%5;
            pthread_mutex_unlock(&mutex);
            sem_post(&empty);
            srand((unsigned)time(&rt));
            sleep(3+rand()%5);
    }
    
}

int main(int argc,char* argv[])
{
    pthread_t pid1,pid2;
    pthread_t cid1,cid2;
    sem_init(&full,0,0);
    sem_init(&empty,0,5);
    pthread_mutex_init(&mutex,NULL);

    pthread_create(&pid1,NULL,producer,1);
    pthread_create(&pid2,NULL,producer,2);
    pthread_create(&cid1,NULL,consumer,1);
    pthread_create(&cid2,NULL,consumer,2);

    pthread_join(pid1,NULL);
    pthread_join(pid2,NULL);
    pthread_join(cid1,NULL);
    pthread_join(cid2,NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);

    return 0;
}