
/*author: xiangzizhou ,versions: wbed1.0 */
/*RTC code chip ds1302*/
#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include<sys/ioctl.h>
#include <unistd.h>
#include<string.h>
#include <sys/time.h>
#include <linux/rtc.h>

void RTC_parameter(struct rtc_time *rtc_tm);
void only_read_RTC();

int main(int arc,char *argv[]){
int fd,chanshu,ret;
int argv_year,argv_mon,argv_day,argv_hours,argv_min,argv_sec;
struct rtc_time rtc_tm;

if(arc!=8){
chanshu=0;	
}
else
{
switch(atoi(argv[7]))
{
  case 1: only_read_RTC();break;
  case 2 :
  argv_sec=atoi(argv[1]);
  argv_min=atoi(argv[2]);
  argv_hours=atoi(argv[3]);
  argv_day=atoi(argv[4]);
  argv_mon=atoi(argv[5]);
  argv_year=atoi(argv[6]);
  rtc_tm.tm_sec =argv_sec;
  rtc_tm.tm_min=argv_min;
  rtc_tm.tm_hour=argv_hours;
  rtc_tm.tm_mday=argv_day;
  rtc_tm.tm_mon=argv_mon;
  rtc_tm.tm_year=argv_year;
  RTC_parameter(&rtc_tm);
  break;

default:
  break;

}
}


/*application please parameter as follow code*/

////////////////////////////////**///////////////////////////////////////////
rtc_tm.tm_sec=55;
rtc_tm.tm_min=59;
rtc_tm.tm_hour=23;
rtc_tm.tm_mday=31;
rtc_tm.tm_mon=12;
rtc_tm.tm_year=2016;
fd=open("/dev/ds1302_rtc", O_RDWR|O_RDONLY,0); //open ds1302 
if(fd<0){
	printf("ds1302 open failed!");	
	}

if(ret=write(fd, &rtc_tm, sizeof(rtc_tm))){

  	printf("ds1302 ioctl RTC_SET_TIME erro\n");
	}


while(1){

	ret=read(fd, &rtc_tm, sizeof(rtc_tm));

	printf(" app read sec=%d min=%d hour=%d day=%d tm_mon=%d tm_year=%d\n",rtc_tm.tm_sec,rtc_tm.tm_min,rtc_tm.tm_hour,rtc_tm.tm_mday,rtc_tm.tm_mon,rtc_tm.tm_year);
	sleep(1);

}

colse(fd);
return 0;
}

////////////////////////////////////////////////////****///////////////////////////



void RTC_parameter(struct rtc_time *rtc_tm){
int fd,ret;
fd=open("/dev/ds1302_rtc", O_RDWR|O_RDONLY,0); //open ds1302 
if(fd<0){
	printf("ds1302 open failed!");	
	}

if(ret=write(fd, rtc_tm, sizeof(rtc_tm))){

        printf("ds1302 ioctl RTC_SET_TIME erro\n");
 	}

while(1){
 	ret=read(fd, rtc_tm, sizeof(rtc_tm));

 	printf(" app read sec=%d min=%d hour=%d day=%d tm_mon=%d tm_year=%d\n",rtc_tm->tm_sec,rtc_tm->tm_min,rtc_tm->tm_hour,rtc_tm->tm_mday,rtc_tm->tm_mon,rtc_tm->tm_year);
 	sleep(1);
  	}

}



void only_read_RTC(){
int fd,ret;
struct rtc_time rtc_tm;

fd=open("/dev/ds1302_rtc", O_RDWR|O_RDONLY,0); //open ds1302 

if(fd<0){
	printf("ds1302 open failed!");	
	}

while(1){
	
 	ret=read(fd, &rtc_tm, sizeof(rtc_tm)); 
	printf(" app read sec=%d min=%d hour=%d day=%d tm_mon=%d tm_year=%d\n",rtc_tm.tm_sec,rtc_tm.tm_min,rtc_tm.tm_hour,rtc_tm.tm_mday,rtc_tm.tm_mon,rtc_tm.tm_year);
 	sleep(1); 
}

}
