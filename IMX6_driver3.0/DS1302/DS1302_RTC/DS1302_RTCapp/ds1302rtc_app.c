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
rtc_tm.tm_sec =55;
rtc_tm.tm_min=59;
rtc_tm.tm_hour=23;
rtc_tm.tm_mday=31;
mon=5;
rtc_tm.tm_mon=(mon);
rtc_tm.tm_year=2015;
fd=open("/dev/rtc1",O_RDWR);
if(fd<0){

	printf("open rtc1 erro\n");
}

ioctl(fd, RTC_SET_TIME, &rtc_tm);
close(fd);

fd=open("/dev/rtc1",O_RDWR);
if(fd<0){

	printf("open rtc1 erro\n");
}
while(1){
/*ioctl(fd, RTC_RD_TIME, &rtc_rtm);
printf(" app read sec=%d min=%d hour=%d mday=%d tm_mon=%d tm_year=%d\n",rtc_rtm.tm_sec,rtc_rtm.tm_min,rtc_rtm.tm_hour,rtc_rtm.tm_mday,rtc_rtm.tm_mon,rtc_rtm.tm_year);
*/
/*ret=ioctl(fd, RTC_SET_TIME, &rtc_tm); 
if(ret==-1){
	printf("RTC SET_TIME erro\n");

}*/

ioctl(fd, RTC_RD_TIME, &rtc_rtm);

printf(" app read sec=%d min=%d hour=%d day=%d tm_mon=%d tm_year=%d\n",rtc_rtm.tm_sec,rtc_rtm.tm_min,rtc_rtm.tm_hour,rtc_rtm.tm_mday,rtc_rtm.tm_mon+1,rtc_rtm.tm_year);

sleep(1);
}
close(fd);
return 0;
}
