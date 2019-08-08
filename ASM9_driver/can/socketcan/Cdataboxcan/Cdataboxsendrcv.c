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
#include <sys/select.h>
#include <linux/can/raw.h>
#define AF_xCAN 29


int main(int arc,char *argv[])
{
	int can_fd = -1,ret = -1,nbytes = 0,i = 0;
	int family = AF_xCAN, type = SOCK_RAW, proto = CAN_RAW;
	unsigned int txID = 0,rxID = 0,filterID = 0;
	unsigned int txlength = 8,rxlength = 0,data[8];
	unsigned int IDnum = 0;	
	unsigned int weightdata = 0,buff = 0,chipID = 0;
	struct ifreq ifr;
	struct sockaddr_can addr;
	struct can_frame txframe;
	struct can_frame rxframe;
	struct can_filter rxfilter;
	unsigned char send_data[8] = {0x01,0x01,0x01,0x01,0xff,0xff,0xff,0xff};
	unsigned char receive_data[8];
	if(arc!=2)
	{
		printf("error\n");
		return -1;
	}
	txID = atoi(argv[1]);
	printf("txID = %d\n",txID);
	printf("filterID = %d\n",filterID);		
	


while(1){
		
	if((can_fd = socket(AF_xCAN,SOCK_RAW,CAN_RAW))<0)
	{
		perror("can_fdocket");
		return 1;	
	}
	strcpy(ifr.ifr_name, "can1" );	
	addr.can_family = family;
	if(ioctl(can_fd, SIOCGIFINDEX, &ifr)) 
	{
		perror("ioctl");
		return 1;					
	}
	addr.can_ifindex = ifr.ifr_ifindex;
	
	if (bind(can_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
	{
		perror("bind");
		return 1;					
	}
	/*********************data can_fdend*******************/
	for(i = 0;i<txlength;i++)
	{
		txframe.data[i] = send_data[i];	//in the 8 data write txframe.data struct
	}
	txlength = 0x08; // can_fdend data length
	txframe.can_id = txID | CAN_EFF_FLAG;//in the ID write can_id can_fdtruct
	txframe.can_dlc = txlength;//in the data length write can_dlc can_fdtruct
	
	/************filter can_fdet******************/
	rxfilter.can_id = txID;  //only receive filter ID addr data	
	rxfilter.can_mask = CAN_SFF_MASK;
	setsockopt(can_fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rxfilter, sizeof(rxfilter));
	/***********************************************/
		

	ret = write(can_fd, &txframe, sizeof(txframe));  //send can data
	if (ret == -1) 
	{
		printf("can data can_fdend failed....disconnect\n");	
		break;
	}

	if ((nbytes = read(can_fd, &rxframe, sizeof(rxframe))) < 0)  //recvie can data
	{
		perror("read");
		return -1;
	}
	rxID = rxframe.can_id & CAN_EFF_MASK;
	rxlength = rxframe.can_dlc;
	for(i = 0;i<rxlength;i++)
	{
		receive_data[i] = rxframe.data[i];
	}
	printf("ID = 0x%x  length = %d \n",rxID,rxlength);
	printf("data[0] = %x data[1] = %x data[2] = %x data[3] = %x data[4] = %x data[5] = %x  data[6] = %x data[7] = %x\n" ,receive_data[0],receive_data[1],receive_data[2],receive_data[3],receive_data[4],receive_data[5],receive_data[6],receive_data[7]);
	usleep(500000);	
	close(can_fd);

	}
	return 0;

}
	

