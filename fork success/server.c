
/*server.c*/

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netinet/in.h>
#include<fcntl.h>
#include<sys/stat.h>

#define PORT			4321
#define BUFFER_SIZE		1024
#define MAX_QUE_CONN_NM	5


void do_service(int client_fd,int num){

    printf("进程号为 %d的进程开始继续诗歌传送",num);
    int fd = -1;
    ssize_t size =-1;  
    char buf[1024];   
    char filename[] = "test.txt";
    fd = open(filename,O_RDONLY);
    if(-1==fd){
   
     printf("Open file %s failuer,fd:%d\n",filename,fd);
   
     }else{
            printf("Open file %s success,fd:%d\n",filename,fd);
                 }   

   
        size = read(fd,buf,1024);
        if(-1==size)
        {   
            close(fd);
            printf("Read file %s error occurs\n",filename);
            return -1; 
        }else{
            if(send(client_fd,buf,strlen(buf),0)== -1 ){                     
                 perror("send");
		 exit(1);
                }else{
                       printf("send success congulation!"); 
                      }

         }

}

int main()
{
	struct sockaddr_in server_sockaddr, client_sockaddr;
	int sin_size, recvbytes;
	int sockfd, client_fd;
	//char buf[BUFFER_SIZE];
	
	/*建立socket连接*/
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0))== -1)
	{
		perror("socket");
		exit(1);
	}
	printf("Socket id = %d\n",sockfd);
	
	/*设置sockaddr_in 结构体中相关参数*/
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(PORT);
	server_sockaddr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_sockaddr.sin_zero), 8);
	
	int i = 1;/* 使得重复使用本地地址与套接字进行绑定 */
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));	
	
	/*绑定函数bind*/
	if (bind(sockfd, (struct sockaddr *)&server_sockaddr, sizeof(struct sockaddr))== -1)
	{
		perror("bind");
		exit(1);
	}
	printf("Bind success!\n");
	
	/*调用listen函数*/
	if (listen(sockfd, MAX_QUE_CONN_NM) == -1)
	{
		perror("listen");
		exit(1);
	}
	printf("Listening....\n");
	
     pid_t pid;

     while(1){
        /*调用accept函数，等待客户端的连接*/
	if ((client_fd = accept(sockfd, (struct sockaddr *)&client_sockaddr, &sin_size)) == -1)
	{
		perror("accept");
		exit(1);
	}
     
       pid=fork();
       if(pid==-1)
       {
         printf("fork error\n");
        }
       if(pid ==0){
               int num=getpid();
             do_service(client_fd,num);
              exit(EXIT_SUCCESS);
          }else{
           close(client_fd);
        }


      }

	
      /* 从文件中读出数据，并且将数据发送到客户端    */
 

    
        
   // } 
	

	/*调用recv函数接收客户端的请求*/
	//memset(buf , 0, sizeof(buf));
	//if ((recvbytes = recv(client_fd, buf, BUFFER_SIZE, 0)) == -1)
	//{
		//perror("recv");
		//exit(1);
	//}
	//printf("Received a message: %s\n", buf);


	close(sockfd);
	exit(0);
}
