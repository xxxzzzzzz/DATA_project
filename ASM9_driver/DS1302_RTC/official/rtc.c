#include <stdio.h>
#include <stdlib.h>		/* getenv() */
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <linux/rtc.h>

#define VERSION "1.0"

struct rtc_cal{
	int value;
	int direction;
};

int Init()
{
	int fd;
	//open device file
	fd = open("/dev/rtc0",O_RDWR);
	if(fd < 0)
	{
		printf("device open fail\n");
		return -1;
	}

	return fd;
}

int main(int argc,char **argv)
{
	int fd,ch;
	struct rtc_time tm;
	struct rtc_wkalrm alm;
	struct rtc_wkalrm alm1;
	struct rtc_cal rtccal;
	int year,mon,mday,hour,min,sec,value,direction;
	int ayear,amon,amday,ahour,amin,asec;
	year = atoi(argv[1]);
	mon = atoi(argv[2]);
	mday = atoi(argv[3]);
	hour = atoi(argv[4]);
	min = atoi(argv[5]);
	sec = atoi(argv[6]);
	ayear = atoi(argv[7]);
	amon = atoi(argv[8]);
	amday = atoi(argv[9]);
	ahour = atoi(argv[10]);
	amin = atoi(argv[11]);
	asec = atoi(argv[12]);
	//value = atoi(argv[13]);
	//direction = atoi(argv[14]);
	fd=Init();
	if (fd<0)
		return -1;
	
	tm.tm_year = year-1900;
	tm.tm_mon = mon-1;
	tm.tm_mday = mday;
	tm.tm_wday = 2;
	tm.tm_yday = 90;
	tm.tm_hour = hour;
	tm.tm_min = min;
	tm.tm_sec = sec;
	ioctl(fd,RTC_SET_TIME,&tm);
	for(ch=0;ch<0x10000;ch++);
	ioctl(fd,RTC_RD_TIME,&tm);
	printf("RTC_RD_TIME %04d.%02d.%02d,%02d:%02d:%02d\n"
		,tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
	alm.time.tm_year = ayear-1900;
	alm.time.tm_mon = amon-1;
	alm.time.tm_mday = amday;
	alm.time.tm_yday = 90;
	alm.time.tm_wday = 2;
	alm.time.tm_hour = ahour;
	alm.time.tm_min = amin;
	alm.time.tm_sec = asec;
	ioctl(fd,RTC_ALM_SET,&alm);
	for(ch=0;ch<0x10000;ch++);
	ioctl(fd,RTC_ALM_READ,&alm1);
	printf("RTC_ALM_READ %04d.%02d.%02d,%02d:%02d:%02d\r\n"
			,alm1.time.tm_year+1900,alm1.time.tm_mon+1,alm1.time.tm_mday,
			alm1.time.tm_hour,alm1.time.tm_min,alm1.time.tm_sec);
	//rtccal.value = value;
	//rtccal.direction = direction;
	//ioctl(fd,RTC_PLL_SET,&rtccal);
	ioctl(fd,RTC_UIE_ON,NULL);
	ioctl(fd,RTC_AIE_ON,NULL);
	while(1){};
	
	close(fd);
	return 0;
}

