#include "noteClient.h"
using namespace std;

void usage()
{
    cerr << "usage: please enter your choice and parameters" << endl;
    cerr << "NoteClient add start_time end_time content --add your content start by start_time and end by end_time" << endl;
    cerr << "NoteClient list datetime --return all contents in datetime" << endl;
    cerr << "NoteClient delete datetime info_seq --delete info in datetime by info_seq" << endl;
    cerr << "NoteClient done datetime info_seq --mark info as done in datetime by info_seq" << endl;
    cerr << "NoteClient get start_time end_time  --get all info between start_time and end_time" << endl;

    cerr << endl;
    cerr << "example:" << endl;
    cerr << "NoteClient add 2018-08-28_09:00:00 2018-08-28_10:00:00 coding" << endl;
    cerr << "NoteClient list 2018-08-28" << endl;
    cerr << "NoteClient delete 2018-08-28 1" << endl;
    cerr << "NoteClient done 2018-08-28 1" << endl;
    cerr << "NoteClient get 2018-08-28_09:00:00 2018-08-28_10:00:00" << endl; 
}

int main(int argc, char *argv[])
{
    int sockfd, n;
	socklen_t len;
	struct sockaddr_in servaddr;

	if(argc < 2)
    {
        usage();
        return -1;
    }

    string cmdMsg;
    for(int i = 1; i < argc; ++i)
    {
        cmdMsg += argv[i];
        cmdMsg += " ";
    }

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
        cerr << "socket failed" << endl;
		return -1;
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(6610);

	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	n = write(sockfd, cmdMsg.c_str(), cmdMsg.length());
    if(n < 0)
    {
        cerr << "write error" << endl;
        return -1;
    }
    
    char recvBuff[1024];
    n = read(sockfd, recvBuff, 1024);
    recvBuff[n] = '\0';

    cout << "recvBuff: "<< endl;
    cout << recvBuff ;
	return 0;  
}