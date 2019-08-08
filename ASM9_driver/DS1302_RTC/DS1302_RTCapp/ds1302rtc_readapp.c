#include <stdio.h> 
#include <linux/rtc.h> 
#include <sys/ioctl.h> 
#include <sys/time.h> 
#include <sys/types.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <errno.h>




int main(){
	int fd,ret;
	int mon;
	struct rtc_time rtc_tm;
	struct rtc_time rtc_rtm;
	fd=open("/dev/rtc0",O_RDWR);
	if(fd<0){
		printf("open rtc1 erro\n");
	}
	while(1){
	ioctl(fd, RTC_RD_TIME, &rtc_rtm);

	printf(" app read sec=%d min=%d hour=%d day=%d tm_mon=%d tm_year=%d\n",rtc_rtm.tm_sec,rtc_rtm.tm_min,rtc_rtm.tm_hour,rtc_rtm.tm_mday,rtc_rtm.tm_mon+1,rtc_rtm.tm_year);

	sleep(1);
	}
	close(fd);
	return 0;
}
