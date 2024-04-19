#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>


void* thread_fun1(void* arg)
{
    printf("thread 1 returning\n");
    pthread_exit((void*)1);
}
void* thread_fun2(void* arg)
{
    printf("thread 2 returning\n");
    pthread_exit((void*)2);
}

int main(void)
{
    pthread_t tid1,tid2;
    void* tret;
    int err;
    err = pthread_create(&tid1,NULL,thread_fun1,NULL);
    if (err!=0)
    {
        fprintf(stderr,"创建线程1失败：%s\n",strerror(err));
    }
    err = pthread_create(&tid2,NULL,thread_fun2,NULL);
    if (err!=0)
    {
        fprintf(stderr,"创建线程2失败：%s\n",strerror(err));
    }
    err = pthread_join(tid1,&tret);
    if(err!=0)
    {
        fprintf(stderr,"can't join with thread1 :%s\n",strerror(err));
    }
    printf("thread1 exit code %d\n",(int)tret);
    err = pthread_join(tid2,&tret);
    if (err!=0)
    {
        fprintf(stderr,"can't join with thread2 :%s\n",strerror(err));
    }
    printf("thread2 exit code %d\n",(int)tret);
    printf("我是父进程：");
    sleep(2);
    exit(0);
}