/*noteServer V0.3
 *using IO multiplexing, implement a simple server
*/

#include "noteServer.h"
#include "stringUtil.h"
#include "sqlExecuteResult.h"
#include "sqlExecuteResultFactory.h"
#include <sstream>
using namespace std;

int getResultFromMysql(const string &request, string &response);

int main(int argc, char *argv[])
{
	char recvBuf[1024],	sendBuf[1024];
	string response;
    int listenfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr, cliaddr;
	char buff[1024];
	time_t ticks;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1)
	{
		cerr << "socket error!" << endl;
		return -1;
	}

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(6610);

	int epollfd, nfds;
	struct epoll_event ev, events[20];// listen 20 events

	epollfd = epoll_create(1);
	ev.data.fd = listenfd;
	ev.events = EPOLLIN|EPOLLET;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);

	if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        cerr << "bind error" << endl;
        return -1;
    }

	if(listen(listenfd, LISTENQ) < 0)
    {
        cerr << "listen error" << endl;
        return -1;
    }

	cout << "ready for event-loop" << endl;

	for(;;)//event loop
	{
		nfds = epoll_wait(epollfd, events, 20, -1);
		for(int i = 0; i < nfds; ++i)
		{
			if(events[i].data.fd == listenfd)
			{
				len = sizeof(cliaddr);
				connfd = accept(listenfd,(struct sockaddr*)&cliaddr,&len);
			    if(connfd < 0)
			    {
			        cerr << "accept error" << endl;
			        return -1;
			    }
				cout << "connection from " << inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)) << ", port is " << ntohs(cliaddr.sin_port) << endl;
				ev.data.fd = connfd;
				ev.events = EPOLLIN|EPOLLET;
				if(epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) < 0)
				{
					//printf(stderr, "epoll set insertion error: fd = %d\n", connfd);
					cerr << "epoll set insertion error: fd = " << connfd << endl;
					return -1;
				}
			}
			else
			{
				int n;
				int sockfd = events[i].data.fd;
				if(events[i].events & EPOLLIN){		
					n = read(sockfd, recvBuf, 1024);
					if(n < 0)
					{
						cerr << "read error" << endl;
						return -1;
					}

					if(n == 0)
					{
						cout << "disconnect from client" << endl;
						continue;
					}

					recvBuf[n] = '\0';
					cout << "EPOLLIN: recv msg is: " << recvBuf << endl;

					string strRecvBuf(recvBuf);
					if(getResultFromMysql(recvBuf, response) < 0)
					{
						cerr << "getResultFromMysql error!" << endl;
						return -1;		
					}						
					
					ev.data.fd = sockfd;
					ev.events = EPOLLOUT|EPOLLET;
					epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &ev);
				}
				if(events[i].events & EPOLLOUT)
				{					
					n = write(sockfd, response.c_str(), response.length());
					if(n < 0)
					{
						cerr << "write error" << endl;
						return -1;
					}
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

	string result;
	CSqlExecuteResult* pSqlExecute = CSqlExecuteResultFactory::create(vectQueryElement[0]);
	pSqlExecute->assginParameter(vectQueryElement);
	if(pSqlExecute->executeAndGetResult(result) < 0)
	{
		cerr << "executeAndGetResult error" << endl;
		return -1;
	}
	response = result;
	return 0;
}