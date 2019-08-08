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
	int s = -1,nbytes = 0,i = 0;
	int family = AF_xCAN, type = SOCK_RAW, proto = CAN_RAW;
	unsigned long ID;
	unsigned int length,data[8];
	struct ifreq ifr;
	struct sockaddr_can addr;
	struct can_frame frame;
	struct can_filter rxfilter;

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
	/************filter set******************/
	rxfilter.can_id = 0xffff;  //only receive 0xffff ID addr data
	rxfilter.can_mask = CAN_SFF_MASK;
	setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rxfilter, sizeof(rxfilter));
	/***************************************/
	while(1)
	{
		if ((nbytes = read(s, &frame, sizeof(struct can_frame))) < 0) {
			perror("read");
			return -1;										
		} 
		ID = frame.can_id & CAN_EFF_MASK;
		length = frame.can_dlc;
		for(i = 0;i<length;i++)
		{
			data[i] = frame.data[i];
		}
		printf("ID = 0x%x  length = %d \n",ID,length);
		printf("data[0] = %x data[1] = %x data[2] = %x data[3] = %x data[4] = %x data[5] = %x data[6] = %x data[7] = %x\n" ,
				data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
		
	}
	close(s);
	return 0;
}
