#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include<sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>




int main(void){
	
	int fd;
	unsigned int data;
	float lx;
	if((fd=open("/dev/bh1750", O_RDWR | O_NDELAY))<0){ //open temphumi

	printf("open bh1750 failed!");	
	}

	while(1){ 

	usleep(180000);//每一次读取数据的时候必须要延时180ms
	read(fd,&data,sizeof(&data));
	lx=(float)data/1.2;
	printf("bh1750 = %f\n",lx);//lx 才是最终输出的光亮值

	}
	close(fd);
	return 0;
}
