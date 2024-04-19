#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/epoll.h>
#include <vector>
#include <unistd.h>

// Epoll类
class Epoll
{
private:
    static const int MaxEvents = 100;            // epoll_wait()返回事件数组的大小
    int epollfd_ = -1;              // epoll句柄，在构造函数中创建
    epoll_event events_[MaxEvents]; // 存放epoll_wait返回事件数组，在构造函数中分配内存

public:
    Epoll();
    ~Epoll();

    void addfd(int fd,uint32_t op);//把fd和它需要监视的事件添加到红黑树上。
    std::vector<epoll_event> loop(int timeout=-1);
};