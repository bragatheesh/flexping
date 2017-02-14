/*
 *			P I N G . C
 *
 * Using the InterNet Control Message Protocol (ICMP) "ECHO" facility,
 * measure round-trip-delays and packet loss across network paths.
 *
 * Author -
 *	Mike Muuss
 *	U. S. Army Ballistic Research Laboratory
 *	December, 1983
 * Modified at Uc Berkeley
 * Record Route and verbose headers - Phil Dykstra, BRL, March 1988.
 *
 * Status -
 *	Public Domain.  Distribution Unlimited.
 *
 * Bugs -
 *	More statistics could always be gathered.
 *	This program has to run SUID to ROOT to access the ICMP socket.
 */

#include <stdio.h>
#include <errno.h>
#include <sys/time.h>

#include <sys/param.h>
#include <sys/socket.h>
#include <sys/file.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>

#define	PING_MAXWAIT	10	/* max time to wait for response, sec. */
#define	PING_MAXPACKET	4096	/* max packet size */
#define PING_NUMERIC	1	/* return dotted quads */
#define PING_VERBOSE	2	/* verbose flag */
#define PING_QUIET	4	/* quiet flag */
#define PING_DEBUG	8	/* turn on socket debugging */
#define PING_DONTROUTE	16	/* dont route pings*/
#define	PING_RROUTE	32	/* record route flag */
#define PING_FLOOD	64	/* floodping flag */
#define PING_CISCO	128	/* cisco style ping */
#define	PING_NROUTES	9	/* number of record route slots (9 max) */
#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN	64
#endif

u_char	packet[PING_MAXPACKET];
int	i, pingflags;
extern	int errno;

int s;			/* Socket file descriptor */
struct hostent *hp;	/* Pointer to host info */
struct timezone t