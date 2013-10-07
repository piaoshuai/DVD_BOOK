#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stddef.h>        /* for offsetof */
#include <string.h>        /* for convenience */
#include <unistd.h>        /* for convenience */
#include <signal.h>        /* for SIG_ERR */
#include <errno.h>
#include <syslog.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/resource.h>

#include<netinet/in.h>//struct sockaddr_in

#define BUFLEN	128
#define QLEN 10

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

#include "ctrlcmds.h"
#include "ctrlprefix.h"
#include "zynq_server.h"
#include "smart_car.h"

int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
	int fd;
	int err = 0;

	if ((fd = socket(addr->sa_family, type, 0)) < 0)
		return(-1);
	if (bind(fd, addr, alen) < 0) {
		err = errno;
		goto errout;
	}
	if (type == SOCK_STREAM || type == SOCK_SEQPACKET) {
		if (listen(fd, qlen) < 0) {
			err = errno;
			goto errout;
		}
	}
	return(fd);

errout:
	close(fd);
	errno = err;
	return(-1);
}

int		clfd;
int hrobot_tcp_receive(TCP_CTRL_FRAME *frame, uint8_t *buf)
{
	int n,i;
	int fd;
	int16_t angle,speed;
	char cmdbuf[5]="0000";
	while(1){
		angle=0x0000;
		speed=0x0000;
		memset(frame,0,sizeof(TCP_CTRL_FRAME));
		memset(buf,0,HROBOT_TCP_MAX_DATA_LENGTH);

		if ((n = recv(clfd,frame, HROBOT_TCP_CTRL_FRAME_SIZE, 0)) > 0)
		{
			frame->cmd = ntohs(frame->cmd);
			frame->length = ntohs(frame->length);
			syslog(LOG_ERR,"frame_CTR:recv %d data",n);
			syslog(LOG_ERR,"cmd:%d, length:%d",frame->cmd, frame->length);
		/*	cmdbuf[0]='0'+frame->cmd/10;
			cmdbuf[1]='0'+frame->cmd%10;
			cmdbuf[2]='0'+frame->length/10;
			cmdbuf[3]='0'+frame->length%10;
			cmdbuf[4]='Z';
			if ((fd=open("/home/boat/state_tcp.txt",O_WRONLY|O_CREAT|O_APPEND,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH))<0)
					syslog(LOG_ERR,"fd=%d open error \n",fd);
			if (write(fd,&cmdbuf,5)<0)
					syslog(LOG_ERR,"write error \n");
			close(fd);
			*/
		}
		else{
			return 0;
		}
		if ((n = recv(clfd, buf, frame->length, 0)) > 0)
		{
			syslog(LOG_ERR,"frame_DATA:recv %d data",n);
			for(i=0;i<n;++i)
				syslog(LOG_DEBUG,"recv data:%x",buf[i]);
			angle=(buf[1]<<8)|buf[0];
			speed=(buf[3]<<8)|buf[2];
			
			syslog(LOG_DEBUG,"angle=%d,speed=%d",angle,speed);
			smart_car_set(speed,angle);
		}
		else{
			return 0;
		}
		

	}

}

void serve(int sockfd)
{

	struct	sockaddr_in addr_client;
	int 	client_len;
	
	TCP_CTRL_FRAME ctrl_frame;
	uint8_t data[HROBOT_TCP_MAX_DATA_LENGTH];

	
	client_len = sizeof(struct sockaddr_in);

	for (;;) {
		clfd = accept(sockfd,(struct sockaddr *)&addr_client,&client_len);
		if (clfd < 0) {
			syslog(LOG_ERR, "Zynq@Server: accept error: %s",strerror(errno));
			exit(1);
		}
		syslog(LOG_ERR,"accept a new client,ip:%s\n",inet_ntoa(addr_client.sin_addr));
		
		hrobot_tcp_receive(&ctrl_frame, data);
		close(clfd);
	}
}

int main(int argc, char *argv[])
{
	struct addrinfo	*ailist, *aip;
	struct addrinfo	hint;
	int				sockfd, err, n;
	char			*host;

	if (argc != 1)
		err_quit("usage: ./%s",argv[0]);
#ifdef _SC_HOST_NAME_MAX
	n = sysconf(_SC_HOST_NAME_MAX);
	if (n < 0)	/* best guess */
#endif
		n = HOST_NAME_MAX;
	host = malloc(n);
	if (host == NULL)
		err_sys("malloc error");
	if (gethostname(host, n) < 0)
		err_sys("gethostname error");
	init_LED();
	printf("hostname=%s\n",host);

	daemonize("Zynq@Server");
	hint.ai_flags = AI_PASSIVE;
	hint.ai_family = 0;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = NULL;
	hint.ai_addr = NULL;
	hint.ai_next = NULL;
	if ((err = getaddrinfo(host, "2001", &hint, &ailist)) != 0) {
		syslog(LOG_ERR, "Zynq@Server: getaddrinfo error: %s",
		  gai_strerror(err));
		exit(1);
	}
	for (aip = ailist; aip != NULL; aip = aip->ai_next) {
		if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr,
		  aip->ai_addrlen, QLEN)) >= 0) {
			serve(sockfd);
			exit(0);
		}
	}
	exit(1);
}
