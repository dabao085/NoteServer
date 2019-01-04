/*
* Copyright (c) 2018, Leonardo Cheng <chengxiang085@gmail.com>.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*
*  1. Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*@file noteServer.cpp
 *@brief using IO multiplexing, implement a simple server
*/

#include "noteServer.h"
#include "stringUtil.h"
#include "sqlExecuteResult.h"
#include "sqlExecuteResultFactory.h"
#include <sstream>
#include <memory>
using namespace std;

int getResultFromMysql(const string &request, string &response);

int main(int argc, char *argv[])
{
	char recvBuf[1024],	sendBuf[1024], buff[1024];
	string response;
    int listenfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr, cliaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;	//IPV4 protocal
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(6610);

	int epollfd, nfds;
	struct epoll_event ev, events[20];// listen 20 events

	epollfd = epoll_create(1);
	ev.data.fd = listenfd;
	ev.events = EPOLLIN|EPOLLET;//focus on "Input"
	epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);

	Bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);

	cout << "ready for event-loop" << endl;

	for(;;)//event loop
	{
		nfds = epoll_wait(epollfd, events, 20, -1);//maxevents: 20
		for(int i = 0; i < nfds; ++i)
		{
			//listenning for new connection
			if(events[i].data.fd == listenfd)
			{
				len = sizeof(cliaddr);
				connfd = Accept(listenfd, (struct sockaddr*)&cliaddr, &len);

				cout << "connection from " << inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)) << ", port is " << ntohs(cliaddr.sin_port) << endl;
				ev.data.fd = connfd;
				ev.events = EPOLLIN|EPOLLET;
				if(epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) < 0)
				{
					cerr << "epoll set insertion error: fd = " << connfd << endl;
					return -1;
				}
			}
			else
			{
				int sockfd = events[i].data.fd;
				//READ operation is ready!
				if(events[i].events & EPOLLIN){		
					int n = read(sockfd, recvBuf, 1024);
					if(n == 0)
					{
						cout << "disconnect from client" << endl;
						epoll_ctl(epollfd, EPOLL_CTL_DEL, sockfd, NULL);	//delete socket which is disconnected.
						continue;
					}

					recvBuf[n] = '\0';
					cout << "EPOLLIN: recv msg is: " << recvBuf << endl;

					//string strRecvBuf(recvBuf);
					if(getResultFromMysql(recvBuf, response) < 0)
					{
						cerr << "getResultFromMysql error!" << endl;
						return -1;		
					}						
					
					if(response.empty()){
						response = "记录为空";
					}
					//sockfd: READ->WRITE into eventset.
					ev.data.fd = sockfd;
					ev.events = EPOLLOUT|EPOLLET;
					epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &ev);
				}
				if(events[i].events & EPOLLOUT)
				{					
					write(sockfd, response.c_str(), response.length());
					response.clear();
					memset(recvBuf, 0, 1024);

					ev.data.fd = sockfd;
					ev.events = EPOLLIN|EPOLLET;
					epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &ev);
				}
			}
		}
	}

	return 0;
}

/*
 *@inputParam1:request is a string which contains command and parameters etc.
 *example:add 2018-07-10_09:00 2018-07-10_10:00 reading
 *@inputParam2:reponse is a string which contains query result from mysql.
 *@return value:0 means success, -1 means error
*/

/*
 * 连接数据库进行操作
 * request为请求
 * response为返回的结果
*/
int getResultFromMysql(const string &request, string &response)
{
    if(request.empty())
    {
        cerr << "request is empty" << endl;
        return -1;
    }

    //split request by blank character
    string cmdElement;
    vector<string> vectQueryElement;    
    istringstream cmdRecord(request);
    while(cmdRecord >> cmdElement)
        vectQueryElement.push_back(cmdElement);

	if(vectQueryElement.empty())
	{
		cerr << "vectQueryElement is empty!" << endl;
		return -1;
	}

	//string result;
	CSqlExecuteResult* pSqlExecute = CSqlExecuteResultFactory::create(vectQueryElement[0]);
	//shared_ptr<CSqlExecuteResult> pSqlExecute(CSqlExecuteResultFactory::create(vectQueryElement[0]));

	pSqlExecute->assginParameter(vectQueryElement);
	if(pSqlExecute->executeAndGetResult(response) < 0)
	{
		cerr << "executeAndGetResult error" << endl;
		return -1;
	}
	//response = result;
	return 0;
}