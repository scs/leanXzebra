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

/*!@file leanXip.h
 * @Common tools for the leanXtogg application
 */
#ifndef H_LEANXIP
#define H_LEANXIP

#define MAX_CLI 10
#define SENDBUF (1024*512)
#define DATABUF (1024*512)
#define PORT 8111
#define SOCK_ERROR -1

int ip_start_server();
int ip_stop_server();
void ip_do_work();
int ip_send_all(char *buf, int len);
uint32 ip_sendtest();

#endif
