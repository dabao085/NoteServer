/*noteServer V0.2
 *using accept and fork modle, implement a simple server
*/

#include "noteServer.h"
#include "stringUtil.h"
#include "sqlExecuteResult.h"
#include "sqlExecuteResultFactory.h"
#include <sstream>
using namespace std;

void sig_child(int signo);
int disposeProcess(int fd);
int getResultFromMysql(const string &request, string &response);

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
		cerr << "socket error!" << endl;
		return -1;
	}

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(6610);

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

	signal(SIGCHLD,sig_child);

	for(;;)
	{
		len = sizeof(cliaddr);
		connfd = accept(listenfd,(struct sockaddr*)&cliaddr,&len);
        if(connfd < 0)
        {
            cerr << "accept error" << endl;
            return -1;
        }

        cout << "connection from " << inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)) << ", port is " << ntohs(cliaddr.sin_port) << endl;

		pid = fork();
		if(pid < 0)
		{
            cerr << "fork error" << endl;
			return -1;
		}
		if(pid == 0)//child
		{
			close(listenfd);
			disposeProcess(connfd);
			return 0;
		}

		close(connfd);
	}
}

void sig_child(int signo)
{
	pid_t pid;
	int stat;

	while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
	{
        cout << "child " << pid << " terminated" << endl;
	}

	return ;
}

int disposeProcess(int fd)
{
    char recvBuf[1024];
    int n;
    n = read(fd, recvBuf, 1024);
    if(n < 0)
    {
        cerr << "read error" << endl;
        return -1;
    }
    recvBuf[n] = '\0';
    cout << "recv msg is: " << recvBuf << endl;

    string strRecvBuf(recvBuf);
    string response;
	if(getResultFromMysql(recvBuf,response) < 0)
        return -1;

    n = write(fd, response.c_str(), response.length());
    if(n < 0)
    {
        cerr << "write error" << endl;
        return -1;
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

// int getResultFromMysql(const string &request, string &response)
// {
//     if(request.empty())
//     {
//         cerr << "request is empty" << endl;
//         return -1;
//     }

//     MYSQL_RES* res = NULL;
// 	MYSQL_ROW 		row;
// 	MYSQL* mysql =  mysql_init(NULL);
// 	unsigned int timeout = 3000;
// 	mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);

// 	if (mysql_real_connect(mysql, DBHOST, DBUSER, DBPASS, DBNAME, DBPORT, DBSOCK, DBPCNT) == NULL)
//     {
//         cerr << "connect failed: " << mysql_error(mysql) << endl;
// 		mysql_close(mysql);
// 		mysql_library_end();
// 		return -1;
// 	}

//     //split request by blank character
//     string cmdElement;
//     vector<string> vectQueryElement;    
//     istringstream cmdRecord(request);
//     while(cmdRecord >> cmdElement)
//         vectQueryElement.push_back(cmdElement);

//     string sql;
// 	string dateStr(vectQueryElement[1].substr(0,10));

//     if(strcmp(vectQueryElement[0].c_str(), "add") == 0)
// 	{
//         char sqlChar[256];
//         sprintf(sqlChar, "insert into noteinfo(start_time,end_time,issue_content,complete_flag,serial_num_of_the_day,state) values('%s','%s','%s',0,0,'F0A')", vectQueryElement[1].c_str(), vectQueryElement[2].c_str(), vectQueryElement[3].c_str());
//         sql = sqlChar;
// 	}
// 	else if(strcmp(vectQueryElement[0].c_str(), "delete") == 0)
// 	{   
//         char sqlChar[256];
//         sprintf(sqlChar, "delete from noteinfo where start_time like \' %s%%\' and info_seq = %d", dateStr.c_str(), atoi(vectQueryElement[2].c_str()));
//         sql = sqlChar;
// 	}
// 	else if(strcmp(vectQueryElement[0].c_str(), "done") == 0)
// 	{
//         char sqlChar[256];
//         sprintf(sqlChar, "update noteinfo set complete_flag = 1 where start_time like \' %s%%\' and info_sql = %d", dateStr.c_str(), atoi(vectQueryElement[2].c_str()));
//         sql = sqlChar;
// 	}
// 	else if(strcmp(vectQueryElement[0].c_str(), "list") == 0)
// 	{
//         sql = "select info_seq, start_time, end_time, issue_content, complete_flag, serial_num_of_the_day, state from noteinfo where start_time like \'" + dateStr + "%%\'" ;
// 	}
//     else if(strcmp(vectQueryElement[0].c_str(), "get") == 0)
//     {
//         char sqlChar[256];
//         sprintf(sqlChar, "select info_seq, start_time, end_time, issue_content, complete_flag, serial_num_of_the_day, state where start_time > %s and start_time < %s and complete_flag = 1", vectQueryElement[1].c_str(), vectQueryElement[2].c_str());
//         sql = sqlChar;
//     }

// 	int ret = mysql_query(mysql, sql.c_str());
// 	if(strcmp(vectQueryElement[0].c_str(), "list") == 0)//select operatition needs more work, like return result etc.
// 	{
// 		res = mysql_store_result(mysql);
// 		if(res == NULL)
// 		{
//             cerr << "mysql_store_result failed" << endl;
// 			return -1;
// 		}

// 		row = mysql_fetch_row(res);
// 		int fieldcount = mysql_num_fields(res);
// 		int rows = mysql_num_rows(res);
// 		char buf[32];
//         string queryResultBuf;
// 		int i, j;

// 		for(j = 0; j < rows; ++j)
// 		{
// 			for(i = 0; i < fieldcount; ++i)
// 			{
// 				//memset(buf, 0, sizeof(buf));
// 				if (row[i] != NULL && (strlen(row[i])>0))
// 				{
// 					//snprintf(buf, sizeof(buf), "%s", row[i]);
//                     queryResultBuf = row[i];
// 					response += queryResultBuf;
//                     response += " ";
// 					cout << queryResultBuf << " ";
// 				}
// 			}
//             response += "\n";
// 			cout << endl;
// 			row = mysql_fetch_row(res);
// 		}
// 		cout << "response: " <<endl;
//         cout << response;
// 	}
//     if(ret = 1)//success
//     {
//         response = response + vectQueryElement[0] + " success\n";
//         cout << "mysql operation success" << endl;
//     }
//     else
//     {
//         response = response + vectQueryElement[0] + " failed\n";        
//         cout << "mysql operation failed" << endl;
//         return -1;
//     }
// }