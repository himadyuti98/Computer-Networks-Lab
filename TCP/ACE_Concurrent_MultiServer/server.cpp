#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Stream.h>
#include<ace/Handle_Set.h>
#include<stdio.h>
#include<stdlib.h>

char buff[30];

//static ACE_SOCK_Acceptor acceptor;
static ACE_Handle_Set activity_handles;// Scratch copy of activity_handlesstatic 
static ACE_Handle_Set ready_handles;

int main()
{
	//ACE_SOCK_Stream sfd[3];
	ACE_INET_Addr addr1(7660,INADDR_LOOPBACK);
	ACE_INET_Addr addr2(7661,INADDR_LOOPBACK);
	ACE_INET_Addr addr3(7662,INADDR_LOOPBACK);
	ACE_SOCK_Acceptor ac1(addr1);
	ACE_SOCK_Acceptor ac2(addr2);
	ACE_SOCK_Acceptor ac3(addr3);
	activity_handles.set_bit (ac1.get_handle ());
	activity_handles.set_bit (ac2.get_handle ());
	activity_handles.set_bit (ac3.get_handle ());
	while(1)
	{
		ready_handles.reset();
		ready_handles = activity_handles;
		ACE::select(ACE_DEFAULT_SELECT_REACTOR_SIZE,ready_handles);
		if(ready_handles.is_set(ac1.get_handle()))
		{
			ACE_SOCK_Stream str;
			if(ac1.accept(str)!=-1)
			{
				int c = fork();
				if(c==0)
				{
					while(1)
					{
						int s = str.recv_n(buff,50);
						if(s>0)
						{
							printf("acc1 %s\n",buff);
						}
						else break;
					}
				}
				else
					str.close();
				
				/*activity_handles.set_bit (ac1.get_handle ());
				activity_handles.set_bit (ac2.get_handle ());
				activity_handles.set_bit (ac3.get_handle ());*/
			}
		}
		else if(ready_handles.is_set(ac2.get_handle()))
		{
			ACE_SOCK_Stream str;
			if(ac2.accept(str)!=-1)
			{
				int c = fork();
				if(c==0)
				{
					while(1)
					{
						int s = str.recv_n(buff,50);
						if(s>0)
						{
							printf("acc2 %s\n",buff);
						}
						else break;
					}
				}
				else
					str.close();
				
				/*activity_handles.set_bit (ac1.get_handle ());
				activity_handles.set_bit (ac2.get_handle ());
				activity_handles.set_bit (ac3.get_handle ());*/
			}
		}
		else if(ready_handles.is_set(ac3.get_handle()))
		{
			ACE_SOCK_Stream str;
			if(ac3.accept(str)!=-1)
			{
				int c = fork();
				if(c==0)
				{
					while(1)
					{
						int s = str.recv_n(buff,50);
						if(s>0)
						{
							printf("acc3 %s\n",buff);
						}
						else break;
					}
				}
				else
					str.close();
				
				/*activity_handles.set_bit (ac1.get_handle ());
				activity_handles.set_bit (ac2.get_handle ());
				activity_handles.set_bit (ac3.get_handle ());*/
			}
		}

	}
}