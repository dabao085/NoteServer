#include "noteClient.h"
using namespace std;

void printWelcome()
{
    cout << "Welcome to use NoteClient, press 'quit' to exit, press 'h' to get help" << endl;
    cout << "add start_time end_time content --add your content start by start_time and end by end_time" << endl;
    cout << "list datetime --return all contents in datetime" << endl;
    cout << "delete datetime info_seq --delete info in datetime by info_seq" << endl;
    cout << "done info_seq --mark info as done in datetime by info_seq" << endl;
    cout << "+++++++++++++Example++++++++++++++" << endl;
    cout << "add 2018-08-28_09:00:00 2018-08-28_10:00:00 coding" << endl;
    cout << "list 2018-08-28" << endl;
    cout << "delete 2018-08-28 1" << endl;
    cout << "done 1" << endl;
}

int main(int argc, char **argv)
{
    int sockfd, n;
	socklen_t len;
	struct sockaddr_in servaddr;
    string sendBufStr;

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(6610);

    printWelcome();
	Connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

    while(1){
        getline(cin, sendBufStr);
        if(strcasecmp(sendBufStr.substr(0, 4).c_str(), "quit") == 0){
            cout << "Program will exit" << endl;
            break;
        }else if (strcasecmp(sendBufStr.substr(0, 1).c_str(), "h") == 0)
        {
            printWelcome();
            continue;
        }

	    write(sockfd, sendBufStr.c_str(), sendBufStr.length());      
        char recvBuff[MAXLEN];
        n = read(sockfd, recvBuff, MAXLEN);
        recvBuff[n] = '\0';

        cout << "recvBuff: "<< recvBuff << endl;
    }
	return 0;  
}