#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>  
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>


int main(){
int fd;
int zheng=16745493;
int zheng2=16745118;
int zheng3=16745414;
int zheng4=16475555;
float xisu=1.946283;
char buffer[20];

char zifu[]="\n";
int length;

fd=open("../biaoding.txt",O_RDWR,0600);
if(fd<0)
	printf("txt open failed\n");
else
	printf("txt open success\n");

sprintf(buffer,"%d",zheng);
length=write(fd,buffer,strlen(buffer));
if(length==-1)
	printf("txt write failed\n");
else
	printf("txt write success\n");

length=write(fd,zifu,strlen(zifu));
if(length==-1)
	printf("txt write failed\n");
else
	printf("txt write success\n");

sprintf(buffer,"%d",zheng2);
length=write(fd,buffer,strlen(buffer));
if(length==-1)
	printf("txt write failed\n");
else
	printf("txt write success\n");


length=write(fd,"\n",strlen("\n"));
if(length==-1)
	printf("txt write failed\n");
else
	printf("txt write success\n");

sprintf(buffer,"%d",zheng3);
length=write(fd,buffer,strlen(buffer));
if(length==-1)
	printf("txt write failed\n");
else
	printf("txt write success\n");


length=write(fd,"\n",strlen("\n"));
if(length==-1)
	printf("txt write failed\n");
else
	printf("txt write success\n");

sprintf(buffer,"%d",zheng4);
length=write(fd,buffer,strlen(buffer));
if(length==-1)
	printf("txt write failed\n");
else
	printf("txt write success\n");


length=write(fd,"\n",strlen("\n"));
if(length==-1)
	printf("txt write failed\n");
else
	printf("txt write success\n");

sprintf(buffer,"%f",xisu);
length=write(fd,buffer,strlen(buffer));
if(length==-1)
	printf("txt write failed\n");
else
	printf("txt write success\n");
return 0;
}
