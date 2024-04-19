#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

class tcpClient
{
public:
    int client_fd;
    string m_ip;
    unsigned short m_port;

    tcpClient() : client_fd(-1) {}
    ~tcpClient() {close(client_fd);}

    bool my_connect(const string &ip, const unsigned short port)
    {
        if (client_fd != -1)
        {
            return false;
        }
        // 初始化服务器地址
        m_ip = ip;
        m_port = port;
        struct sockaddr_in servaddr;
        memset(&servaddr,0,sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(m_port);

        // 创建socket
        client_fd = socket(AF_INET, SOCK_STREAM, 0);

        // 请求连接connect
        struct hostent *h;
        if ((h = gethostbyname(m_ip.c_str())) == nullptr)
        {
            cout << "get failed!" << endl;
            close(client_fd);
            return false;
        }
        memcpy(&servaddr.sin_addr, h->h_addr, h->h_length);
        if (connect(client_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
        {
            cout<<"connect failed"<<endl;
            close(client_fd);
            return false;
        }
        return true;
   }
    bool my_send(const string &buf)
    {
        if (send(client_fd, buf.data(), buf.size(), 0)==-1)
        {
            perror("send");
            return false;
        }
        return true;
    }

    bool my_recv(string &buffer, const size_t maxlen)
    {
        buffer.clear();
        buffer.resize(maxlen);
        int recvbytes = recv(client_fd,&buffer[0],buffer.size(),0);
        if (recvbytes<=0)
        {
            buffer.clear();
            return false;
        }
        buffer.resize(recvbytes);
        return true;
    }
};

int main(int agrc, char *argv[])
{
    if (agrc != 3)
    {
        cout << "Using :/tcpClient 服务端IP 服务端端口\nExampe:./tcpClient 192.168.200.130 5005\n"
             << endl;
        return -1;
    }

    tcpClient client;
    if (client.my_connect(argv[1], atoi(argv[2]))==false)
    {
        perror("connect()");
        return -1;
    }

    // 与服务端通讯
    string buf;
    for (int i = 0; i < 10; i++)
    {
        buf = "这是第"+to_string(i+1)+"个。";
        if ((client.my_send(buf))==false)
        {
            perror("send");
            break;
        }
        cout << "发送：" << buf << endl;

        if (client.my_recv(buf, sizeof(buf))==false)
        {
            break;
        }
        cout << "接收：" << buf << endl;
        sleep(1);
    }

    return 0;
}
