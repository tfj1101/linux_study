#include "Epoll.h"

Epoll::Epoll()
{
    if ((epollfd_ = epoll_create(1)) == -1)
    {
        printf("epoll_create() failed(%d)", errno);
        exit(-1);
    }
}

Epoll::~Epoll()
{
    close(epollfd_);
}

void Epoll::addfd(int fd, uint32_t op) // 把listenfd和它需要监视的事件添加到红黑树上。
{
    epoll_event ev;                              // 声明事件的数据结构
    ev.data.fd = fd;                             //
    ev.events = op;                              // epoll监视fd的读事件，采用水平触发
    epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev); // 把需要监听的fd和它的事件加入epollfd中。
}
// 运行epoll_wait()，等待事件的发生，已发生的事件用vector容器返回
std::vector<epoll_event> Epoll::loop(int timeout)
{
    std::vector<epoll_event> evs;
    bzero(events_,sizeof(events_));
    int infds = epoll_wait(epollfd_,events_,MaxEvents,timeout);
    if (infds<0)
    {
        perror("epoll_wait failed");
        exit(-1);
    }
    if (infds==0)
    {
        printf("epoll_wait() timeout\n");
        return evs;
    }

    for (int i = 0; i < infds; i++)
    {
        evs.push_back(events_[i]);
    }
    
    
    return evs;
}