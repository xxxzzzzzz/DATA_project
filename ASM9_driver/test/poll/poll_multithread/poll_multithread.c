#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h> 
#include<poll.h>//要用poll函数就必须添加poll头文件
#include<pthread.h>

void func2(int *arg)
{
	int ret,res,fd;
	unsigned char key;
	struct pollfd fds;
	fd = open("/dev/poll_dev2",O_RDWR);
	if(fd<0)
		printf("open poll_dev failed\n");
	fds.fd = fd;
	fds.events = POLLOUT;
	while(1)
	{
		ret = poll(&fds,1,1000);
		if(ret == 0)
			printf("func2 time out... \n");
		else
		{
			res = read(fd,&key,1);
			printf("func2 key = %d\n",key);
			sleep(1);
		}
	}

}

void func1(int *arg)
{
	int ret,res,fd;
	unsigned char key;
	struct pollfd fds;
	fd = open("/dev/poll_dev",O_RDWR);
	if(fd<0)
		printf("open poll_dev failed\n");
	fds.fd = fd;
	fds.events = POLLIN;
	while(1)
	{
		ret = poll(&fds,1,1000);
		if(ret == 0)
			printf("func1 time out... \n");
		else
		{
			res = read(fd,&key,1);
			printf("func1 key = %d\n",key);
			sleep(1);
		}
	}

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
