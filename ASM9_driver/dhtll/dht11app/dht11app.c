#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include<sys/ioctl.h>
#include <unistd.h>

/*data[0] is temperature ,data[1] is humi*/


int main(){
int fd;
unsigned int undatatem,undatahumi;
unsigned char data[2];
if((fd=open("/dev/dht11", O_RDWR | O_NDELAY))<0){ //open temphumi

printf("open temphumi failed!");	
}

while(1){
 printf("app read\n");
 read(fd, data, sizeof(data));//read temp humi value	
 printf("temperature = %d : humi = %d\n",data[0],data[1]);
 undatatem=(unsigned int)data[0];
 undatahumi=(unsigned int)data[1];
 printf("undtatyem =%d undtahumi = %d\n",undatatem,undatahumi);
 sleep(2);//因为dht11是温度传感器，必须用很慢的时间，大于2秒的时间等待传感器下次响应才行
}


return 0;
}
