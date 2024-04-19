#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
class InetAddress
{
private:
    sockaddr_in addr_;

public:
    InetAddress();
    // 如果是监听的fd，用这个构造函数
    InetAddress(const std::string &ip, uint16_t port);

    // 如果是客户端连接上来的fd，用这个构造函数
    InetAddress(const sockaddr_in addr);

    ~InetAddress();

    // 获取ip
    const char *get_ip() const;

    // 获取端口
    uint16_t get_port() const;

    // 获取sockaddr 类型成员 addr_的地址
    const sockaddr *get_addr() const;

    void setaddr(sockaddr_in clientaddr);
};