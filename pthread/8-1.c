#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>

pthread_t ntid;
void printtids(const char* s)
{
    pid_t pid;
    pthread_t tid;
    pid = getpid();
    tid = pthread_self();
    printf("%s pid = %u  tid = %u\n",s,(unsigned int)pid,(unsigned int)tid);
}

void* thread_fun(void* arg)
{
    printtids(arg);
    return NULL;
}

int main(void)
{
    int err;
    err = pthread_create(&ntid,NULL,thread_fun,"这是新线程：");
    if (err!=0)
    {
        fprintf(stderr,"创建线程失败：%s\n",strerror(err));
        exit(1);
    }
    printtids("我是父进程：");
    sleep(2);
    return 0;
}