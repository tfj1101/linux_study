#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#define nLOOP 10
pthread_t thread[2];
pthread_mutex_t mutex;
int counter = 0;
int i;
void *pthread1()
{
    for (i = 0; i < nLOOP; i++)
    {
        printf("线程1: %d\n", counter);
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }
    pthread_exit(NULL);
}
void *pthread2()
{
    for (i = 0; i < nLOOP; i++)
    {
        printf("线程2: %d\n", counter);
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
        sleep(3);
    }
    pthread_exit(NULL);
}

void thread_create(void)
{
    int temp;
    // 创建线程
    memset(&thread, 0, sizeof(thread));
    if ((temp = pthread_create(&thread[0], NULL, pthread1, NULL)) != 0)
    {
        printf("线程1创建失败！\n");
    }
    else
    {
        printf("线程1创建成功！\n");
    }
    if ((temp = pthread_create(&thread[1], NULL, pthread2, NULL)) != 0)
    {
        printf("线程2创建失败！\n");
    }
    else
    {
        printf("线程2创建成功！\n");
    }
}

void thread_wait()
{
    for (int i = 0; i < 2; i++)
    {
        if (thread[i] != 0)
        {
            pthread_join(thread[i], NULL);
            printf("线程%d已经结束！\n", i + 1);
        }
    }
}
int main(int argc, char *argv)
{
    pthread_mutex_init(&mutex,NULL);
    printf("我是主函数，正在创建线程...\n");
    thread_create();
    printf("我是主函数，正在等待线程完成任务...\n");
    thread_wait();
    pthread_mutex_destroy(&mutex);
    return 0;
}
