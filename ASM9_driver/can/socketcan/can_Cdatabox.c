/*author xzz     
 *cancan_fdendrcvfilter sendIDnumber 
 *cancan_fdendrcvfilter   10      
 so can for 10 nede
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

//#define DEBUG
#define readID
#define RGBS
int CAN_detection_check(unsigned int IDnumber);//connect check

int main(int arc,char *argv[])
{
	int can_fd = -1,ret = -1,nbytes = 0;
	int family = AF_xCAN, type = SOCK_RAW, proto = CAN_RAW;
	unsigned int txID = 0,rxID = 0,filterID = 0;
	unsigned int txlength = 8,rxlength = 0,data[8];
	unsigned int IDnum = 0;	
	unsigned int weightdata = 0,buff = 0,chipID = 0;
	unsigned int chipID1 = 0,chipID2 = 0,chipID3 = 0;
	struct ifreq ifr;
	struct sockaddr_can addr;
	struct can_frame txframe;
	struct can_frame rxframe;
	struct can_filter rxfilter;
	unsigned char i=0,chipIDaddr=0,lumin=0,RGB=0;
	unsigned char send_data[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char receive_data[8];
	if(arc!=2)
	{
		printf("error\n");
		return -1;
	}
	txID = atoi(argv[1]);

#ifdef DEBUG
	printf("txID = %d\n",txID);
	printf("filterID = %d\n",filterID);
#endif
		ret = CAN_detection_check(txID);
		if(ret < 0)
		{
			printf("CAN_detection_check func failed\n");
			return -1;
		}
while(1){
		for(IDnum = 1;IDnum < txID+1;IDnum++)
		{

/*********************************************read STM32 chip ID***************************************************************/			
			chipID1=0;
			chipID2=0;
			chipID3=0;
#ifdef readID
			for(chipIDaddr=0;chipIDaddr<3;chipIDaddr++)
			{
				send_data[7]=0x01;//Byte7 commnd
				send_data[6]=chipIDaddr;//Byte6 STM32 chip ID addr
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
				txframe.can_id = IDnum | CAN_EFF_FLAG;//in the ID write can_id can_fdtruct
				txframe.can_dlc = txlength;//in the data length write can_dlc can_fdtruct
	
				/************filter can_fdet******************/
				rxfilter.can_id = IDnum;  //only receive filter ID addr data	
				rxfilter.can_mask = CAN_SFF_MASK;
				setsockopt(can_fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rxfilter, sizeof(rxfilter));
				/***********************************************/
		

				ret = write(can_fd, &txframe, sizeof(txframe));  //send can data
				if (ret == -1) {
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
#ifdef DEBUG
				printf("ID = 0x%x  length = %d \n",rxID,rxlength);
				printf("data[0] = %x data[1] = %x data[2] = %x data[3] = %x data[4] = %x data[5] = %x  data[6] = %x data[7] = %x\n" ,receive_data[0],receive_data[1],receive_data[2],receive_data[3],receive_data[4],receive_data[5],receive_data[6],receive_data[7]);
#endif
				if(receive_data[7]==0x01&&receive_data[4] == rxID)
				{
					if(receive_data[6] == 0x00)
					{
						buff = (unsigned int)(receive_data[3]);
						chipID1 |= (buff << 24);
						buff = (unsigned int)(receive_data[2]);
						chipID1 |= (buff << 16);
						buff = (unsigned int)(receive_data[1]);
						chipID1 |= (buff << 8);
						buff = (unsigned int)(receive_data[0]);
						chipID1 |= buff;
#ifdef DEBUG
						printf("chipID1 = %x\n",chipID1);
#endif
					}
					else if(receive_data[6] == 0x01)
					{
						buff = (unsigned int)(receive_data[3]);
						chipID2 |= (buff << 24);
						buff = (unsigned int)(receive_data[2]);
						chipID2 |= (buff << 16);
						buff = (unsigned int)(receive_data[1]);
						chipID2 |= (buff << 8);
						buff = (unsigned int)(receive_data[0]);
						chipID2 |= buff;
#ifdef DEBUG
						printf("chipID2 = %x\n",chipID2);
#endif
					}
					else if(receive_data[6] == 0x02)
					{
						buff = (unsigned int)(receive_data[3]);
						chipID3 |= (buff << 24);
						buff = (unsigned int)(receive_data[2]);
						chipID3 |= (buff << 16);
						buff = (unsigned int)(receive_data[1]);
						chipID3 |= (buff << 8);
						buff = (unsigned int)(receive_data[0]);
						chipID3 |= buff;
#ifdef DEBUG
						printf("chipID3 = %x\n",chipID3);
#endif
					}
					else
					{
						printf("STM32 chip read failed...\n");
					}

				}	
				close(can_fd);
			}
			printf("stm32ID = %x%x%x\n",chipID1,chipID2,chipID3);//chipID1--chipID2--chipID3
#endif
/******************************************************read node weight****************************************************************/	
			for(i=0;i<8;i++)
			{
				send_data[i]=0;	
			}
			send_data[7]=0x02;//Byte7 commnd read weight
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
			txframe.can_id = IDnum | CAN_EFF_FLAG;//in the ID write can_id can_fdtruct
			txframe.can_dlc = txlength;//in the data length write can_dlc can_fdtruct
	
			/************filter can_fdet******************/
			
			rxfilter.can_id = IDnum;  //only receive filter ID addr data	
			rxfilter.can_mask = CAN_SFF_MASK;
			setsockopt(can_fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rxfilter, sizeof(rxfilter));
			/***********************************************/
		

			ret = write(can_fd, &txframe, sizeof(txframe));  //send can data
			if (ret == -1) {
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
#ifdef DEBUG
			printf("ID = 0x%x  length = %d \n",rxID,rxlength);
			printf("weight can frame data[0] = %x data[1] = %x data[2] = %x data[3] = %x data[4] = %x data[5] = %x  data[6] = %x data[7] = %x\n" ,receive_data[0],receive_data[1],receive_data[2],receive_data[3],receive_data[4],receive_data[5],receive_data[6],receive_data[7]);
#endif
			if(receive_data[7]==0x02&&receive_data[4] == rxID)
			{
				weightdata =0;
				buff =(unsigned int)(receive_data[3]);
				weightdata |= (buff << 24);
				buff = (unsigned int)(receive_data[2]);
				weightdata |= (buff << 16);
				buff = (unsigned int)(receive_data[1]);
				weightdata |= (buff << 8);
				buff = (unsigned int)(receive_data[0]);
				weightdata |= buff;
#ifdef DEBUG
				printf("data[3] = %x data[2] = %x data[1] = %x data[0] = %x\n",receive_data[3],receive_data[2],receive_data[1],receive_data[0]);
#endif
			}
			else
			{
				printf("read STM32 weight failed..\n");
			}
			close(can_fd);
			printf("Can ID = %d length = %d weightdata = %d\n",rxID,rxlength,weightdata);
/***************************************************can control RGB***********************************************************************************/
#ifdef RGBS
			/*RGB liumin value 0x10~0xa0  ...... min=0x10 max=0xa0*/
			for(RGB=0;RGB<3;RGB++)
			{
				for(lumin=0x00;lumin<0xb0;lumin=lumin+0x10)
				{
					for(i=0;i<8;i++)
					{
						send_data[i]=0;	
					}
					send_data[7]=0x03;//Byte7 commnd control RGB
					send_data[RGB]=lumin;//Byte0 RGB BLUE luminance
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
					txframe.can_id = IDnum | CAN_EFF_FLAG;//in the ID write can_id can_fdtruct
					txframe.can_dlc = txlength;//in the data length write can_dlc can_fdtruct
	
					/************filter can_fdet******************/
			
					rxfilter.can_id = IDnum;  //only receive filter ID addr data	
					rxfilter.can_mask = CAN_SFF_MASK;
					setsockopt(can_fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rxfilter, sizeof(rxfilter));
					/***********************************************/
		

					ret = write(can_fd, &txframe, sizeof(txframe));  //send can data
					if (ret == -1) {
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
					if(receive_data[7]==0x03&&receive_data[4] == rxID)
					{
						printf("RGB luminance value R = %x  G = %x B =%x\n",receive_data[2],receive_data[1],receive_data[0]);
					}
					else
					{
						printf("control RGB failed..\n");
					}
					close(can_fd);
					usleep(100000);

				}
			}
#endif
/**********************************************************************************************************************************************/			
			usleep(500000);	
		}
	}
		return 0;
}

