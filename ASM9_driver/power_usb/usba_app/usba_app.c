#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
int main(void)
{
	while(1)
	{
		system("echo 0 >/sys/Cusba_POWER/usba_POWER");
		sleep(10);
		system("echo 1 >/sys/Cusba_POWER/usba_POWER");
		sleep(10);
	}
	return 0;
}
