/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>*/
//#include <can_config.h>

#include <errno.h>
#include <getopt.h>
#include <libgen.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>

#include <net/if.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#define AF_xCAN 29

int main(void)
{
	int s = -1,ret = -1,nbytes = 0,i = 0;
	int family = AF_xCAN, type = SOCK_RAW, proto = CAN_RAW;
	unsigned long ID;
	unsigned int length,data[8];
	struct ifreq ifr;
	struct sockaddr_can addr;
	struct can_frame frame;
	unsigned char send_data[8] = {0x01,0x02,0x03,0x04,0x10,0x20,0x30,0x40};
	unsigned char send_data2[8] = {0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80};

while(1)
{
	if((s = socket(AF_xCAN,SOCK_RAW,CAN_RAW))<0)
	{
		perror("socket");
		return 1;	
	}
	strcpy(ifr.ifr_name, "can1" );	
	addr.can_family = family;
	if(ioctl(s, SIOCGIFINDEX, &ifr)) 
	{
		perror("ioctl");
		return 1;					
	}
	addr.can_ifindex = ifr.ifr_ifindex;
	
	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		return 1;					
	}
	
	ID = 0x00|CAN_EFF_FLAG; // ID data range 0x00000000 ~ 0x00ffffff
	length = 0x08;
	frame.can_id = ID | CAN_EFF_FLAG;
	frame.can_dlc = length;

	for(i = 0;i<length;i++)
	{
		//frame.data[i] = send_data[i];	
		frame.data[i] = send_data2[i];	
	}
	ret = write(s, &frame, sizeof(frame));
	if (ret == -1) {
		printf("can data send failed....disconnect\n");
		
	}	
	
	

		printf("ID = 0x%x  length = %d \n",ID,length);

		ret = write(s, &frame, sizeof(frame));
		if (ret == -1) {
			printf("can data send failed....disconnect\n");
		}	
		usleep(10000);	
	
	close(s);
}
	return 0;
}
