#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/watchdog.h>
#define WDIOC_SETMSTIMER 0xfedcba//_IOWR(WATCHDOG_IOCTL_BASE, 101, int)
#define WDIOC_GETMSTIMER 0xfedcb9//_IOR(WATCHDOG_IOCTL_BASE, 102, int)


int main(void)
{
	int fdw = 0,wdt_val = 0,timer_val = 0,ret = 0;
	
	timer_val = 0;	
	wdt_val = 10;//看门狗设置板子重启时间为10秒之后	
	if((fdw=open("/dev/watchdog",O_RDWR))==-1)
	{
		printf("Can not open /dev/watchdog\n");
	}
	
	ret=ioctl(fdw,WDIOC_SETTIMEOUT,&wdt_val);//设置看门狗
	while(1)
	{	
		sleep(8);
		ret=ioctl(fdw,WDIOC_SETMSTIMER,&timer_val);//如果设置的看门狗重启时间为10秒，那么在8秒左右喂狗，可以清0看门狗重新计数。不要等到10秒喂狗，因为有喂狗时间延时，会导致板子重启
	}
	close(fdw);//如果打开了看门狗就不要再关闭设备节点，因为你会随时喂狗
	return 0;
}
