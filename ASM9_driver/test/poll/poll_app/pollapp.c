#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <poll.h>//要用poll函数就必须添加poll头文件

int main(){
int fd,fd2,res,ret;
unsigned char key;
struct pollfd fds[2];//因为一个驱动程序里面有两个poll，我是一个poll对应一个文件节点

fd = open("/dev/poll_dev",O_RDWR);
if(fd<0){

	printf("open poll_dev failed\n");
}

fd2 = open("/dev/poll_dev2",O_RDWR);
if(fd2<0){

	printf("open poll_dev2 failed\n");
}
fds[0].fd = fd;
fds[0].events = POLLIN;

fds[1].fd = fd2;
fds[1].events = POLLOUT;

while(1){
        ret  = poll(fds,2,1000);
	if(ret == 0){
		
		printf("time out 5\n");
	}
	else{
	res = read(fd,&key,1);
	printf("key = %d\n",key);
	}
	printf("while\n");

}
close(fd);
return 0;
}
