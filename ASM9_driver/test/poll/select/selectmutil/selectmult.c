#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h> 
#include<pthread.h>
#include<sys/select.h>
#include<sys/time.h>

void func2(int *arg)
{
	int ret,res,fd;
	unsigned char key;
	fd = open("/dev/poll_dev2",O_RDWR);
	if(fd<0)
		printf("open poll_dev2 failed\n");
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
			printf("func2 select error\n");
		if(ret == 0)
			printf("func2 time out... \n");
		else
		{
			res = read(fd,&key,1);
			printf("func2 key = %d\n",key);
			sleep(1);
		}
	}
	close(fd);
}

void func1(int *arg)
{
	int ret,res,fd;
	unsigned char key;
	fd = open("/dev/poll_dev",O_RDWR);
	if(fd<0)
		printf("open poll_dev failed\n");
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
			printf("func1 select error\n");
		if(ret == 0)
			printf("func1 time out... \n");
		else
		{
			res = read(fd,&key,1);
			printf("func1 key = %d\n",key);
			sleep(1);
		}
	}
	close(fd);

}

int main(){
	int ret;
	pthread_t th1 = -1,th2 = -1;
	ret = pthread_create(&th1,NULL,(void *)func1,NULL);
	if(ret<0)//线程创建失败返回-1
		printf("func1 pthread create failed\n");

	ret = pthread_create(&th2,NULL,(void *)func2,NULL);
	if(ret<0)//线程创建失败返回-1
		printf("func2 pthread create failed\n");

	while(1){
		sleep(1);		
	}
	return 0;
}
