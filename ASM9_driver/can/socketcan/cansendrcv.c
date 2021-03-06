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

int main()
{
	int s = -1,ret = -1,nbytes = 0,i = 0;
	int family = AF_xCAN, type = SOCK_RAW, proto = CAN_RAW;
	unsigned long txID = 0,rxID = 0;
	unsigned int txlength = 0,rxlength = 0,data[8];
	struct ifreq ifr;
	struct sockaddr_can addr;
	struct can_frame txframe;
	struct can_frame rxframe;
	unsigned char send_data[8] = {0x01,0x02,0x03,0x04,0x10,0x20,0x30,0x40};
	unsigned char receive_data[8];
	
	
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
	txID = 0x00000001|CAN_EFF_FLAG; // ID data range 0x00000000 ~ 0x00ffffff
	txlength = 0x08;
	txframe.can_id = txID | CAN_EFF_FLAG;
	txframe.can_dlc = txlength;
	for(i = 0;i<txlength;i++)
	{
		txframe.data[i] = send_data[i];	
	}
	while(1)
	{
		ret = write(s, &txframe, sizeof(txframe));
		if (ret == -1) {
			printf("can data send failed....disconnect\n");	
		}
		/*else 
			printf("can write success\n");*/
/*------------------------------------------------------------------------------------*/
		if ((nbytes = read(s, &rxframe, sizeof(rxframe))) < 0) {
			perror("read");
			return -1;										
		} 
		/*else
			printf("can read success\n");*/
		rxID = rxframe.can_id & CAN_EFF_MASK;
		rxlength = rxframe.can_dlc;
		for(i = 0;i<rxlength;i++)
		{
			receive_data[i] = rxframe.data[i];
		}
		printf("rxID = 0x%x rxlength = %d\n",rxID,rxlength);
		printf("data[0] = %x data[1] = %x data[2] = %x data[3] = %x data[4] = %x data[5] = %x data[6] = %x data[7] = %x\n" ,receive_data[0],receive_data[1],receive_data[2],receive_data[3],receive_data[4],receive_data[5],receive_data[6],receive_data[7]); 
	
	usleep(5000);
	}
	close(s);
	return 0;	
}
