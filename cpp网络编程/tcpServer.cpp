#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;

class tcpServer
{
private:
    int m_listenfd;
    int m_clientfd;
    string m_clientip;
    unsigned short m_port;

public:
    tcpServer() : m_clientfd(-1), m_listenfd(-1) {}

    // 初始化服务器
    bool initServer(const unsigned short port)
    {
        if ((m_listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            cout << "socket failed" << endl;
            return false;
        }
        m_port = port;

        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(m_port);
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(m_listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
        {
            cout << "bind failed" << endl;
            close(m_listenfd);
            m_listenfd = -1;
            return false;
        }

        if (listen(m_listenfd, 10) == -1)
        {
            cout << "listen failed" << endl;
            close(m_listenfd);
            return false;
        }
        return true;
    }

    bool my_accept()
    {
        struct sockaddr_in clientaddr;
        socklen_t size = sizeof(clientaddr);
        if ((m_clientfd = accept(m_listenfd, (struct sockaddr *)&clientaddr, &size)) == -1)
        {
            return false;
        }
        m_clientip = inet_ntoa(clientaddr.sin_addr);
        return true;
    }
    const string &get_clientip() const
    {
        return m_clientip;
    }
    // 接收来自客户端的报文
    bool my_recv(string &buf, const size_t maxlen)
    {
        buf.clear();
        buf.resize(maxlen);
        int recvbytes = recv(m_clientfd, &buf[0], buf.size(), 0);
        if (recvbytes <= 0)
        {
            buf.clear();
            return false;
        }
        buf.resize(recvbytes);
        return true;
    }
    // 向客户端发送报文
    bool my_send(const string &buf)
    {
        if (m_clientfd == -1)
        {
            return false;
        }
        if (send(m_clientfd, buf.data(), buf.size(), 0) <= 0)
        {
            return false;
        }
        return true;
    }

    bool closeClient()
    {
        if (m_clientfd == -1)
        {
            return false;
        }
        close(m_clientfd);
        m_clientfd = -1;
        return true;
    }

    bool closeListen()
    {
        if (m_listenfd == -1)
        {
            return false;
        }
        close(m_listenfd);
        m_listenfd = -1;
        return true;
    }
    ~tcpServer()
    {
        closeClient();
        closeListen();
    }
};

int main(int argc, char *argv[])
{
    tcpServer server;
    if (!server.initServer(atoi(argv[1])))
    {
        cout << "init" << endl;
    }
    while(true)
    {
        int pid = fork();
        if(pid>0)
        {
            continue;
        }
        if (pid==-1)
        {
            perror("fork()");
            return -1;
        }
        if (!server.my_accept())
        {
            cout << "accept" << endl;
        }
        string buf;
        while (true)
        {
            if (server.my_recv(buf, 1024) == false)
            {
                perror("recv");
                cout << "recv" << endl;
                break;
            }
            cout << "收到来自" << server.get_clientip() << "的消息:" << buf << endl;
            buf = "ok";
            server.my_send(buf);
            cout << "发送：" << buf << endl;
        }
    }
}
