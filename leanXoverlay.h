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

/*!@file leanXoverlay.h
 * @Overlay text to image
 */
#ifndef H_LEANXOVERLAY
#define H_LEANXOVERLAY

static unsigned char ov_numbers[10][60] = {
{ 0, 1, 1, 1, 0, 0,
  1, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  0, 1, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0},

{ 0, 0, 1, 0, 0, 0,
  1, 1, 1, 0, 0, 0,
  0, 0, 1, 0, 0, 0,
  0, 0, 1, 0, 0, 0,
  0, 0, 1, 0, 0, 0,
  0, 0, 1, 0, 0, 0,
  0, 0, 1, 0, 0, 0,
  0, 0, 1, 0, 0, 0,
  0, 0, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0 },

{ 0, 1, 1, 1, 0, 0,
  1, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 1, 0,
  0, 0, 0, 1, 0, 0,
  0, 0, 1, 0, 0, 0,
  0, 1, 0, 0, 0, 0,
  1, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 1, 0,
  0, 0, 0, 0, 0, 0 },

{ 0, 1, 1, 1, 0, 0,
  1, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 1, 0,
  0, 1, 1, 1, 0, 0,
  0, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  0, 1, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0 },

{ 0, 0, 0, 1, 0, 0,
  0, 0, 1, 1, 0, 0,
  0, 0, 1, 1, 0, 0,
  0, 1, 0, 1, 0, 0,
  0, 1, 0, 1, 0, 0,
  1, 0, 0, 1, 0, 0,
  1, 1, 1, 1, 1, 0,
  0, 0, 0, 1, 0, 0,
  0, 0, 0, 1, 0, 0,
  0, 0, 0, 0, 0, 0 },

{ 1, 1, 1, 1, 1, 0,
  1, 0, 0, 0, 0, 0,
  1, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 0, 0,
  1, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  0, 1, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0 },

{ 0, 1, 1, 1, 0, 0,
  1, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 0, 0,
  1, 0, 0, 0, 0, 0,
  1, 1, 1, 1, 0, 0,
  1, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  0, 1, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0 },

{ 1, 1, 1, 1, 1, 0,
  0, 0, 0, 0, 1, 0,
  0, 0, 0, 1, 0, 0,
  0, 0, 0, 1, 0, 0,
  0, 0, 1, 0, 0, 0,
  0, 0, 1, 0, 0, 0,
  0, 1, 0, 0, 0, 0,
  0, 1, 0, 0, 0, 0,
  0, 1, 0, 0, 0 ,0,
  0, 0, 0, 0, 0, 0},

{ 0, 1, 1, 1, 0, 0,
  1, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  0, 1, 1, 1, 0, 0,
  1, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  0, 1, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0 },

{ 0, 1, 1, 1, 0, 0,
  1, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  0, 1, 1, 1, 1, 0,
  0, 0, 0, 0, 1, 0,
  0, 0, 0, 0, 1, 0,
  1, 0, 0, 0, 1, 0,
  0, 1, 1, 1, 0, 0,
  0, 0, 0, 0, 0, 0 }
};

void overlay(struct OSC_PICTURE *pPic, int x, int y, int size, char *text);
#endif