/*connect check*/

int CAN_detection_check(unsigned int IDnumber)
{
	int can_fd = -1,ret = -1,nbytes = 0,i = 0;
	int family = AF_xCAN, type = SOCK_RAW, proto = CAN_RAW;
	unsigned  int txID = 0,rxID = 0,filterID = 0;
	unsigned  int txlength = 8,rxlength = 0,data[8];
	struct ifreq ifr;
	struct sockaddr_can addr;
	struct can_frame txframe;
	struct can_frame rxframe;
	struct can_filter rxfilter;
	unsigned char send_data[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02};
	fd_set fdset;	
	struct timeval tm;
	unsigned int IDnum = 0;
	for(IDnum = 1;IDnum<IDnumber+1;IDnum++)
	{

		if((can_fd = socket(AF_xCAN,SOCK_RAW,CAN_RAW))<0)
		{
			perror("socket");
			return -1;	
		}
		strcpy(ifr.ifr_name, "can1" );	
		addr.can_family = family;
		if(ioctl(can_fd, SIOCGIFINDEX, &ifr)) 
		{
			perror("ioctl");
			return -1;					
		}
		addr.can_ifindex = ifr.ifr_ifindex;
		if (bind(can_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
		{
			perror("bind");
			return -1;					
		}
		/*********************data send*******************/
		for(i = 0;i<txlength;i++)
		{
			txframe.data[i] = send_data[i];	//in the 8 data write txframe.data struct
		}
	
		txlength = 0x08; // send data length
		txframe.can_id = IDnum | CAN_EFF_FLAG;//in the ID write can_id struct
		txframe.can_dlc = txlength;//in the data length write can_dlc struct
	
		/************filter set******************/
		rxfilter.can_id = IDnum;  //only receive filter ID addr data	
		rxfilter.can_mask = CAN_SFF_MASK;
		setsockopt(can_fd, SOL_CAN_RAW, CAN_RAW_FILTER, &rxfilter, sizeof(rxfilter));
		/***********************************************/
		FD_ZERO(&fdset);
		FD_SET(can_fd,&fdset);
		
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
		{
			printf("socket_can node detection CAN ID = %d failed \n",IDnum);
			return -1;
			
		}	
		else
		{
				
			if ((nbytes = read(can_fd, &rxframe, sizeof(rxframe))) < 0)  //recvie can data
			{
					perror("read");
					return -1;
			}
			printf("CAN ID = %d connect success \n",IDnum);
		}
		close(can_fd);
	}		
	
}
