/*1hm_sensor crib versions: wbed1.0
* author : 向仔州*/
/*1hm_sensor 4 application poll operation*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

int main(void)
{
	int weight1;
	unsigned long data1 = 0,original = 0;
	int ret = 0;
	struct pollfd fds[1];
	/*weight1 open sensor*/
	if((weight1=open("/dev/threesboy_weight1", O_RDWR | O_NDELAY))<0)
		printf("APP open weight1_class failed\n"); 
	else 
		printf("APP open weight1_class success\n");

	fds[0].fd = weight1;
	fds[0].events = POLLIN;
        ret  = poll(fds,1,1);
	if(ret == 0){
		printf("disconnect time out \n"); //data line disconnect
	}
	else
	{
		
		read(weight1, &original, sizeof(&original));//read original weight1 value
	}

	while(1)
	{
        	ret  = poll(fds,1,1);
		if(ret == 0){
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
/*	while(1)
	{
		read(weight1, &data1, sizeof(&data1));//read weight1 value
		printf("weight1 data =%ld\n",data1);
	       	usleep(10);		
	}*/
	close(weight1);
    return 0; 
}
