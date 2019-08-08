#include<stdio.h>

int main()
{
	int fd = 0;
	fd = open("ID",O_RDWR| O_NDELAY)
	if(fd<0)
		printf("open ID failed..\n");
	else
		printf("open ID success..\n");
	system("cat ID")
}
