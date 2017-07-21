#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "i2c-dev.h"
#include <unistd.h> 



int fd;


int main(void){
unsigned int data,i;
unsigned char buf[3];
fd=open("/dev/i2c-1",O_RDWR);  	

if(fd<0){
  printf("i2c-1 open erro\n");
}

if(ioctl(fd,I2C_SLAVE,0x46)<0){

	printf("ioctl erro\n");
}
while(1){
i2c_smbus_write_byte(fd,0x01);
i2c_smbus_write_byte(fd,0x10);
printf("out 1\n");
usleep(180000);

printf("out 2\n");

buf[0]=i2c_smbus_read_byte(fd);
buf[1]=i2c_smbus_read_byte(fd);
buf[2]=i2c_smbus_read_byte(fd);

//buf[2]=i2c_smbus_read_word_data(fd,0x00);
printf("out 3\n");


printf("out 4\n");
data=buf[0];
data=(data<<8)+buf[1];
printf("BH1750 out %d\n",data);
return 0;
}
close(fd);
}
