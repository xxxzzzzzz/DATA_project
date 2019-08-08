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
	struct rtc_time rtc_alrsettm;
	struct rtc_time rtc_alrgetrtm;

	rtc_tm.tm_sec =200;
	rtc_tm.tm_min=200;
	rtc_tm.tm_hour=200;
	rtc_tm.tm_mday=200;
	mon=200;
	rtc_tm.tm_mon=(mon);
	rtc_tm.tm_year=200;

	rtc_alrsettm.tm_sec =500;
	rtc_alrsettm.tm_min=500;
	rtc_alrsettm.tm_hour=500;
	rtc_alrsettm.tm_mday=500;
	mon=500;
	rtc_alrsettm.tm_mon=(mon);
	rtc_alrsettm.tm_year=500;


/*	fd=open("/dev/rtc0",O_RDWR);
	if(fd<0){

		printf("open rtc0 error\n");
	}

	ioctl(fd, RTC_SET_TIME, &rtc_tm);
	ioctl(fd, RTC_ALM_SET, &rtc_alrsettm);
	close(fd);*/

	fd=open("/dev/rtc0",O_RDWR);
	if(fd<0){

		printf("open rtc0 error\n");
	}
	while(1){
	ioctl(fd, RTC_RD_TIME, &rtc_rtm);

	printf(" app read sec=%d min=%d hour=%d day=%d tm_mon=%d tm_year=%d\n",rtc_rtm.tm_sec,rtc_rtm.tm_min,rtc_rtm.tm_hour,rtc_rtm.tm_mday,rtc_rtm.tm_mon+1,rtc_rtm.tm_year);

	ioctl(fd, RTC_ALM_READ, &rtc_alrgetrtm);
	printf(" app rtc_alrgetrtm sec=%d min=%d hour=%d day=%d tm_mon=%d tm_year=%d\n",rtc_alrgetrtm.tm_sec,rtc_alrgetrtm.tm_min,rtc_alrgetrtm.tm_hour,rtc_alrgetrtm.tm_mday,rtc_alrgetrtm.tm_mon+1,rtc_alrgetrtm.tm_year);
	sleep(1);
	}
	close(fd);
	return 0;
}
