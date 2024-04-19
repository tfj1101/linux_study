#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
pthread_cond_t count_nonzero;//条件变量
pthread_mutex_t count_lock;//互斥锁
unsigned count = 0;
//等待线程
void* decrement_count(void* arg)
{
    //等待线程：1使用pthread_cond_wait前要先加锁
    pthread_mutex_lock(&count_lock);
    printf("decrement_count get count_lock\n"); 
    while(count == 0)
    {
        printf("decrement_count count == 0\n");
        printf("decrement_count before cond_wait \n");
        //等待条件变量被其他线程激活
        pthread_cond_wait(&count_nonzero,&count_lock);//阻塞线程
        printf("decrement_count after cond_wait\n");
    }
    printf("tid1:count--\n");
    count = count-1;
    printf("tid1:count = %d\n",count);
    pthread_mutex_unlock(&count_lock);
}
//激活线程
void *increment_count(void* arg)
{
    //激活线程：1加锁（和等待线程用同一个锁）
    pthread_mutex_lock(&count_lock);
    printf("increment_count get count_lock\n");
    if(count == 0)//count == 0，唤醒等待的线程
    {
        printf("increment_count before cond_signal\n");
        pthread_cond_signal(&count_nonzero);//发送信号唤醒线程
        printf("increment_count after cond_signal\n");
    }
    printf("tid2:count++\n");
    count = count+1;
    printf("tid2:count=%d\n",count);
    //3解锁，激活线程的上面三个操作在运行时间上都在等待线程的pthread_cond_wait函数内部
    pthread_mutex_unlock(&count_lock);
}

int main(void)
{
    pthread_t tid1,tid2;
    //初始化互斥锁，初始化条件变量
    pthread_mutex_init(&count_lock,NULL);
    pthread_cond_init(&count_nonzero,NULL);

    pthread_create(&tid1,NULL,decrement_count,NULL);
    printf("tid1: decrement is created,begin sleep 2s\n");
    sleep(2);
    printf("after sleep 2s ,start create tid2 increment\n");
    pthread_create(&tid2,NULL,increment_count,NULL);
    printf("after tid2 increment is created,begin sleep 10s\n");
    sleep(5);
    printf("after sleep 5s ,begin exit!\n");
    pthread_exit(0);
    return 0;
}