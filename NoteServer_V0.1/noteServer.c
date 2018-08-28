/*noteServer V0.1
 *using accept and fork modle, implement a simple server
*/

#include "noteServer.h"

void sig_child(int signo);
void disposeProcess(int fd);
void getResultFromMysql(char *request, char *response);

int main(int argc, char *argv[])
{
    int listenfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr, cliaddr;
	char buff[1024];
	time_t ticks;
	pid_t pid;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1)
	{
		fprintf(stderr, "socket error!");
		exit(EXIT_FAILURE);
	}

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(6610);

	if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        fprintf(stderr, "bind error!");
        exit(EXIT_FAILURE);
    }

	if(listen(listenfd,LISTENQ) < 0)
    {
        fprintf(stderr, "listen error!");
        exit(EXIT_FAILURE);
    }
	signal(SIGCHLD,sig_child);

	for(;;)
	{
		len = sizeof(cliaddr);
		connfd = accept(listenfd,(struct sockaddr*)&cliaddr,&len);
        if(connfd < 0)
        {
            fprintf(stderr, "accept error");
            exit(EXIT_FAILURE);
        }

		printf("connection from %s, port is %d\n", inet_ntop(AF_INET,&cliaddr.sin_addr,buff,sizeof(buff)),ntohs(cliaddr.sin_port));

		pid = fork();
		if(pid < 0)
		{
			fprintf(stderr, "fork error!");
			exit(EXIT_FAILURE);
		}
		if(pid == 0)//child
		{
			close(listenfd);
			disposeProcess(connfd);
			exit(EXIT_SUCCESS);
		}

		close(connfd);
	}
}

/*inputParam1:request is a string which contains command and parameters etc.
 *inputParam2: queryElements are the result parsed by request, split by inputParam3
 *inputParam3: split character
*/
inline parseRequest(char *request, char *queryElements[], char splitChar)
{
	char *pch = NULL;
    int i = 0;
    pch = strtok(request, &splitChar);
    while(pch != NULL)
    {
        queryElements[i++] = pch;
        pch = strtok(NULL, &splitChar);
    }
}

void sig_child(int signo)
{
	pid_t pid;
	int stat;

	while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
	{
		printf("child %d terminated\n\n", pid);	
	}

	return ;
}

void disposeProcess(int fd)
{
    //read and operate msyql.
    char recvBuf[1024];
	char response[1024];
    int n;
    n = read(fd, recvBuf, 1024);
    if(n < 0)
    {
        fprintf(stderr, "read error!");
        exit(EXIT_FAILURE);
    }
    recvBuf[n] = '\0';
    printf("recv msg is %s\n", recvBuf);
	getResultFromMysql(recvBuf,response);
}

/*
 *@inputParam1:request is a string which contains command and parameters etc.
 *example:add 2018-07-10_09:00 2018-07-10_10:00 看书
 *@inputParam2:reponse is a string which contains query result from mysql.
*/
void getResultFromMysql(char *request, char *response)
{
    if(request == NULL || strlen(request) == 0)
    {
        fprintf(stderr, "The request string is empty! please check your input.");
        exit(EXIT_FAILURE);
    }

    MYSQL_RES* res = NULL;
	MYSQL_ROW 		row;
	MYSQL* mysql =  mysql_init(NULL);
	unsigned int timeout = 3000;
	mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);

	if (mysql_real_connect(mysql, DBHOST, DBUSER, DBPASS, DBNAME, DBPORT, DBSOCK, DBPCNT) == NULL)
    {
		printf("connect failed: %s\n", mysql_error(mysql));
		mysql_close(mysql);
		mysql_library_end();
		//return 0;
	}

	//printf("connect success!\n");
    char *queryElements[4] = {NULL};
    parseRequest(request, queryElements, ' ');

	char sql[256], responseMessage[128];

    if(strcmp(queryElements[0],"add") == 0)
	{
    	sprintf(sql, "insert into noteinfo(start_time,end_time,issue_content,complete_flag,serial_num_of_the_day,state) values('%s','%s','%s',0,0,'F0A')",queryElements[1],queryElements[2],queryElements[3]);		
	}
	else if(strcmp(queryElements[0],"delete") == 0)
	{


	}
	else if(strcmp(queryElements[0],"done") == 0)
	{

	}
	else if(strcmp(queryElements[0],"list") == 0)
	{
		char dateStr[128];
		strncpy(dateStr, queryElements[1], 10);
		sprintf(sql, "select info_seq, start_time, end_time, issue_content, complete_flag, serial_num_of_the_day, state from noteinfo where start_time like \'%s%%\'", dateStr);
	}


	int ret = mysql_query(mysql, sql);
	if(strcmp(queryElements[0],"list") == 0)//select operatition needs more work, like return result etc.
	{
		res = mysql_store_result(mysql);
		if(res == NULL)
		{
			fprintf(stderr, "mysql_store_result failed!");
			return ;
		}

		row = mysql_fetch_row(res);
		int fieldcount = mysql_num_fields(res);
		int rows = mysql_num_rows(res);
		char buf[32];
		int i, j;

		for(j = 0; j < rows; ++j)
		{
			for(i = 0; i < fieldcount; ++i)
			{
				memset(buf, 0, sizeof(buf));
				if (row[i] != NULL && (strlen(row[i])>0))
				{
					snprintf(buf, sizeof(buf), "%s", row[i]);
					strcpy(response, buf);
					strcpy(response, " ");
					printf("%s ", buf);
				}
			}
			strcpy(response, "\n");
			printf("\n");
			row = mysql_fetch_row(res);
		}
		printf("%s\n", response);
	}
//	res = mysql_store_result(mysql);
    if(ret = 1)//success
    {
        printf("mysql operation success!");
    }
    else
    {
		printf("mysql operatition failed!");
		return ;
    }
}