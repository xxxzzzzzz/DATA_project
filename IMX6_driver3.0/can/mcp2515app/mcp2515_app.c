#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include<sys/ioctl.h>
#include <unistd.h>


int main(){
	int fd;

	fd = open("/dev/mcp2515_can", 0); //open 3 led
	if(fd<0){
	printf("led open failed!");	
        }

	while(1){
	}

	return 0;
}
