#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h> 
#include <math.h>


int main(int arc,char *argv[]){

	int fd0,fd1,fd2;
	if((fd0 = open("/dev/xzz_dev_class0",O_RDWR | O_NDELAY))<0)
		printf("open xzz_dev_class0 failed\n");	
	else
		printf("open xzz_dev_class0 success\n");

	if((fd1 = open("/dev/xzz_dev_class1",O_RDWR | O_NDELAY))<0)
		printf("open xzz_dev_class1 failed\n");	
	else
		printf("open xzz_dev_class1 success\n");

	if((fd2 = open("/dev/xzz_dev_class2",O_RDWR | O_NDELAY))<0)
		printf("open xzz_dev_class2 failed\n");	
	else
		printf("open xzz_dev_class2 success\n");

	close(fd0);
	close(fd1);
	close(fd2);
	return 0;
}

