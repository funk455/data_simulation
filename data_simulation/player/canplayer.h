#ifndef _CANPLAYER_H_
#define _CANPLAYER_H_

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

//#include <linux/can.h>
//#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include "can.h"
#include "raw.h"
#include "lib.h"
#include<getopt.h>

#define DEFAULT_GAP	1	/* ms */
#define DEFAULT_LOOPS	1	/* only one replay */
#define CHANNELS	20	/* anyone using more than 20 CAN interfaces at a time? */
#define COMMENTSZ 200
#define BUFSZ (sizeof("(1345212884.318850)") + IFNAMSIZ + 4 + CL_CFSZ + COMMENTSZ) /* for one line in the logfile */
#define STDOUTIDX	65536	/* interface index for printing on stdout - bigger than max uint16 */

struct assignment {
	char txif[IFNAMSIZ];
	int  txifidx;
	char rxif[IFNAMSIZ];
};
static struct assignment asgn[CHANNELS];
const int canfd_on = 1;


int canplayer_main(int argc, char *argv[]);
extern int optind, opterr, optopt;
#endif