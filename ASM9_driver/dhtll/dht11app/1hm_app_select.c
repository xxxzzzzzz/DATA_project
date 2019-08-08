
/*1hm_sensor crib versions: wbed1.0
* author : 向仔州*/
/*1hm_sensor 4 application select operation*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/select.h>
#include<sys/time.h>

int main(void)
{
	int weight1;
	unsigned long data1 = 0,original = 0;
	int ret = 0;
	struct timeval tm;
	
	/*weight1 open sensor*/
	if((weight1=open("/dev/threesboy_weight1", O_RDWR | O_NDELAY))<0)
		printf("APP open weight1_class failed\n"); 
	else 
		printf("APP open weight1_class success\n");

	fd_set fdset;

	FD_ZERO(&fdset);
	FD_SET(weight1,&fdset);
	tm.tv_sec = 1;
	tm.tv_usec = 0;
	ret = select(weight1+1,&fdset,NULL,NULL,&tm);
	if(ret<0)
		printf("select error\n");
	else if(ret == 0)
	{
		printf("disconnect time out \n");//data line disconnect
		
	}
	else
		read(weight1, &original, sizeof(&original));//read original weight1 value

	while(1)
	{
		
		tm.tv_sec = 0;
		tm.tv_usec = 0;
		FD_ZERO(&fdset);
		FD_SET(weight1,&fdset);
		ret = select(weight1+1,&fdset,NULL,NULL,&tm);
		if(ret<0)
			printf("select error\n");
		else if(ret == 0)
		{
			printf("disconnect time out \n");//data line disconnect
		}
		else
		{
			read(weight1, &data1, sizeof(&data1));//read weight1 value
			data1 = data1 - original;
			printf("weight1 data =%ld\n",data1);
		}
		usleep(10);
	}
	return 0;
}
