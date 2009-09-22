/*	
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

/*!@file leanXoverlay.c
 * @Write text on top of an image
 */

#include <string.h>
#include "oscar.h"
#include "leanXoverlay.h"

void blob(struct OSC_PICTURE *pPic, int x, int y, int size, unsigned char color) 
{
	int a, b;
	unsigned char *data = pPic->data;
	for (a=0; a<size; a++) {
		for (b=0; b<size; b++) {
			data[x+b+(y+a)*pPic->width] = color;
		}
	}
}

void overlay(struct OSC_PICTURE *pPic, int x, int y, int size, char *text) 
{
	int i;
	int a,b;
	unsigned char num;
	for (i=0; text[i]!=0; i++) {
		num = text[i] - '0';
		if (num>10)
			continue;
		for (a=0; a<6; a++) {
			for (b=0; b<10; b++) {
				blob(pPic, x+a*size, y+b*size, size, ov_numbers[num][a+b*6]*255);
			}
		}
		x = x + 6*size;
	}
}
