#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string.h>
#include<net/if_arp.h>

int main(void)
{
	int fd,ret;
	struct ifreq ifr;
	int i;
	char mac[6]={0};
	mac[0] = 0x00;
	mac[1] = 0x00;
	mac[2] = 0x00;
	mac[3] = 0x11;
	mac[4] = 0x11;
	mac[5] = 0x11;
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
