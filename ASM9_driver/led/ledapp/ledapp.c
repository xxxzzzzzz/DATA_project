#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
int main(void)
{
	while(1)
	{
		system("echo 0 >/sys/Cled/led1");
		system("echo 0 >/sys/Cled/led2");
		system("echo 0 >/sys/Cled/led3");
		sleep(1);
		system("echo 1 >/sys/Cled/led1");
		system("echo 1 >/sys/Cled/led2");
		system("echo 1 >/sys/Cled/led3");
		sleep(1);
	}
	return 0;
}
