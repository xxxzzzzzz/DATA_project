#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include<sys/ioctl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

//#define DEBUG
int main(){
	int fd = 0,devicefd = 0,ret = -1,flag = 1;
	unsigned char i,dataID[8];
	unsigned char IDstr[32];

	memset(IDstr,'\0',sizeof(IDstr));
	if((devicefd=open("/dev/ds2401", O_RDWR | O_NDELAY))<0)
	{ 
 		printf("open ds2401 failed!");	
	}


	while(flag == 1)
	{
		usleep(100000);
		ret = read(devicefd, dataID, sizeof(dataID));
 		if(ret<0)
		{
			printf("app read ID failed\n");
			flag=1;
 			continue;
		}
		flag--;
 		for(i = 0;i<8;i++)
 		{
			 printf("app read ID = %d\n",dataID[i]);
 		}
			 printf("-----------------------------\n");
	
#ifdef DEBUG
		dataID[0]=255; dataID[1]=255; dataID[2]=255; dataID[3]=255; dataID[4]=255; dataID[5]=255; dataID[6]=255; dataID[7]=255;		
#endif
		sprintf(IDstr,"%s%d%d%d%d%d%d%d%d","ASM9260T",dataID[0],dataID[1],dataID[2],dataID[3],dataID[4],dataID[5],dataID[6],dataID[7]);
		fd = open("./ID",O_RDWR|O_CREAT);
		if(fd<0)
			printf("ID create failed \n");
		ret = write(fd,IDstr,sizeof(IDstr));
		if(ret<0)
			printf("write ID failed\n");
		printf("write ID success \n");
		close(devicefd);
		close(fd);
	return 0;
	}

}
