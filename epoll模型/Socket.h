#pragma once
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include "InetAddress.h"

int createSocket();
class Socket
{
public:
    Socket(int fd);
    ~Socket();
    int fd() const;                         // 返回fd_成员
    void setreuseaddr(bool on);             // 设置SO_REUSEADDR选项
    void setreuseport(bool on);             // 设置SO_REUSEPORT选项
    void settcpnodelay(bool on);            // 设置TCP_NODELAY选项
    void setkeepalive(bool on);             // 设置SO_KEEPALIVE选项
    void bind(const InetAddress &servaddr); // 服务端socket调用
    void listen(int n = 128);               // 服务端socket调用
    int accept(InetAddress &clientaddr);    // 服务端socket调用

private:
    const int fd_;
};