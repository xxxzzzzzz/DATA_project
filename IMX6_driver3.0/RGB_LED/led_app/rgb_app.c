#include <stdio.h>
#include <stdlib.h> 
#include <fcntl.h>
#include<sys/ioctl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define  LED_RED 0
#define  LED_GREEN 1
#define  LED_BLUE 2
#define  led_on       1
#define  led_off      0
/*-----------------------------RGB single function-------------*/
void RED(int level,int liumin);
void GREEN(int level,int liumin);
void BLUE(int level,int liumin);
/*-------------------RGB mixture function-------------*/
void RGBLED_R(int level,int liumin);
void RGBLED_G(int level,int liumin);
void RGBLED_B(int level,int liumin);
void RGBmixture(int level,int liumin);
/*-----------------------------RGB single debug-------------*/
/*./rgb_app r 1 60 ---- open red liumin for 60 ,liumin max 80*/
/*./rgb_app r 0 60 ---- close red liumin 60 invalid */

/*./rgb_app g 1 60 ---- open green liumin for 60 ,liumin max 80*/
/*./rgb_app g 0 60 ---- close green liumin 60 invalid */

/*./rgb_app b 1 60 ---- open blue liumin for 60 ,liumin max 80*/
/*./rgb_app b 0 60 ---- close blue liumin 60 invalid */



int main(int arc,char *argv[]){
	int fd;
	int i,liumin,level;
	char color;
	if(arc!=4){  //main r 1 10  
	printf("input erro \n");
	}
	else
	{
	level=atoi(argv[2]);
	liumin=atoi(argv[3]);

	if (strcmp(argv[1], "r") == 0)
	{
		printf("set RED level = %d liumin = %d\n",level,liumin);RED(level,liumin);
	}
	else if(strcmp(argv[1], "g") == 0)
	{
		printf("set GREEN level = %d liumin = %d\n",level,liumin);GREEN(level,liumin);
	}
	else if(strcmp(argv[1], "b") == 0)
	{
		printf("set BLUE level = %d liumin = %d\n",level,liumin);BLUE(level,liumin);
	}


	else if (strcmp(argv[1], "rr") == 0)
	{
		printf("set RGB_R level = %d liumin = %d\n",level,liumin);RGBLED_R(level,liumin);
	}


	else if (strcmp(argv[1], "gg") == 0)
	{
		printf("set RGB_G level = %d liumin = %d\n",level,liumin);RGBLED_G(level,liumin);
	}

	else if (strcmp(argv[1], "bb") == 0)
	{
		printf("set RGB_B level = %d liumin = %d\n",level,liumin);RGBLED_B(level,liumin);
	}

	else if (strcmp(argv[1], "q") == 0)
	{
		printf("set RGB_B level = %d liumin = %d\n",level,liumin);RGBmixture(level,liumin);
	}
	else
		printf("exit RGB pthread\n");

	}
	
	return 0;
}



void RED(int level,int liumin)
{
	int fd;	
	fd = open("/dev/RGB_led",O_RDWR|O_RDONLY, 0); //open 3 led
	if(fd<0){
		printf("RGB open failed!");	
	}
	if(level==led_on){
	ioctl(fd,led_on,LED_RED);      //select RED close GREEN BLUE
	ioctl(fd,led_off,LED_GREEN);      //select RED close GREEN BLUE
	ioctl(fd,led_off,LED_BLUE);      //select RED close GREEN BLUE

	}
	else
	{
		
	ioctl(fd,led_off,LED_RED);      //select RED close GREEN BLUE
	ioctl(fd,led_off,LED_GREEN);      //select RED close GREEN BLUE
	ioctl(fd,led_off,LED_BLUE);      //select RED close GREEN BLUE
	}

	write(fd, &liumin, sizeof(&liumin));

	close(fd);
}


void GREEN(int level,int liumin)
{
	
	int fd;	
	fd = open("/dev/RGB_led",O_RDWR|O_RDONLY, 0); //open 3 led
	if(fd<0){
		printf("RGB open failed!");	
	}
	if(level==led_on){
	ioctl(fd,led_off,LED_RED);      //select GREEN close RED BLUE
	ioctl(fd,led_on,LED_GREEN);      //select GREEN close RED BLUE
	ioctl(fd,led_off,LED_BLUE);      //select GREEN close RED BLUE

	}
	else
	{
		
	ioctl(fd,led_off,LED_RED);      //select GREEN close RED BLUE
	ioctl(fd,led_off,LED_GREEN);     //select GREEN close RED BLUE
	ioctl(fd,led_off,LED_BLUE);     //select GREEN close RED BLUE
	}

	write(fd, &liumin, sizeof(&liumin));

	close(fd);
}

