#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include<sys/ioctl.h>
#include <unistd.h>




int main(){
int fd,ret = -1;

unsigned char i,dataID[8];

	if((fd=open("/dev/ds2401", O_RDWR | O_NDELAY))<0)
	{ 
 		printf("open ds2401 failed!");	
	}

while(1)
{
	usleep(100000);
	ret = read(fd, dataID, sizeof(dataID));
 	if(ret<0)
	{
		printf("app read ID failed\n");
 		continue;
	}
 	for(i = 0;i<8;i++)
 	{
		 printf("app read ID = %d\n",dataID[i]);
 	}
		 printf("-----------------------------\n");
	
}

	return 0;
}
