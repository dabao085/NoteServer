#ifndef _NOTE_HEADER_H_
#define _NOTE_HEADER_H_

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <time.h>
#include <signal.h>
#include <assert.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include "noteMysql.h"

#define LISTENQ 10
#define MAXN 16384
#define MAXLEN 1024

int Socket(int domain, int type, int protocal)
{
    int sockfd;
    if((sockfd = socket(domain, type, protocal)) == -1) {
        std::cerr << "Socket error!" << std::endl;
        exit(1);
    }
    return 0;
}

int Connect(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen)
{
	if((connect(sockfd, servaddr, addrlen)) == -1) {
		std::cerr << "server is not open or connect:" << std::endl;
		close(sockfd);
        exit(1);
	}
	return 0;
} 

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int retv;
    if((retv = bind(sockfd, addr, addrlen)) == -1) {
        std::cerr << "Bind error!" << std::endl;
        exit(1);
    }
    return 0;
}

int Listen(int sockfd, int backlog)
{
	if((listen(sockfd, backlog)) == -1) {
		std::cerr << "listen:" << std::endl;
        exit(1);
	}
	return 0;
} 

int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen)
{
	int n;
	if((n = accept(sockfd, cliaddr, addrlen)) == -1) {
		std::cerr << "accept:" << std::endl;
        exit(1);
	}
	return n;
}

ssize_t Write(int fd, const void *buf, size_t count)
{
    ssize_t retv;
    if((retv = write(fd, buf, count)) == -1) {
        std::cerr << "Write error!" << std::endl;
        exit(1);
    }
    return retv;
}

ssize_t Read(int fd, void *buf, size_t count)
{
    ssize_t retv;
    if((retv == read(fd, buf, count)) == -1) {
        std::cerr << "Read error!" << std::endl;
        exit(1);
    }
    return retv;
}
#endif