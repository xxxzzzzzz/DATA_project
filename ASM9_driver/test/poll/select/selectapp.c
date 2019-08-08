#include<stdio.h>
#include<sys/select.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <stdlib.h> 


int main()
{
	int fd = -1,ret = 0;
	fd = open("/dev/poll_dev",O_RDWR);
	if(fd<0){

		printf("open poll_dev failed\n");
	}

	fd_set fdset;
	FD_ZERO(&fdset);
	FD_SET(fd,&fdset);

	struct timeval tm;

	while(1)
	{
	tm.tv_sec = 1;
	tm.tv_usec = 0;
	ret = select(fd+1,&fdset,NULL,NULL,&tm);
	if(ret<0)
		printf("select error\n");
	else if(ret == 0)
		printf("select time out\n");
	else
	{
		if(FD_ISSET(fd,&fdset))
		{
			printf("read data\n");
		}
	}
}
	close(fd);
	return 0;
}
