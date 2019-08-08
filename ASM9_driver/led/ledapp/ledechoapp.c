#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>

int main(void)
{
	FILE *fp;
	char buffer[3];
while(1)
{
	fp = popen("echo 0 >/sys/Cled/led1","w");//set led1 status 0 for the light 1 for the black
	pclose(fp);
	fp = popen("cat /sys/Cled/led1","r");//read led1 status 
	fgets(buffer,sizeof(buffer),fp);  //led1 status storage buffer
	printf("led1 status = %s",buffer);//out led1 staus 
	pclose(fp);

	fp = popen("echo 0 >/sys/Cled/led2","w");
	pclose(fp);
	fp = popen("cat /sys/Cled/led2","r");
	fgets(buffer,sizeof(buffer),fp);
	printf("led2 status = %s",buffer);
	pclose(fp);

	fp = popen("echo 0 >/sys/Cled/led3","w");
	pclose(fp);
	fp = popen("cat /sys/Cled/led3","r");
	fgets(buffer,sizeof(buffer),fp);
	printf("led3 status = %s",buffer);
	pclose(fp);

	sleep(1);

	fp = popen("echo 1 >/sys/Cled/led1","w");
	pclose(fp);
	fp = popen("cat /sys/Cled/led1","r");
	fgets(buffer,sizeof(buffer),fp);
	printf("led1 status = %s",buffer);
	pclose(fp);
		
	fp = popen("echo 1 >/sys/Cled/led2","w");
	pclose(fp);
	fp = popen("cat /sys/Cled/led2","r");
	fgets(buffer,sizeof(buffer),fp);
	printf("led2 status = %s",buffer);
	pclose(fp);

	fp = popen("echo 1 >/sys/Cled/led3","w");
	pclose(fp);
	fp = popen("cat /sys/Cled/led3","r");
	fgets(buffer,sizeof(buffer),fp);
	printf("led3 status = %s",buffer);
	pclose(fp);

	sleep(1);
}

	return 0;
}

