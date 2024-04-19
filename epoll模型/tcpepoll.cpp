#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <netinet/tcp.h>
#include <iostream>
#include <fcntl.h>
#include "InetAddress.h"
#include "Socket.h"
#include "Epoll.h"
using namespace std;

// void setnonblocking(int fd)
// {
//     fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
// }

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        cout << "usage: ./tcpepoll 192.168.200.130 port" << endl;
        return -1;
    }
    Socket sockfd(createSocket());
    // 创建非阻塞socket
    // int listensock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP);

    cout << "listensock = " << sockfd.fd() << endl;
    // 设置listensock的属性
    // int opt = 1;
    // setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &opt, static_cast<socklen_t>(sizeof(opt)));
    // setsockopt(listensock, SOL_SOCKET, TCP_NODELAY, &opt, static_cast<socklen_t>(sizeof(opt)));
    // setsockopt(listensock, SOL_SOCKET, SO_REUSEPORT, &opt, static_cast<socklen_t>(sizeof(opt)));
    // setsockopt(listensock, SOL_SOCKET, SO_KEEPALIVE, &opt, static_cast<socklen_t>(sizeof(opt)));
    sockfd.setreuseaddr(true);
    sockfd.setreuseport(true);
    sockfd.setkeepalive(true);
    sockfd.settcpnodelay(true);
    // setnonblocking(listensock);

    InetAddress servaddr(argv[1], atoi(argv[2]));

    // sockaddr_in servaddr;
    // memset(&servaddr, 0, sizeof(servaddr));
    // servaddr.sin_family = AF_INET;
    // servaddr.sin_port = htons(atoi(argv[1]));
    // servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind(listensock,(sockaddr*)&servaddr,sizeof(servaddr));

    // bind(listensock, servaddr.get_addr(), sizeof(servaddr));
    // listen(listensock, 128);

    sockfd.bind(servaddr);
    sockfd.listen();

    // 创建epoll句柄
    Epoll ep;
    // int epollfd = epoll_create(1);

    // 为服务端准备listenfd读事件
    ep.addfd(sockfd.fd(), EPOLLIN); // epoll监视fd的读事件，采用水平触发
    std::vector<epoll_event> evs;

    // epoll_event ev;
    // ev.data.fd = sockfd.fd();
    // ev.events = EPOLLIN;

    // 把需要监视的listenfd加入到epollfd中
    //epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd.fd(), &ev);

    // 存放epoll_wait()返回事件的数组
    //epoll_event evs[10];

    while (true)
    {
        // 等待监视的fd有事件发生
        // int infds = epoll_wait(epollfd, evs, 10, -1);
        // if (infds < 0)
        // {
        //     perror("epoll failed");
        //     break;
        // }
        // 超时
        // if (infds == 0)
        // {
        //     cout << "epoll timeout";
        //     break;
        // }

        evs = ep.loop();
        //for (int i = 0; i < infds; i++)
        for(auto &ev:evs)
        {
            if (ev.events & EPOLLRDHUP) // 对方已经关闭
            {
                close(ev.data.fd);
            }
            else if (ev.events & (EPOLLIN | EPOLLPRI)) // EPOLLIN 普通数据     EPOLLPRI 带外数据
            {
                // listenfd有事件发生，表示有新客户端连接上来
                if (ev.data.fd == sockfd.fd())
                {
                    // sockaddr_in peeraddr;
                    // socklen_t len = sizeof(peeraddr);
                    InetAddress clientaddr;
                    Socket *clientsock = new Socket( sockfd.accept(clientaddr));
                    //int clientsock = sockfd.accept(clientaddr);
                    // 优化非阻塞代码 accep4()
                    // int clientsock = accept4(listensock, (sockaddr *)&peeraddr, &len, SOCK_NONBLOCK);
                    // int clientsock = accept(listensock, (sockaddr *)&peeraddr, &len);
                    //  设置为非阻塞
                    // setnonblocking(clientsock);

                    // InetAddress client(peeraddr);
                    cout << "accpet peeraddr(socket=" << clientsock->fd() << "),ip=" << clientaddr.get_ip() << "port = " << clientaddr.get_port() << endl;

                    // 为新客户断连接准备读事件，并添加到epoll中。
                    ep.addfd(clientsock->fd(),EPOLLIN|EPOLLET);
                    // ev.data.fd = clientsock->fd();
                    // ev.events = EPOLLIN | EPOLLET; // 边缘触发
                    // epoll_ctl(epollfd, EPOLL_CTL_ADD, clientsock->fd(), &ev);
                }
                else // 客户端连接的fd有事件发生
                {
                    char buf[1024];
                    while (true)
                    {
                        bzero(&buf, sizeof(buf));
                        ssize_t nread = read(ev.data.fd, buf, sizeof(buf));
                        if (nread > 0)
                        {
                            cout << "recv(eventfd=" << ev.data.fd << "):" << buf << endl;
                            send(ev.data.fd, buf, sizeof(buf), 0);
                        }
                        else if (nread == -1 && errno == EINTR)
                        {
                            continue;
                        }
                        else if (nread == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
                        {
                            break;
                        }
                        else if (nread == 0)
                        {
                            cout << "client(eventfd = " << ev.data.fd << ")diconnect" << endl;
                            close(ev.data.fd);
                            break;
                        }
                    }
                }
            }
            else if (ev.events & EPOLLOUT) // 有数据需要写
            {
            }
            else // 其他事件都视为错误
            {
                cout << "peeraddr eventfd = " << ev.data.fd << " error" << endl;
                close(ev.data.fd);
            }

            // if (recv(evs .data.fd,buf,1024,0)<=0)
            // {
            //     cout<<"peeraddr(eventfd = "<<evs .data.fd<<")diconnect"<<endl;
            //     close(evs .data.fd);
            // }
            // else
            // {
            //     cout<<"收到："<<buf<<endl;
            //     send(evs .data.fd,buf,sizeof(buf),0);
            // }
        }
    }
    return 0;
}