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

    //************************
	fd = open("/dev/ASM9260_CAN", O_RDWR);	//打开CAN设备
	if(fd == -1) {
		printf("Can't open ASM9260 Device!\n");
		close(fd);
		exit(-1);
	}


   unsigned long a=0x00077f03;
   i = ioctl(fd, IOCTL_BAUD,&a);
   if(i<0)	{
           printf("Set Err!.\n");
   } else	{
           printf(" Baud set successful.\n");
   }

    while(1) {

		count = read(fd,(void*)(&Buf), sizeof(Buf));
        printf("count = %d\n", count);

		if(count < 0) {
			printf("read err!.\n");
		} else {
			printf("CAN Recive %d Bytes.\n", count);
			printf("CAN ID = 0x%08X.\n", Buf.FrID);
			printf("CAN Info = 0x%08X.\n", Buf.FrIf.Byte);
			for(i=0; i<(Buf.FrIf.Bits.DLC); i++) {
				printf("CAN Data[0x%02X] = 0x%02X.\n",i,Buf.DA[i]);
			}
		}
	}

	close(fd);
	return 0;
}
