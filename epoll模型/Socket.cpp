#include "Socket.h"

int createSocket()
{
    int listenfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);
    if (listenfd < 0)
    {
        perror("socket failed");
        exit(-1);
    }
    return listenfd;
}

Socket::Socket(int fd) : fd_(fd)
{
}
Socket::~Socket()
{
    ::close(fd_);
}
// 返回fd_成员
int Socket::fd() const
{
    return fd_;
}

// 设置SO_REUSEADDR选项
void Socket::setreuseaddr(bool on)
{
    int optval = on ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &optval, static_cast<socklen_t>(sizeof(optval)));
}
// 设置SO_REUSEPORT选项
void Socket::setreuseport(bool on)
{
    int optval = on ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &optval, static_cast<socklen_t>(sizeof(optval)));
}
// 设置TCP_NODELAY选项
void Socket::settcpnodelay(bool on)
{
    int optval = on ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof(optval)));
}
// 设置SO_KEEPALIVE选项
void Socket::setkeepalive(bool on)
{
    int optval = on ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &optval, static_cast<socklen_t>(sizeof(optval)));
}
// 服务端socket调用
void Socket::bind(const InetAddress &servaddr)
{
    if (::bind(fd_, servaddr.get_addr(), sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        close(fd_);
        exit(-1);
    }
}
// 服务端socket调用
void Socket::listen(int n)
{
    if (::listen(fd_, n) != 0)
    {
        perror("listen failed");
        close(fd_);
        exit(-1);
    }
}
// 服务端socket调用
int Socket::accept(InetAddress &clientaddr)
{
    sockaddr_in peeraddr;
    socklen_t len = sizeof(peeraddr);
    // 优化非阻塞代码 accep4()
    int clientfd = accept4(fd_, (sockaddr *)&peeraddr, &len, SOCK_NONBLOCK);
    clientaddr.setaddr(peeraddr);
    return clientfd;
}