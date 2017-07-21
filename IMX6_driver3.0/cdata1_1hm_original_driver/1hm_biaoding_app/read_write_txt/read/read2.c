#include<stdio.h>
#include <fcntl.h>
#include <stdlib.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<string.h>


int main(){
int fd;
int buf;
int buf2;
int length;

fd=open("/home/xiang/app/biaoding.txt",O_RDWR);
if(fd<0)
	printf("txt open failed\n");
else
	printf("txt open success\n");

length=read(fd,&buf,sizeof(&buf));

if(length== -1) 
	printf("read txt erro\n");
else
	printf("read txt success\n");

printf("read out buf = %d\n",buf);
printf("read out buf2 = %d\n",buf2);



close(fd);
return 0;
}

