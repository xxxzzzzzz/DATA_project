
/*wbed 单线程按键输入子系统; 作者：向仔州*/
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>  
#include <sys/types.h>  
#include <fcntl.h>  
#include <errno.h>  
#include <time.h>  
#include <linux/input.h>


int main(){
	int fd;
	int ret;
	struct input_event *keyEvent;
  
	keyEvent = malloc(sizeof(struct input_event));
	fd=open("/dev/event0",O_RDWR);
	if(fd<0){
		printf("open input event1 erro\n");	  
 	}

	memset(keyEvent, 0, sizeof(struct input_event));
	while(1){
		ret = read(fd,keyEvent,sizeof(struct input_event));
	if(ret<0){
		printf("read keyEvent erro\n");
	}

	/*如果只想知道按键是否按下，就用下面这个条件判断就可以了，不需要打开上面上报原理的打印段*/  
	if(keyEvent->code==KEY_L){  
 		printf("enter AP mode\n");	  
 	}
}

	free(keyEvent);
	close(fd);
}
