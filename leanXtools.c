/*	The Toeggeli-Application
	Copyright (C) 2008 Reto Bättig
	
	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation; either version 2.1 of the License, or (at
	>your option) any later version.
	
	This program is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
	General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with this library; if not, write to the Free Software Foundation,
	Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*!@file leanXtools.c
 * @Common tools for the leanXtogg application
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include "oscar.h"
#include "leanXtools.h"

struct listtest {
	struct list l; /* Has to be on top of the structure */
	int i;
};

int ring_datalen(struct ringbuf *buf) 
{
	int full;
	full = buf->w_ptr - buf->r_ptr;
	if (buf->r_ptr>buf->w_ptr) {
		full += buf->size;
	}
	return full;
}

int ring_free(struct ringbuf *buf) 
{
	return buf->size - ring_datalen(buf)-1;
}

void ring_debug(struct ringbuf *buf)
{
	char str[100];
	sprintf(str, "size=%i, r_ptr=%i, w_ptr=%i\n", buf->size, (int)(buf->r_ptr-buf->data), (int)(buf->w_ptr-buf->data));
	OscLog(DEBUG, str);
	memcpy(str, buf->data, min(99,buf->size));
	str[min(99, buf->size)]=0;
	OscLog(DEBUG, str);
	OscLog(DEBUG, "\n");
}

void ring_addtoptr(struct ringbuf *buf, char **ptr, unsigned int len) 
{
	*ptr += len;
	if (*ptr >= buf->data+buf->size) {
		*ptr -= buf->size;
	}
}

void ring_subfromptr(struct ringbuf *buf, char **ptr, unsigned int len)
{
	*ptr -= len;
	if (*ptr < buf->data) {
		*ptr += buf->size;
	}
}

void ring_init(struct ringbuf *buf, int len)
{
	buf->data  = malloc(len);
	buf->size  = len;
	buf->r_ptr = buf->data;
	buf->w_ptr   = buf->data;
}

int ring_write(struct ringbuf *buf, char *data, int len) 
{
	int part;

	len = min(len, ring_free(buf));	
	part = min(len, buf->data+buf->size-buf->w_ptr);
	memcpy(buf->w_ptr, data, part);
	if (part < len) {
		memcpy(buf->data, data+part, len-part);
	}
	ring_addtoptr(buf, &(buf->w_ptr), len);
	ring_debug(buf);
	return len;
}

int ring_peek(struct ringbuf *buf, char *data, int maxlen)
{
	int part;
	maxlen = min(maxlen, ring_datalen(buf));	
	part = min(maxlen, buf->data+buf->size-buf->r_ptr);
	memcpy(data, buf->r_ptr, part);
	if (part < maxlen)
		memcpy(data+part, buf->data, maxlen-part);
	return maxlen;
}

int ring_read(struct ringbuf *buf, char *data, int maxlen)
{
	int retval;
	retval = ring_peek(buf, data, maxlen);
	ring_addtoptr(buf, &(buf->r_ptr), retval);
	ring_debug(buf);
	return retval;
}

/* ring_peekfrom
 * 
 * Reads from the ringbuffer as if the read pointer were at position *r_ptr.
 * Sets the *r_ptr to the position as it would be after the read
 * The original ringbuffer is left unchanged! Also the original buf->r_ptr!
 */
int ring_peekfrom(struct ringbuf *buf, char *r_ptr, char *data, int maxlen)
{
	char *r_ptr_orig = buf->r_ptr; /* Save original readpointer */
	int retval;


	buf->r_ptr = r_ptr;
	retval = ring_read(buf, data, maxlen);

	buf->r_ptr=r_ptr_orig; /* restore original readpointer */

	return retval;
}

