#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>


#define I2C_RDWR 0x0707 

struct i2c_msg
{
	unsigned short addr;
	unsigned short flags;
	#define I2C_M_TEN 0x0010
	#define I2C_M_RD 0x0001
	unsigned short len;
	unsigned char *buf;
};


struct i2c_rdwr_ioctl_data
{
	struct i2c_msg *msgs;
	int nmsgs; 
};

int main(void)
{
	int fd = -1, ret = -1;
	struct i2c_rdwr_ioctl_data e2prom_data;
	unsigned char buffer=10;

	fd = open("/dev/i2c-1",O_RDWR);
	if(fd<0)
		printf("open i2c error\n");

	e2prom_data.nmsgs=2; 
	e2prom_data.msgs=(struct i2c_msg*)malloc(e2prom_data.nmsgs*sizeof(struct i2c_msg));
	if(!e2prom_data.msgs)
	{
		perror("malloc error 1");
		exit(1);   
	}
	
	e2prom_data.nmsgs=1;
	(e2prom_data.msgs[0]).len=1;
	(e2prom_data.msgs[0]).addr=0x29;
	(e2prom_data.msgs[0]).flags=0; //write
	(e2prom_data.msgs[0]).buf=&buffer;

	ret=ioctl(fd,I2C_RDWR,(unsigned long)&e2prom_data);
	if(ret<0)
	{
		perror("ioctl error1");
	}
	else
		printf("ioctl success\n");

	close(fd);


	return 0;
}
