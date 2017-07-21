#include<stdio.h>
#include <fcntl.h>
#include <stdlib.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(){
int fd;

fd=creat("../biaoding.txt",0600);

if(fd<0){printf("txt creat failed\n");}
else
	printf("txt creat success\n");

close(fd);

return 0;
}
