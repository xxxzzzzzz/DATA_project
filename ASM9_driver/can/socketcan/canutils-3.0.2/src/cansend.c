#include <can_config.h>

#include <getopt.h>
#include <libgen.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include <net/if.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>

#include <linux/can.h>
#include <linux/can/raw.h>

extern int optind, opterr, optopt;


enum {
		VERSION_OPTION = CHAR_MAX + 1,
};

#define DLC 8
//int main(int argc, char **argv)
int main(void)
{
	struct can_frame frame = {
		.can_id = 1,
	};
	struct ifreq ifr;
	struct sockaddr_can addr;
	char *interface;
	int family = AF_CAN, type = SOCK_RAW, proto = CAN_RAW;
	int loopcount = 1, infinite = 0;
	int s, opt, ret, i, dlc = 0, rtr = 0, extended = 0;
	int verbose = 0;	

       interface ="can1";


	//s = socket(family, type, proto);
       s  = socket(AF_CAN, SOCK_RAW, CAN_RAW);  
	if (s < 0) {
		perror("socket");
		return 1;
	}

	addr.can_family = family;
	strcpy(ifr.ifr_name, interface);
	if (ioctl(s, SIOCGIFINDEX, &ifr)) {
		perror("ioctl");
		return 1;
	}
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		return 1;
	}

	for (i =0; i < DLC; i++) {
		//frame.data[dlc] = strtoul(argv[i], NULL, 0);
		frame.data[dlc] = i;
		dlc++;
		if (dlc == 8)
			break;
	}
	frame.can_dlc = dlc;


	if (extended) {
		frame.can_id &= CAN_EFF_MASK;
		frame.can_id |= CAN_EFF_FLAG;
	} else {
		frame.can_id &= CAN_SFF_MASK;
	}

	if (rtr)
		frame.can_id |= CAN_RTR_FLAG;

	if (verbose) {
		printf("id: %d ", frame.can_id);
		printf("dlc: %d\n", frame.can_dlc);
		for (i = 0; i < frame.can_dlc; i++)
			printf("0x%02x ", frame.data[i]);
		printf("\n");
	}

	while (infinite || loopcount--) {
		ret = write(s, &frame, sizeof(frame));
		if (ret == -1) {
			perror("write");
			break;
		}
	}

	close(s);
	return 0;
}
