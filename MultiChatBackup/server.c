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

int nsfd[20];
int n;
int usfd,ffd,sfd;
int p;
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
int s = sendmsg(socket, &socket_message, 0);
perror("send");
  return s;
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
   return -1;

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





void * keyboard(void * f)
{
	char buff[20];
	while(1)
	{
		memset(buff,'\0',20);
		scanf("%s",buff);
		char b[20];
		strcpy(b,buff);
		if(strcmp(buff,"down")==0)
		{
			sprintf(buff,"%d%d",p,n);
			strcat(buff,"down");
			printf("%s\n",buff);
			write(ffd,buff,strlen(buff));
		}
		else if(strcmp(buff,"up")==0)
		{
			sprintf(buff,"%d%d",p,n);
			strcat(buff,"up");
			write(ffd,buff,strlen(buff));
		}
		usfd = socket(AF_UNIX, SOCK_STREAM, 0);
		sleep(2);
		struct sockaddr_un addr;
		memset(&addr, 0, sizeof(addr));
		addr.sun_family = AF_UNIX;
		strncpy(addr.sun_path, argv[1], sizeof(addr.sun_path)-1);
		printf("before connect\n");
		int f = connect(usfd,(struct sockaddr *)&addr,sizeof(addr));
		printf("%d\n", f);
		while(f<0) 
			f = connect(usfd,(struct sockaddr *)&addr,sizeof(addr));
		perror("connect");
		printf("%d\n", f);
		if(strcmp(b,"down")==0)
		{
			printf("sending\n");
			for(int i=0;i<n;i++)
			{
				send_fd(usfd,nsfd[i]);
				close(nsfd[i]);
				sleep(1);
			}
			close(usfd);
			printf("sent\n");
		}
		else if(strcmp(b,"up")==0)
		{
			printf("recieving\n");
			for(int i=0;i<n;i++)
			{
				nsfd[i] = recv_fd(usfd);
				printf("%d",nsfd[i]);
			}
			close(usfd);
			printf("recieved\n");
		}
	}
}

void *service(void *f)
{
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
			FD_SET(nsfd[i], &r);
			if(m<nsfd[i])
				m=nsfd[i];
		}	
		int s = select(m+1,&r,NULL,NULL,&t);
		if(s>0)
		{
			for(int i=0;i<n;i++)
			{
				if(FD_ISSET(nsfd[i],&r))
				{
					char b[50];
					if(read(nsfd[i],b,50)>0){
					printf("%s\n", b);
					for(int j=0;j<n;j++)
					{
						if(i==j)
							continue;
						write(nsfd[j],b,strlen(b));
					}
				}
				}
			}
		}
	}
	
}

int main(int argc, char * arg[])
{

	p = atoi(arg[3]);
	for(int i=0;i<argc;i++)
		strcpy(argv[i],arg[i]);

	mkfifo("./fifo",0666);
	ffd = open("./fifo",O_RDWR);

	pthread_t k;
	pthread_create(&k,NULL,&keyboard,NULL);


	sfd= socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(atoi(argv[2]));
	bind(sfd, (struct sockaddr*)&saddr, sizeof(saddr));
	listen(sfd,SOMAXCONN);
	pthread_t s;
	pthread_create(&s,NULL,&service,NULL);
	while(1)
	{
		nsfd[n] = accept(sfd,NULL,NULL);
		n++;
	}

}