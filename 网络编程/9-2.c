#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define SERVPORT 3333
#define BACKLOG 10
#define MAXDATASIZE 256
#define EXITTOSIG "exit###"
int sockfd;
int oppofd;

struct hostent *host;
struct sockaddr_in serv_addr;
struct sockaddr_in my_addr;
struct sockaddr_in remote_addr;
FILE *fp;
pthread_t ntid;

int sendexit();

void *pthread_recv(void *arg)
{
    int recvbytes;
    char rstr[MAXDATASIZE];
    char timestr[20];
    while (1)
    {
        recvbytes = recv(oppofd, rstr, MAXDATASIZE, 0);
        if (recvbytes == -1)
        {
            perror("recv 失败！\n");
            exit(1);
        }
        else if (strcmp(rstr, EXITTOSIG) == 0)
        {
            printf("聊天结束！\n");
            fclose(fp);
            exit(0);
        }
        rstr[recvbytes] = '\0';
        fprintf(fp, "%s", rstr);
        fprintf(fp, "\n");
        printf("%s\n", rstr);
    }
    pthread_exit((void *)1);
}

int main(int argc, char *argv[])
{
    int sin_size;
    int pth_err;
    int recvbytes;
    char buf[MAXDATASIZE];
    char sstr[MAXDATASIZE];
    char writing[MAXDATASIZE];
    char chatname[17];
    time_t timep;

    if (argc >= 2)
    {
        printf("\nwhat's your name?\n");
        scanf("%s",chatname);
        if ((fp = fopen("./record_cus.txt", "a+")) == NULL)
        {
            perror("creating fp failed!\n");
        }
        exit(1);

        if ((host = gethostbyname(argv[1])) == NULL)
        {
            perror("gethost failed!\n");
            exit(1);
        }
        if ((oppofd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("socket error!\n");
            exit(1);
        }
        __bzero(serv_addr.sin_zero, sizeof(struct sockaddr_in));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(SERVPORT);
        serv_addr.sin_addr = *((struct in_addr *)host->h_addr_list[0]);
        if (connect(oppofd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1)
        {
            perror("connect failed!\n");
            exit(1);
        }

        if ((recvbytes = recv(oppofd, buf, MAXDATASIZE, 0)) == -1)
        {
            perror("received error!\n");
            exit(1);
        }

        buf[recvbytes] = '\0';
        printf("the first received information is: %s\n", buf);
        pth_err = pthread_create(&ntid, NULL, pthread_recv, NULL);

        while (1)
        {
            scanf("%s",writing);
            if (strcmp(writing, "exit") == 0)
            {
                sendexit();
                exit(0);
            }

            strcpy(sstr, chatname);
            strcat(sstr, writing);
            strcat(sstr, "\n");
            strcat(sstr, " at ");
            time(&timep);
            strcat(sstr, asctime(gmtime(&timep)));
            strcat(sstr, "\n");
            fprintf(fp, "%s", sstr);
            fprintf(fp, "\n");
            printf("%s", sstr);
            if (send(oppofd, sstr, strlen(sstr), -1))
            {
                perror("send sstr failed!\n");
                exit(1);
            }
            memset(sstr, 0, MAXDATASIZE);
            memset(writing, 0, MAXDATASIZE);
        }
        close(oppofd);
        fclose(fp);
    }
    else
    {
        if ((fp = fopen("./record_ser.txt", "a+")) == NULL)
        {
            perror("creating fp failed!\n");
            exit(1);
        }
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            perror("socker failed!\n");
            exit(1);
        }
        __bzero(&my_addr, sizeof(struct sockaddr_in));
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(SERVPORT);
        my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
        {
            perror("bind failed!\n");
            exit(1);
        }
        printf("Server's IP :\n");
        system("ifconfig | grep inet");
        printf("\n\n");
        printf("What's your name?\n");
        scanf("%s",chatname);
        if (listen(sockfd, BACKLOG) == -1)
        {
            perror("listen failed!\n");
            exit(1);
        }

        sin_size = sizeof(struct sockaddr_in);
        while (1)
        {
            oppofd = accept(sockfd, (struct sockaddr *)&remote_addr, &sin_size);
            if (oppofd == -1)
            {
                perror("accept failed!\n");
                exit(1);
            }
            else
            {
                break;
            }
        }
        printf("received a connect of %s\n", inet_ntoa(remote_addr.sin_addr));
        if (send(oppofd, "Sucessfully connected!\n", 26, 0) == -1)
        {
            perror("send failed!\n");
            exit(1);
        }
        pth_err = pthread_create(&ntid, NULL, pthread_recv, NULL);
        if (pth_err != 0)
        {
            perror("pthread failed!\n");
            exit(1);
        }
        while (1)
        {
            scanf("%s",writing);
            if (strcmp(writing, "exit") == 0)
            {
                sendexit();
                exit(0);
            }
            strcpy(sstr, chatname);
            strcat(sstr, writing);
            strcat(sstr, "\n");
            strcat(sstr, " at ");
            time(&timep);
            strcat(sstr, asctime(gmtime(&timep)));
            strcat(sstr, "\n");
            fprintf(fp, "%s", sstr);
            fprintf(fp, "\n");
            printf("%s", sstr);
            if (send(oppofd,sstr,strlen(sstr),0)==-1)
            {
                perror("send sstr failed!\n");
                exit(1);
            }
            memset(writing,0,MAXDATASIZE);
            memset(sstr,0,MAXDATASIZE);
            
        }
        close(sockfd);
        close(oppofd);
        fclose(fp);
    }
    return 0;
}

int sendexit()
{
    send(oppofd,EXITTOSIG,strlen(EXITTOSIG),0);
    return 0;
}