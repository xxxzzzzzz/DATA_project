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


unsigned int read_weight(unsigned int txID);
void CAN_node_perssure(unsigned int txID);//CAN node1
void CAN_node4all_perssure(float CAN_ID1FL,float  CAN_ID2FL,float  CAN_ID3FL,float  CAN_ID4FL);

int main(int arc,char *argv[])
{
	unsigned int CAN_ID = 0;
	float CANID1FL = 1,CANID2FL = 1,CANID3FL = 1,CANID4LF = 1;
	
	if(arc<3)
	{
		while(1)
		{
			CAN_ID = atoi(argv[1]);
			printf("txID = %d\n",CAN_ID);
			printf("input pressure CAN node = %d success\n",CAN_ID);
			CAN_node_perssure(CAN_ID);
		}
	}
	else if((arc>2) && (arc<6))
	{
		
		CANID1FL = atof(argv[1]);
		CANID2FL = atof(argv[2]);
		CANID3FL = atof(argv[3]);
		CANID4LF = atof(argv[4]);
		CAN_node4all_perssure(CANID1FL,CANID2FL,CANID3FL,CANID4LF);
	}
	else
	{
		printf("error\n");
		return -1;	
	}


while(1)
{
	
	
}
	return 0;
}

unsigned int read_weight(unsigned int txID)
{
	int can_fd = -1,ret = -1,nbytes = 0,i = 0;
	int family = AF_xCAN, type = SOCK_RAW, proto = CAN_RAW;
	unsigned int rxID = 0,filterID = 0;
	unsigned int txlength = 8,rxlength = 0;
	unsigned int weight = 0,data = 0;	

	struct ifreq ifr;
	struct sockaddr_can addr;
	struct can_frame txframe;
	struct can_frame rxframe;
	struct can_filter rxfilter;
	unsigned char send_data[8] = {0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char receive_data[8];
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
		return -1;
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
	close(can_fd);

	data = (unsigned int)receive_data[7];
	weight = data<<24;
	data = 0;
	data = (unsigned int)receive_data[6];
	weight = weight | data<<16;
	data = 0;
	data = (unsigned int)receive_data[5];
	weight = weight | data<<8;
	data = 0;
	data = (unsigned int)receive_data[4];
	weight = weight | data;
	return weight;
}

void CAN_node_perssure(unsigned int txID)
{
	unsigned int weight1 = 0,yuanshidata = 0;
	yuanshidata = read_weight(txID);
	
	while(1)
	{
		weight1 = read_weight(txID);
		printf("weight1 = %d\n",(weight1-yuanshidata));
		usleep(500000);
	}	
}




void CAN_node4all_perssure(float CAN_ID1FL,float  CAN_ID2FL,float  CAN_ID3FL,float  CAN_ID4FL)
{
	unsigned int weight1 = 0,weight2 = 0,weight3 = 0,weight4 = 0;
	unsigned int yuanshidata1 = 0,yuanshidata2 = 0,yuanshidata3 = 0,yuanshidata4 = 0;
	unsigned int result1 = 0,result2 = 0,result3 = 0,result4 = 0;
	
	printf("CAN_ID1FL = %f CAN_ID2FL = %f CAN_ID3FL = %f CAN_ID4FL = %f\n",CAN_ID1FL,CAN_ID2FL,CAN_ID3FL,CAN_ID4FL);
	yuanshidata1 = read_weight(1);
	yuanshidata2 = read_weight(2);
	yuanshidata3 = read_weight(3);
	yuanshidata4 = read_weight(4);
	
	while(1)
	{
		weight1 = read_weight(1);
		weight2 = read_weight(2);
		weight3 = read_weight(3);
		weight4 = read_weight(4);
		
		result1 = weight1 - yuanshidata1;
		result2 = weight2 - yuanshidata2;
		result3 = weight3 - yuanshidata3;
		result4 = weight4 - yuanshidata4;
		printf("weight1 = %d weight2 = %d weight3 = %d weight4 = %d weightzong = %f\n",result1,result2,result3,result4,
				((float)(result1*CAN_ID1FL)+(float)(result2*CAN_ID2FL)+(float)(result3*CAN_ID3FL)+(float)(result4*CAN_ID4FL)));
		usleep(500000);
	}	
}