void BLUE(int level,int liumin)
{
	
	int fd;	
	fd = open("/dev/RGB_led",O_RDWR|O_RDONLY, 0); //open 3 led
	if(fd<0){
		printf("RGB open failed!");	
	}
	if(level==led_on){
	ioctl(fd,led_off,LED_RED);      //select BLUE close GREEN RED
	ioctl(fd,led_off,LED_GREEN);    //select BLUE close GREEN RED
	ioctl(fd,led_on,LED_BLUE);      //select BLUE close GREEN RED

	}
	else
	{
		
	ioctl(fd,led_off,LED_RED);      //select BLUE close GREEN RED
	ioctl(fd,led_off,LED_GREEN);    //select BLUE close GREEN RED
	ioctl(fd,led_off,LED_BLUE);      //select BLUE close GREEN RED
	}

	write(fd, &liumin, sizeof(&liumin));

	close(fd);
}


void RGBLED_R(int level,int liumin)
{
	int fd;	
	fd = open("/dev/RGB_led",O_RDWR|O_RDONLY, 0); //open 3 led
	if(fd<0){
		printf("RGB open failed!");	
	}
	if(level==led_on){
	ioctl(fd,led_on,LED_RED);      //select BLUE close GREEN RED

	}
	else
	{
		
	ioctl(fd,led_off,LED_RED);      //select BLUE close GREEN RED
	}

	write(fd, &liumin, sizeof(&liumin));

	close(fd);
	
}
void RGBLED_G(int level,int liumin)
{
	int fd;	
	fd = open("/dev/RGB_led",O_RDWR|O_RDONLY, 0); //open 3 led
	if(fd<0){
		printf("RGB open failed!");	
	}
	if(level==led_on){
	ioctl(fd,led_on,LED_GREEN);      //select BLUE close GREEN RED

	}
	else
	{
		
	ioctl(fd,led_off,LED_GREEN);      //select BLUE close GREEN RED
	}

	write(fd, &liumin, sizeof(&liumin));

	close(fd);

}
void RGBLED_B(int level,int liumin)
{
	
	int fd;	
	fd = open("/dev/RGB_led",O_RDWR|O_RDONLY, 0); //open 3 led
	if(fd<0){
		printf("RGB open failed!");	
	}
	if(level==led_on){
	ioctl(fd,led_on,LED_BLUE);      //select BLUE close GREEN RED

	}
	else
	{
		
	ioctl(fd,led_off,LED_BLUE);      //select BLUE close GREEN RED
	}

	write(fd, &liumin, sizeof(&liumin));

	close(fd);
}



/*--------------------RGB mixture------------------*/

void RGBmixture(int level,int liumin)
{
	
	int fd;	
	fd = open("/dev/RGB_led",O_RDWR|O_RDONLY, 0); //open 3 led
	if(fd<0){
		printf("RGB open failed!");	
	}
	ioctl(fd,led_off,LED_GREEN);//RGB off
	ioctl(fd,led_off,LED_BLUE); 
	ioctl(fd,led_off,LED_RED);
	write(fd, &liumin, sizeof(&liumin));
	while(1){
	
	ioctl(fd,led_on,LED_RED);      
	sleep(1);
	ioctl(fd,led_off,LED_RED);      
	sleep(1);

	ioctl(fd,led_on,LED_GREEN);      
	sleep(1);
	ioctl(fd,led_off,LED_GREEN);      
	sleep(1);

	ioctl(fd,led_on,LED_BLUE);      
	sleep(1);
	ioctl(fd,led_off,LED_BLUE);      
	sleep(1);
///////////////////mixture///////////////////
	ioctl(fd,led_on,LED_RED);   //R G
	ioctl(fd,led_on,LED_GREEN); // R G
	ioctl(fd,led_off,LED_BLUE);
	sleep(1);
	ioctl(fd,led_off,LED_GREEN);//RGB off
	ioctl(fd,led_off,LED_BLUE); 
	ioctl(fd,led_off,LED_RED);
	sleep(1);
	
/*	ioctl(fd,led_on,LED_RED); //R B  ///////////////RED BLUE macth color failed/////////////////////
	ioctl(fd,led_on,LED_BLUE);// R B
	ioctl(fd,led_off,LED_GREEN);
	sleep(1);	
	ioctl(fd,led_off,LED_GREEN);//RGB off
	ioctl(fd,led_off,LED_BLUE); 
	ioctl(fd,led_off,LED_RED);
	sleep(1);*/
	ioctl(fd,led_on,LED_GREEN);//G B 
	ioctl(fd,led_on,LED_BLUE); //G B 
	ioctl(fd,led_off,LED_RED);
	sleep(1);
	ioctl(fd,led_off,LED_GREEN);//RGB off
	ioctl(fd,led_off,LED_BLUE); 
	ioctl(fd,led_off,LED_RED);
	sleep(1);
	
	ioctl(fd,led_on,LED_GREEN);//R G B
	ioctl(fd,led_on,LED_BLUE); //R G B 
	ioctl(fd,led_on,LED_RED);
	sleep(1);
	ioctl(fd,led_off,LED_GREEN);//RGB off
	ioctl(fd,led_off,LED_BLUE); 
	ioctl(fd,led_off,LED_RED);
	sleep(1);
	}
	close(fd);

}




