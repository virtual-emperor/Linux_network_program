
/*client.c*/

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

#define PORT	4321
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	int sockfd, sendbytes;
	char buf[BUFFER_SIZE];
	struct hostent *host;
	struct sockaddr_in serv_addr;
	
	if(argc < 2)
	{
		fprintf(stderr,"USAGE: ./client Hostname(or ip address) Text\n");
		exit(1);
	}
	
	/*地址解析函数*/
	if ((host = gethostbyname(argv[1])) == NULL)
	{
		perror("gethostbyname");
		exit(1);
	}
	
	//memset(buf, 0, sizeof(buf));
	//sprintf(buf, "%s", argv[2]);
	
	/*创建socket*/
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("socket");
		exit(1);
	}
	
	/*设置sockaddr_in 结构体中相关参数*/
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(serv_addr.sin_zero), 8);
	
	/*调用connect函数主动发起对服务器端的连接*/
	if(connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(struct sockaddr))== -1)
	{
		perror("connect");
		exit(1);
	}
	int n;
        char buff[100];
         while((recv(sockfd ,buff,10,0))>0){
            buff[10]='\0';
            printf("诗歌是:%s\n",buff);
            sleep(1);
        }
          
	/*发送消息给服务器端*/
	//if ((sendbytes = send(sockfd, buf, strlen(buf), 0)) == -1)
	//{
		//perror("send");
		//exit(1);
	//}
	close(sockfd);
	exit(0);
}
