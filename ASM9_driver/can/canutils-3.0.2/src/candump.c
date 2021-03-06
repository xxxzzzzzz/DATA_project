#include <can_config.h>

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

extern int optind, opterr, optopt;

static int	s = -1;
static int	running = 1;

enum {
	VERSION_OPTION = CHAR_MAX + 1,
	FILTER_OPTION,
};


static void sigterm(int signo)
{
	running = 0;
}

static struct can_filter *filter = NULL;
static int filter_count = 0;

int add_filter(u_int32_t id, u_int32_t mask)
{
	filter = realloc(filter, sizeof(struct can_filter) * (filter_count + 1));
	if(!filter)
		return -1;

	filter[filter_count].can_id = id;
	filter[filter_count].can_mask = mask;
	filter_count++;

	printf("id: 0x%08x mask: 0x%08x\n",id,mask);
	return 0;
}

#define BUF_SIZ	(255)

//int main(int argc, char **argv)
int main(void)
{
	struct can_frame frame;
	struct ifreq ifr;
	struct sockaddr_can addr;
	FILE *out = stdout;
	char *interface = "can0";
	char *optout = NULL;
	char *ptr;
	char buf[BUF_SIZ];
	int family = AF_CAN, type = SOCK_RAW, proto = CAN_RAW;
	int n = 0, err;
	int nbytes, i;
	int opt, optdaemon = 0;
	uint32_t id, mask;

	signal(SIGPIPE, SIG_IGN);

         //socket(PF_CAN, SOCK_RAW, CAN_RAW); 
	//if ((s = socket(family, type, proto)) < 0) {
	if ((s =  socket(AF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("socket");
		return 1;
	}

	addr.can_family = family;
	strncpy(ifr.ifr_name, interface, sizeof(ifr.ifr_name));
	if (ioctl(s, SIOCGIFINDEX, &ifr)) {
		perror("ioctl");
		return 1;
	}
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		return 1;
	}

	while (running) {
		if ((nbytes = read(s, &frame, sizeof(struct can_frame))) < 0) {
			perror("read");
			return 1;
		} else {
			if (frame.can_id & CAN_EFF_FLAG)
				n = snprintf(buf, BUF_SIZ, "<0x%08x> ", frame.can_id & CAN_EFF_MASK);
			else
				n = snprintf(buf, BUF_SIZ, "<0x%03x> ", frame.can_id & CAN_SFF_MASK);

			n += snprintf(buf + n, BUF_SIZ - n, "[%d] ", frame.can_dlc);
			for (i = 0; i < frame.can_dlc; i++) {
				n += snprintf(buf + n, BUF_SIZ - n, "%02x ", frame.data[i]);
			}
			if (frame.can_id & CAN_RTR_FLAG)
				n += snprintf(buf + n, BUF_SIZ - n, "remote request");

			fprintf(out, "%s\n", buf);

			do {
				err = fflush(out);
				if (err == -1 && errno == EPIPE) {
					err = -EPIPE;
					fclose(out);
					out = fopen(optout, "a");
					if (!out)
						exit (EXIT_FAILURE);
				}
			} while (err == -EPIPE);

			n = 0;
		}
	}

	exit (EXIT_SUCCESS);
}