void ring_test() {
	int retval;
	char str[100];
	struct ringbuf buf;
	int i;

	printf("\ninit\n"); 
	ring_init(&buf, 10);
	ring_debug(&buf);

	for (i=0; i<10; i++) {

		printf("\nwrite(\"123\")\n"); 
		retval=ring_write(&buf, "123", 3);
		printf("retval was %i\n", retval);

		printf("\npeek(2)\n"); 
		retval=ring_peek(&buf, str, 2);
		printf("retval was %i\n", retval);
		str[retval]=0;
		printf(str);printf("\n");	

		printf("\npeek(4)\n"); 
		retval=ring_peek(&buf, str, 4);
		printf("retval was %i\n", retval);
		str[retval]=0;
		printf(str);printf("\n");	

		printf("\nwrite(\"4567890\")\n"); 
		retval=ring_write(&buf, "4567890", 7);
		printf("retval was %i\n", retval);

		printf("\nwrite(\"overflow\")\n"); 
		retval=ring_write(&buf, "owerflow", 8);
		printf("retval was %i\n", retval);

		printf("\npeek(20)\n"); 
		retval=ring_peek(&buf, str, 20);
		printf("retval was %i\n", retval);
		str[retval]=0;
		printf(str);printf("\n");	

		printf("\nread(5)\n"); 
		retval=ring_read(&buf, str, 5);
		printf("retval was %i\n", retval);
		str[retval]=0;
		printf(str);printf("\n");	
		ring_debug(&buf);

		printf("\nread(2)\n"); 
		retval=ring_read(&buf, str, 2);
		printf("retval was %i\n", retval);
		str[retval]=0;
		printf(str);printf("\n");	
		ring_debug(&buf);

		printf("\nread(2)\n"); 
		retval=ring_read(&buf, str, 2);
		printf("retval was %i\n", retval);
		str[retval]=0;
		printf(str);printf("\n");	
		ring_debug(&buf);
		
		printf("\nread(1)\n"); 
		retval=ring_read(&buf, str, 1);
		printf("retval was %i\n", retval);
		str[retval]=0;
		printf(str);printf("\n");	
		ring_debug(&buf);

		printf("\nwrite(\"abcdefg\")\n"); 
		retval=ring_write(&buf, "abcdefg", 7);
		printf("retval was %i\n", retval);

		printf("\nread(3)\n"); 
		retval=ring_read(&buf, str, 3);
		printf("retval was %i\n", retval);
		str[retval]=0;
		printf(str);printf("\n");	
		ring_debug(&buf);

		printf("\nwrite(\"hijklmn\")\n"); 
		retval=ring_write(&buf, "hijklmn", 7);
		printf("retval was %i\n", retval);

		printf("\nread(12)\n"); 
		retval=ring_read(&buf, str, 12);
		printf("retval was %i\n", retval);
		str[retval]=0;
		printf(str);printf("\n");	
		ring_debug(&buf);
	}
}

void list_ins(struct list **head, struct list *item) {
	item->next = *head;
	*head=item;
}

void list_del(struct list **head, struct list *item) {
	struct list *i = *head;
	if (i == item) {
		*head=i->next;
		return;
	}
	while (i != NULL) {
		if (i->next == item) {
			i->next = i->next->next;
			return;
		}
		i = i->next;
	}	
}

void list_test() 
{
 /*
	struct list *head=NULL;
	struct listtest *item;
	int i;

	for (i=0; i<3; i++) {
		item = malloc(sizeof(struct listtest));
		item->i = i;
		list_ins(&head, (struct list *)item);
	}

	item = head;
	while (item != NULL) {
		printf("Item %i\n", ((struct listtest *)item)->i);
		item = ((struct list *)item)->next;
	}


	printf("\nDeleting first item\n");
	item = head;
	list_del(&head, (struct list *)item);
	free(item);
	item = head;
	while (item != NULL) {
		printf("Item %i\n", ((struct listtest *)item)->i);
		item = ((struct list *)item)->next;
	}


	printf("\nDeleting last item\n");
	item = head->next;
	list_del(&head, (struct list *)item);
	free(item);
	item = head;
	while (item != NULL) {
		printf("Item %i\n", ((struct listtest *)item)->i);
		item = ((struct list *)item)->next;
	}

	printf("\nDeleting last item\n");
	item = head;
	list_del(&head, (struct list *)item);
	free(item);
	item = head;
	while (item != NULL) {
		printf("Item %i\n", ((struct listtest *)item)->i);
		item = ((struct list *)item)->next;
	}

	printf("\nAdding 3 Items\n");
	for (i=0; i<3; i++) {
		item = malloc(sizeof(struct listtest));
		item->i = i;
		list_ins(&head, (struct list *)item);
	}

	item = head;
	while (item != NULL) {
		printf("Item %i\n", ((struct listtest *)item)->i);
		item = ((struct list *)item)->next;
	}
*/
} /* list_test */

void fatalerror(char *strFormat, ...)
{
	va_list ap; /*< The dynamic argument list */
	va_start(ap, strFormat);
	vprintf(strFormat, ap);
	va_end(ap);
	_exit(1);
} /* fatalerror */

void OscLog2(const enum EnOscLogLevel level, char *strFormat, ...)
{
	if (level <= DEBUG) { 
		va_list ap; /*< The dynamic argument list */
		va_start(ap, strFormat);
		vprintf(strFormat, ap);
		va_end(ap);
	}
} /* fatalerror */

void print_checksum(unsigned char *data, int len, char *string)
{
	int k, h;
	h=0;
	for (k=0; k<len; k++) 
		h+=*(data+k);
	printf(string, h);
}

void dump_buffer(unsigned char *data, int len)
{
	int k, h;
	h=0;
	for (k=0; k<len; k++) {
		printf("%02x ", *(data+k));
		if (k % 16 == 15)
			printf("\n");
	}
	
	if (k % 32 != 0)
		printf("\n");
}


