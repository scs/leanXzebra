/*	The Toeggeli-Application
	Copyright (C) 2008 Reto Bättig
	
	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation; either version 2.1 of the License, or (at
	your option) any later version.
	
	This program is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
	General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with this library; if not, write to the Free Software Foundation,
	Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*!@file leanXip.c
 * @The ip server for the leanXtogg application
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <sys/select.h>
#include "oscar.h"
#include "leanXtools.h"
#include "leanXip.h"

struct client {
	int sock;
	char *r_ptr; /* This client got all the data from wbuf up to this ptr */
};

struct  client clients[MAX_CLI];
struct  sockaddr_in addr;
int	srv_sock;
char	data[DATABUF];	

struct ringbuf wbuf;

int ip_start_server()
{
	int err;
	int i;

	srv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if (srv_sock==SOCK_ERROR) 
		fatalerror("Could not start IP server\n");

	i=1;
	setsockopt(srv_sock, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(int));
	i=1024*512;
	setsockopt(srv_sock, SOL_SOCKET, SO_SNDBUF, &i, sizeof(int));

	bzero(&addr, sizeof(addr));
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;

	err = bind(srv_sock, (struct sockaddr*)&addr, sizeof(addr));
	if (srv_sock==SOCK_ERROR) 
		fatalerror("Could bind socket\n");

	err = listen(srv_sock, MAX_CLI);

	ring_init(&wbuf, SENDBUF);

	for (i=0; i<MAX_CLI; i++)
		clients[i].sock = -1;

	return 0;
} /* ip_start_server */

int ip_stop_server()
{ 
	int i;
	for (i=0; i<MAX_CLI; i++) 
		if (clients[i].sock >0) 
			close(clients[i].sock);
	close(srv_sock);
	return 0;
} /* ip_stop_server */

/*
 * select_readable
 *
 * Checks whether the filedescriptor is readable
 * 
 * Return value: true, if readable 
 */ 
bool select_readable(int fd)
{ 	
	int retval;
	fd_set r;
	struct timeval timeout;

	bzero(&timeout, sizeof(timeout));
	FD_ZERO(&r); 

	FD_SET(fd, &r);
	retval = select(fd+1, &r, NULL, NULL, &timeout);

	if (retval == -1) 
		OscLog(ERROR, "Select Error\n");

	if ((retval>0) && FD_ISSET(fd, &r)) 
		return TRUE;

	return FALSE;
}

/*
 * select_writeable
 *
 * Checks whether the filedescriptor is writeable 
 * 
 * Return value: true, if readable 
 */ 
bool select_writeable(int fd)
{ 	
	int retval;
	fd_set w;
	struct timeval timeout;

	bzero(&timeout, sizeof(timeout));
	FD_ZERO(&w); 

	FD_SET(fd, &w);
	retval = select(fd+1, NULL, &w, NULL, &timeout);

	if (retval == -1) 
		OscLog(ERROR, "Select Error\n");

	if ((retval>0) && FD_ISSET(fd, &w)) 
		return TRUE;

	return FALSE;
}

void ip_cli_connect() 
{
	int i;

	for (i=0; i<MAX_CLI; i++) {
		if (clients[i].sock == -1)
			break;
	}
	if (i== MAX_CLI) {
		OscLog(INFO, "To many clients\n");
		return;
	}
	
	OscLog(DEBUG, "New client connects to IP server\n");
	clients[i].sock=accept(srv_sock, NULL, 0);
	if (clients[i].sock==SOCK_ERROR)
		fatalerror("accept failed\n");
	clients[i].r_ptr = wbuf.r_ptr;
}

void ip_cli_disconnect(int client) 
{
	clients[client].sock = -1;
}

void ip_read(int client)
{
	int err;
	char dummy[100];
	
	err=read(clients[client].sock, &dummy, sizeof(dummy)-1);
	if (err==0) {
		ip_cli_disconnect(client);
		OscLog(DEBUG, "Client disconnected\n");
	}
	if (err>0) {
		dummy[err]=0;
		printf(dummy);
	}
}

int ip_send_all(char *buf, int len)
{
	if (ring_free(&wbuf) > len) 
		return ring_write(&wbuf, buf, len);
	else 
		return 0;
}

void fix_readpointer() {
	char *minptr;
	char *rptr;
	bool hasclient = FALSE;
	int i;

	minptr = wbuf.w_ptr;

	for (i=0; i<MAX_CLI; i++) if (clients[i].sock>0 ) {
		hasclient = TRUE;
		rptr = clients[i].r_ptr;
		if (rptr > wbuf.w_ptr)
			rptr -= wbuf.size;
		minptr = min(minptr, rptr);
	}

	if (minptr < wbuf.data)
		minptr+=wbuf.size;
	if (hasclient) wbuf.r_ptr = minptr;
}
	
void ip_do_work()
{
	int i, len;

	if (select_readable(srv_sock)) {
		ip_cli_connect();
	}
	
	for (i=0; i<MAX_CLI; i++) if (clients[i].sock>0 ) {

		while ((clients[i].r_ptr != wbuf.w_ptr) && select_writeable(clients[i].sock)){
			len = ring_peekfrom(&wbuf, clients[i].r_ptr, data, DATABUF);
			len = send(clients[i].sock, data, len, 0);
			if (len>0)
				ring_addtoptr(&wbuf, &(clients[i].r_ptr), len);
		}
		while ((clients[i].sock>0) && select_readable(clients[i].sock)) {
			ip_read(i);
		} 
	}

	fix_readpointer();
	
}


uint32 ip_sendtest() 
{
	uint32 len=0;
	int retval;
	int i;

	for (i=0; i<MAX_CLI; i++) { 
		
		if (clients[i].sock>0) {
			retval=send(clients[i].sock, data, DATABUF, 0);	
			if (retval>0)
				len+=retval;
		}

	}
	return len;
}


void ip_test() {
	int i=0;
	char s[100];
	while (1) {
		ip_do_work();
		sleep(1);
		sprintf(s, "i=%i\n", i);
		i=i+1;
		ip_send_all(s, strlen(s)+1);
	}
}

