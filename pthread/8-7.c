#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct node
{
    int n_number;
    struct node* n_next;
}*head = NULL;
//释放节点内存
static void cleanup_handle(void* arg)
{
    printf("clean up handler of second thread.\n");
    free(arg);
    (void)pthread_mutex_unlock(&mtx);
}
static void* thread_func(void* arg)
{
    struct node *p = NULL;
    pthread_cleanup_push(cleanup_handle,p);
    pthread_mutex_lock(&mtx);
    while (1)
    {
        while (head == NULL)
        {
            pthread_cond_wait(&cond,&mtx);
        }
        p = head;
        head = head->n_next;
        printf("Got %d from front of queue\n",p->n_number);
        free(p);
    }
    pthread_mutex_unlock(&mtx);
    pthread_cleanup_pop(0);
    return 0;
}

int main(void)
{
    int i;
    pthread_t tid;
    struct node *p;
    pthread_create(&tid,NULL,thread_func,NULL);
    for(i = 0;i<10;i++)
    {
        p = (struct node*) malloc(sizeof(struct node));
        p->n_number = i;
        pthread_mutex_lock(&mtx);
        p->n_next = head;
        head = p;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mtx);
        sleep(1);
    }
    printf("thread1 wannaend the cancel thread2.\n");
    pthread_cancel(tid);
    pthread_join(tid,NULL);
    printf("Alldone---exiting \n");
    return 0;
}