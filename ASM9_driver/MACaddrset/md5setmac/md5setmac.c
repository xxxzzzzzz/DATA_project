#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string.h>
#include <net/if_arp.h>
#include <fcntl.h>
#include <unistd.h>
#include "md5.h"

int StringToHex(char *str, unsigned char *out, unsigned int *outlen)
{
	char *p = str;
	char high = 0, low = 0;
	int tmplen = strlen(p), cnt = 0;
	tmplen = strlen(p);
	while(cnt < (tmplen / 2))
	{
	high = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
	low = (*(++ p) > '9' && ((*p <= 'F') || (*p <= 'f'))) ? *(p) - 48 - 7 : *(p) - 48;
	out[cnt] = ((high & 0x0f) << 4 | (low & 0x0f));
	p ++;
	cnt ++;
	}
	if(tmplen % 2 != 0) out[cnt] = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
	if(outlen != NULL) *outlen = tmplen / 2 + tmplen % 2;
	return tmplen / 2 + tmplen % 2;
}


int main(void)
{
	int IDfile = 0,i = 0,fd = 0;
	unsigned char IDbuffer[30];
	unsigned char decrypt[16]={0};
	ssize_t ret;
	struct ifreq ifr; 
	char mac[16]={0};
	MD5_CTX md5;
	MD5Init(&md5);
	
	IDfile = open("/mnt/yaffs2/Cdatabox/ID",O_RDONLY);
	if( IDfile < 0 )
	{
		printf("IDfile open failed\n");
	}
	ret = read(IDfile, IDbuffer,sizeof(IDbuffer));
	printf("read ID number = %ld\n",ret);	
	close(IDfile);

	MD5Update(&md5,IDbuffer,strlen((char *)(IDbuffer)));
	MD5Final(&md5,decrypt);

	for(i = 0;i <16 ; i++)
	{
		printf("%x",decrypt[i]);
	}
	printf("\n");
	/*write md5 byte4 ~ byte9 to mac*/	
	mac[0] = (decrypt[4]&0xfc);
	mac[1] = decrypt[5];
	mac[2] = decrypt[6];
	mac[3] = decrypt[7];
	mac[4] = decrypt[8];
	mac[5] = decrypt[9];
	
	printf("MAC setting addr = ");
	for(i =0 ;i<6;i++)
	{
		printf("%x:",mac[i]);
	}
	printf("\n");
	fd = socket(AF_INET, SOCK_DGRAM, 0 );
	if ( fd < 0  )
	{
		
		perror("socket");
		return -1;
					        
	}
	ifr.ifr_addr.sa_family = ARPHRD_ETHER;
	strcpy(ifr.ifr_name, "eth0");

	for(i = 0;i < 6;i++)
	{
		ifr.ifr_hwaddr.sa_data[i] = mac[i];
	}
				    
	if( (ret = ioctl(fd, SIOCSIFHWADDR, &ifr) ) != 0  )
	{
		perror("SIOCSIFHWADDR");
	}
	close(fd);
	
	return 0;
}
