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
if((fd=open("/dev/bh1750", O_RDWR | O_NDELAY))<0){ //open temphumi

printf("open bh1750 failed!");	
}

while(1){ 

read(fd,&data,sizeof(&data));
printf("bh1750 = %d\n",data);
usleep(10000);

}
close(fd);
return 0;
}
