#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#define MAXSTACK 100
int stack[MAXSTACK][2];
int size = 0;
sem_t sem;


//信号量的使用


void* RDatal(void)
{
    FILE* fp;
    fp = fopen("file1.dat","r");
    while (!feof(fp))
    {
        fscanf(fp,"%d %d",&stack[size][0],&stack[size][1]);
        sem_post(&sem);
        size++;
    }
    fclose(fp);
}

void* RData2(void)
{
    FILE* fp;
    fp = fopen("file2.dat","r");
    while (!feof(fp))
    {
        fscanf(fp,"%d %d",&stack[size][0],&stack[size][1]);
        sem_post(&sem);
        size++;
    }
    fclose(fp);

}

void* CData1(void )
{
    while (1)
    {
        sem_wait(&sem);
        printf("Plus:%d+%d = %d\n",stack[size][0],stack[size][1],stack[size][0]+stack[size][1]);
        --size;
        sleep(1);
    }
    
}
void* CData2(void )
{
    while (1)
    {
        sem_wait(&sem);
        printf("Multiply:%d*%d = %d\n",stack[size][0],stack[size][1],stack[size][0]*stack[size][1]);
        --size;
        sleep(1);
    }
    
}

int main(void)
{

    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;
    sem_init(&sem,0,0);
    pthread_create(&tid1,NULL,(void*)RDatal,NULL);
    pthread_create(&tid2,NULL,(void*)RData2,NULL);
    pthread_create(&tid3,NULL,(void*)CData1,NULL);
    pthread_create(&tid4,NULL,(void*)CData2,NULL);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);
    pthread_join(tid4,NULL);

    sem_destroy(&sem);
    return 0;
}