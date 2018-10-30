#include "noteServer.h"

int main(int argc, char *argv[])
{
    int sockfd, n;
	socklen_t len;
	char buff[1024];
	struct sockaddr_in servaddr;
    char cmd[16], start_time[32], end_time[32];
    char content[1024];
    char cmdMsg[1024];

	if(argc != 5)
    {
        fprintf(stderr, "usage: NoteClient [add/list/delete/done] start_time end_time content\n");
        exit(EXIT_FAILURE);
    }
		

    strcpy(cmd, argv[1]);
    strcpy(start_time, argv[2]);
    strcpy(end_time, argv[3]);
    strcpy(content, argv[4]);
    sprintf(cmdMsg, "%s %s %s %s", cmd, start_time, end_time, content);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
	{
		fprintf(stderr, "socket failed");
		exit(EXIT_FAILURE);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(6610);

	connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	n = write(sockfd, cmdMsg, strlen(cmdMsg));
    if(n < 0)
    {
        fprintf(stderr, "write error!");
        exit(EXIT_FAILURE);
    }
    //waiting for answer

	exit(EXIT_SUCCESS);

	exit(0);
}