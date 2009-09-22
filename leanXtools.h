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

/*!@file leanXtools.h
 * @Common tools for the leanXtogg application
 */
#ifndef H_LEANXTOOLS
#define H_LEANXTOOLS

#define min(x1,x2) ((x1) > (x2))? (x2):(x1)
#define max(x1,x2) ((x1) > (x2))? (x1):(x2)

struct list {
	struct list *next; /* Has to be on top of the structures */
};

struct ringbuf {
	char *data;
	char *r_ptr;
	char *w_ptr;
	int  size;
};

void list_ins(struct list **head, struct list *item); 
void list_del(struct list **head, struct list *item); 
	
int ring_free(struct ringbuf *buf); 
void ring_init(struct ringbuf *buf, int len);
int ring_write(struct ringbuf *buf, char *data, int len); 
int ring_peek(struct ringbuf *buf, char *data, int maxlen);
int ring_peekfrom(struct ringbuf *buf, char *r_ptr, char *data, int maxlen);
int ring_read(struct ringbuf *buf, char *data, int maxlen);
void ring_addtoptr(struct ringbuf *buf, char **ptr, unsigned int len);
void ring_subfromptr(struct ringbuf *buf, char **ptr, unsigned int len);

void fatalerror(char *strFormat, ...);

void dump_buffer(unsigned char *data, int len);
void print_checksum(unsigned char *data, int len, char *string);
#endif
