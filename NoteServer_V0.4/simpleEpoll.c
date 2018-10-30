/*
 *使用epoll实现的服务器
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <time.h>
#include <string.h>
#include <poll.h>
#include <sys/epoll.h>
#include <limits.h>
#include <errno.h>
 
#define MAXLINE 1024
#define OPEN_MAX 128
#define INFTIM -1
 
char buf[MAXLINE];
 
int main(int argc, char **argv)
{
	int i, listenfd, connfd, sockfd, epollfd, nfds;
	int nready;
	ssize_t n;
	
	socklen_t clilen;
	struct pollfd client[OPEN_MAX];
	struct sockaddr_in cliaddr, serveraddr;
	struct epoll_event ev, events[20];
 
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&serveraddr,sizeof(serveraddr));
 
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(6610);
 
	epollfd = epoll_create1(0);
	ev.data.fd = listenfd;
	ev.events = EPOLLIN|EPOLLET;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);
	bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	listen(listenfd,100);
 
	printf("ready for event loop\n");
	for(;;)
	{
		nfds = epoll_wait(epollfd, events, 20, -1);
		for(i = 0;i < nfds; ++i)
		{
			if(events[i].data.fd == listenfd)
			{
				clilen = sizeof(cliaddr);
				connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
				if(connfd < 0)
				{
					perror("accept error");
					continue;
				}
 
				printf("connection from %s, port is %d\n", inet_ntop(AF_INET,&cliaddr.sin_addr,buf,sizeof(buf)),ntohs(cliaddr.sin_port));
				ev.events = EPOLLIN|EPOLLET;
				ev.data.fd = connfd;
				if(epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) < 0)
				{
					fprintf(stderr, "epoll set insertion error: fd = %d\n", connfd);
					return -1;
				}
 
			}
			else	//operate socket
			{
				int n;
				int sockfd = events[i].data.fd;
 
				if(events[i].events & EPOLLIN)	//fd is readable
				{	
					bzero(buf, MAXLINE);
					//printf("EPOLLIN readable\n");
					if(sockfd < 0)
						continue;
 
					if((n = read(sockfd, buf, MAXLINE)) < 0)
					{
						close(sockfd);
						sockfd = -1;
						perror("read error");
						return ;
					}
					else if(n == 0)
					{
						printf("terminated from port %d\n", ntohs(cliaddr.sin_port));
						close(sockfd);
						sockfd = -1;
					}
					else
					{
						printf("read %d characters\n", n - 1);
						ev.data.fd = sockfd;
						ev.events = EPOLLOUT|EPOLLET;
 
						epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &ev);
					}
				}
				if(events[i].events & EPOLLOUT)	//fd is writeable
				{
					//printf("EPOLLOUT writeable\n");
					if(sockfd < 0)
						continue;
 
					if((n = write(sockfd, buf, strlen(buf))) < 0)
					{
						perror("write error");
						return ;
					}
					else
					{
						printf("write %d characters\n", (n - 1) > 0?(n-1):0);
						ev.data.fd = sockfd;
						ev.events = EPOLLIN|EPOLLET;
						bzero(buf, n);
						epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &ev);
					}
				}
			}
		}
	}
 
	return 0;
}
