#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	int					listenfd, connfd;//定义两个文件描述符（监听、连接）
	socklen_t			len;//socket长度
	struct sockaddr_in	servaddr, cliaddr;//客户端和服务端的socket地址结构体
	char				buff[MAXLINE];//缓冲区
	time_t				ticks;//时间

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);//AF_INET指定ipv4

	bzero(&servaddr, sizeof(servaddr));//重置指针
	servaddr.sin_family      = AF_INET;//指定协议族为ipv4
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//
	servaddr.sin_port        = htons(13);	/* daytime server */

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));//绑定

	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		len = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &len);
		printf("connection from %s, port %d\n",
			   Inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
			   ntohs(cliaddr.sin_port));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

		Close(connfd);
	}
}
