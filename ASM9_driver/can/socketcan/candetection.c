/*author xzz     
 *cansendrcvfilter sendID recive_filterID
 *cansendrcvfilter   10      10
 * */
#include <errno.h>
#include <getopt.h>
#include <libgen.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/time.h>
#include <stdint.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/select.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#define AF_xCAN 29

#define DEBUG

int main(int arc,char *argv[])
{
	int can_fd = -1,ret = -1,nbytes = 0,i = 0;
	int family = AF_xCAN, type = SOCK_RAW, proto = CAN_RAW;
	unsigned int txID = 0,rxID = 0,filterID = 0;
	unsigned int txlength = 8,rxlength = 0,data[8];
	struct ifreq ifr;
	struct sockaddr_can addr;
	struct can_frame txframe;
	struct can_frame rxframe;
	struct can_filter rxfilter;
	unsigned char send_data[8] = {0x01,0x02,0x03,0x04,0x10,0x20,0x30,0x40};
	unsigned char receive_data[8];
	fd_set fdset;	
	struct timeval tm;

	if(arc!=3)
	{
		printf("error\n");
		return -1;
	}
	txID = atoi(argv[1]);
	filterID = atoi(argv[2]);
#ifdef DEBUG
	printf("txID = %d\n",txID);
	printf("filterID = %d\n",filterID);
#endif

		if((can_fd = socket(AF_xCAN,SOCK_RAW,CAN_RAW))<0)
		{
			perror("socket");
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
	
		if (bind(can_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
			perror("bind");
			return 1;					
		}
		/*********************data send*******************/
		for(i = 0;i<txlength;i++)
		{
			txframe.data[i] = send_data[i];	//in the 8 data write txframe.data struct
		}
	
		txID = txID | CAN_EFF_FLAG;// ID data range 0x00000000 ~ 0x00ffffff
		txlength = 0x08; // send data length
		txframe.can_id = txID | CAN_EFF_FLAG;//in the ID write can_id struct
		txframe.can_dlc = txlength;//in the data length write can_dlc struct
	
		/************filter set******************/
		rxfilter.can_id = filterID;  //only receive filter ID addr data	
		rxfilter.can_mask = CAN_SFF_MASK;
		setsockopt(can_fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rxfilter, sizeof(rxfilter));
		/***********************************************/
		FD_ZERO(&fdset);
		FD_SET(can_fd,&fdset);
		while(1)
		{
			ret = write(can_fd, &txframe, sizeof(txframe));  //send can data
			if (ret == -1) {
				printf("can data send failed....disconnect\n");	
				break;
			}

			tm.tv_sec = 1;
			tm.tv_usec = 0;
			ret = select(can_fd+1,&fdset,NULL,NULL,&tm);
			if(ret < 0)
				printf("socket_can select error\n ");
			else if(ret == 0)
				printf("socket_can node detection \n");
			else
			{
				
			
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
			}
			usleep(10000);
		}
		
		close(can_fd);
		return 0;
}
