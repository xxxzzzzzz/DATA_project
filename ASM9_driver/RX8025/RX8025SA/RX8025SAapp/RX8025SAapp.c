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
	struct rtc_time rtc_settm;
	struct rtc_time rtc_gettm;
	struct rtc_time rtc_alm;

	rtc_settm.tm_sec =30;
	rtc_settm.tm_min=10;
	rtc_settm.tm_hour=10;
	rtc_settm.tm_mday=10;
	mon=5;
	rtc_settm.tm_mon=(mon);
	rtc_settm.tm_year=2015;
	fd=open("/dev/rtc0",O_RDWR);
	if(fd<0){

		printf("open rtc0 error\n");
	}

	ioctl(fd, RTC_SET_TIME, &rtc_settm);
	close(fd);

	/*set time alm*/
	rtc_alm.tm_min=11;
	rtc_alm.tm_hour=10;

	fd=open("/dev/rtc0",O_RDWR);
	if(fd<0){

		printf("open rtc0 error\n");
	}

	ioctl(fd, RTC_ALM_SET, &rtc_alm);
	close(fd);


	fd=open("/dev/rtc0",O_RDWR);
	if(fd<0){

		printf("open rtc0 error\n");
	}
	while(1){
	ioctl(fd, RTC_RD_TIME, &rtc_gettm);

	printf(" app read sec=%d min=%d hour=%d day=%d tm_mon=%d tm_year=%d\n",rtc_gettm.tm_sec,rtc_gettm.tm_min,rtc_gettm.tm_hour,rtc_gettm.tm_mday,rtc_gettm.tm_mon+1,rtc_gettm.tm_year);

	sleep(3);
	}
	close(fd);
	return 0;
}
