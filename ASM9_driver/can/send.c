#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/errno.h>        /* error codes */

#include "asm9260.h"

int main(void)
{
	int i,count,fd;
	stcASM9260_BufInfo Buf;	

    //**********************
	Buf.FrID          = 0x12345678;		 //设置帧ID 0x12345678
	Buf.FrIf.Byte     = 0;	
	Buf.FrIf.Bits.DLC = 8;		         //设置帧数据长度为8字节
	Buf.FrIf.Bits.FF  = 1;		         //帧为扩展帧

	/* 帧数据设定为0x00 0x11 0x22 0x33 0x44 0x55 0x66 0x77 */
	for(i=0; i<Buf.FrIf.Bits.DLC; i++) {
		Buf.DA[i] = i<<4|i;
	}
	/*++++++++++++++++++++++++++++++++++++++++++++++*/
	Buf.DA[0] = 0xAA;
	Buf.DA[1] = 0xAA;
	Buf.DA[2] = 0xAA;
	Buf.DA[3] = 0xAA;
	Buf.DA[4] = 0xAA;
	Buf.DA[5] = 0xAA;
	Buf.DA[6] = 0xAA;
	Buf.DA[7] = 0xAA;
	/*++++++++++++++++++++++++++++++++++++++++++++++++++*/
	fd = open("/dev/ASM9260_CAN", O_RDWR);	//打开CAN设备
	if(fd == -1) {
		printf("Can't open ASM9260 Device!\n");
		close(fd);
		exit(-1);
	}

     //   unsigned long a=0x00066800; //500k
        unsigned long a=0x00137f00; //100k
        i = ioctl(fd, IOCTL_BAUD,&a);
   	if(i<0)	{
   		printf("Set Err!.\n");
   	} else	{
   		printf(" Baud set successful.\n");
   	}

	while(1) {
    	printf("CAN Send Frame:\n");
    	printf("\tCAN ID = 0x%08X\n", Buf.FrID);
    	printf("\tCAN Info = 0x%08X\n", Buf.FrIf.Byte);

        for(i=0; i<(Buf.FrIf.Bits.DLC); i++) {
    		printf("\tCAN Data[0x%02X] = 0x%02X.\n",i,Buf.DA[i]);
	    }
	    
        count = write(fd,(void*)(&Buf),sizeof(Buf));
	    sleep(2);
	    
            return 0;
	}

	close(fd);
	return 0;
}
