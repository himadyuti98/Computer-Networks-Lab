#include<stdio.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/un.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<errno.h>

int usfd,ffd;
int serv[20][10];
pthread_t bser[20];
char argv[20][30];


int send_fd(int socket, int fd_to_send)
 {
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  /* storage space needed for an ancillary element with a paylod of length is CMSG_SPACE(sizeof(length)) */
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];
  int available_ancillary_element_buffer_space;

  /* at least one vector of one byte must be sent */
  message_buffer[0] = 'F';
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;

  /* initialize socket message */
  memset(&socket_message, 0, sizeof(struct msghdr));
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  available_ancillary_element_buffer_space = CMSG_SPACE(sizeof(int));
  memset(ancillary_element_buffer, 0, available_ancillary_element_buffer_space);
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = available_ancillary_element_buffer_space;

  /* initialize a single ancillary data element for fd passing */
  control_message = CMSG_FIRSTHDR(&socket_message);
  control_message->cmsg_level = SOL_SOCKET;
  control_message->cmsg_type = SCM_RIGHTS;
  control_message->cmsg_len = CMSG_LEN(sizeof(int));
  *((int *) CMSG_DATA(control_message)) = fd_to_send;

  return sendmsg(socket, &socket_message, 0);
 }
 
 
 
 
 
 int recv_fd(int socket)
 {
  int sent_fd, available_ancillary_element_buffer_space;
  struct msghdr socket_message;
  struct iovec io_vector[1];
  struct cmsghdr *control_message = NULL;
  char message_buffer[1];
  char ancillary_element_buffer[CMSG_SPACE(sizeof(int))];

  /* start clean */
  memset(&socket_message, 0, sizeof(struct msghdr));
  memset(ancillary_element_buffer, 0, CMSG_SPACE(sizeof(int)));

  /* setup a place to fill in message contents */
  io_vector[0].iov_base = message_buffer;
  io_vector[0].iov_len = 1;
  socket_message.msg_iov = io_vector;
  socket_message.msg_iovlen = 1;

  /* provide space for the ancillary data */
  socket_message.msg_control = ancillary_element_buffer;
  socket_message.msg_controllen = CMSG_SPACE(sizeof(int));

  if(recvmsg(socket, &socket_message, MSG_CMSG_CLOEXEC) < 0)
  {
  	perror("recvmsg");
  	printf("%d\n", errno);
  	return -1;
  }
   
  if(message_buffer[0] != 'F')
  {
   /* this did not originate from the above function */
   return -1;
  }

  if((socket_message.msg_flags & MSG_CTRUNC) == MSG_CTRUNC)
  {
   /* we did not provide enough space for the ancillary element array */
   return -1;
  }

  /* iterate ancillary elements */
   for(control_message = CMSG_FIRSTHDR(&socket_message);
       control_message != NULL;
       control_message = CMSG_NXTHDR(&socket_message, control_message))
  {
   if( (control_message->cmsg_level == SOL_SOCKET) &&
       (control_message->cmsg_type == SCM_RIGHTS) )
   {
    sent_fd = *((int *) CMSG_DATA(control_message));
    return sent_fd;
   }
  }

  return -1;
 }





void *service(void *f)
{
	int * a = (int*)f;
	int sno = a[0];
	int n = a[1];
	int m = -1;
	fd_set r;
	struct timeval t;
	t.tv_sec=1;
	t.tv_usec=0;
	while(1)
	{
		FD_ZERO(&r);
		for(int i=0;i<n;i++)
		{
			FD_SET(serv[sno][i], &r);
			if(m<serv[sno][i])
				m=serv[sno][i];
		}	
		int s = select(m+1,&r,NULL,NULL,&t);
		if(s>0)
		{
			for(int i=0;i<n;i++)
			{
				if(FD_ISSET(serv[sno][i],&r))
				{
					char b[50];
					if(read(serv[sno][i],b,50)>0){
					printf("%s\n", b);
					for(int j=0;j<n;j++)
					{
						if(i==j)
							continue;
						write(serv[sno][j],b,strlen(b));
					}}
				}
			}
		}
	}
	
}

void * readfifo(void * f)
{
	//int sfd = *(int *)f;
	char buff[20];
	while(1)
	{
		read(ffd,buff,20);
		printf("read: %s\n", buff);
		int sno = buff[0]-'0';
		int n = buff[1]-'0';
		printf("%c %c\n", buff[2],buff[3]);
		usfd = socket(AF_UNIX, SOCK_STREAM, 0);
		sleep(1);
		struct sockaddr_un addr;
		memset(&addr, 0, sizeof(addr));
		addr.sun_family = AF_UNIX;
		strncpy(addr.sun_path, argv[1+sno], sizeof(addr.sun_path)-1);
		unlink(argv[1+sno]);
		printf("%d %s\n",sno, argv[1+sno]);
		bind(usfd, (struct sockaddr*)&addr, sizeof(addr));
		perror("bind");
		listen(usfd,5);
		perror("listen");
		//int flags = fcntl(usfd,F_GETFL,0);
		//fcntl(usfd,F_SETFL,flags|O_NONBLOCK);
		int sfd2 = accept(usfd,NULL,NULL);
		printf("sfd2=%d\n", sfd2);
		//if(sfd2<0)
		while(sfd2<0)
		{
			sfd2 = accept(usfd,NULL,NULL);
			//perror("accept");
		}
		perror("accept");
		if(buff[2]=='d' && buff[3]=='o' && buff[4]=='w' && buff[5]=='n')
		{
			printf("downdown\n");
			
			char b[30];
			strcpy(b,"served by backup server now\n");
			printf("%s\n", b);
			for(int i=0;i<n;i++)
			{
				serv[sno][i] = recv_fd(sfd2);
				printf("%d %d\n",serv[sno][i],sfd2);
				write(serv[sno][i],b,strlen(b));
			}
			int a[2];
			a[0] = sno;
			a[1] = n;
			pthread_create(&bser[sno],NULL,&service,a);
		}
		else if(buff[2]=='u' && buff[3]=='p')
		{
			printf("upup\n");
			
			pthread_cancel(bser[sno]);
			for(int i=0;i<n;i++)
			{
				send_fd(sfd2,serv[sno][i]);
				printf("sent\n");
				close(serv[sno][i]);
				sleep(1);
			}
		}
		else
			printf("%s\n", buff);
	}
}

int main(int argc, char const *arg[])
{
	

	for(int i=0;i<argc;i++)
	{
		strcpy(argv[i],arg[i]);
	}
	printf("copied\n");
	mkfifo("./fifo",0666);
	ffd = open("./fifo",O_RDWR);

	pthread_t f;
	pthread_create(&f,NULL, readfifo, NULL);
	pthread_join(f,NULL);

	return 0;
}