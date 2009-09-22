/*	leanXalgos.h
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

/*!@file leanXalgos.h
 * @Common tools and algorithms
 */
#ifndef H_LEANXALGOS
#define H_LEANXALGOS

struct ImgStats {
	unsigned char mean;
} ImgStats;

int fastdebayerBGR(const struct OSC_PICTURE pRaw, 
		   struct OSC_PICTURE *pOut, struct ImgStats *stats); 

int fastdebayerRGB(const struct OSC_PICTURE pRaw, 
		   struct OSC_PICTURE *pOut, struct ImgStats *stats); 

int fastdebayerYUV422(const struct OSC_PICTURE pRaw, 
		struct OSC_PICTURE *pOut, struct ImgStats *stats); 

int fastdebayerYUV444(const struct OSC_PICTURE pRaw, 
		struct OSC_PICTURE *pOut, struct ImgStats *stats); 

int fastdebayerChromU(const struct OSC_PICTURE pRaw, 
		struct OSC_PICTURE *pOut, struct ImgStats *stats); 

int fastdebayerChromV(const struct OSC_PICTURE pRaw, 
		struct OSC_PICTURE *pOut, struct ImgStats *stats); 

int fastgrey(   const struct OSC_PICTURE pRaw, 
		struct OSC_PICTURE *pOut, struct ImgStats *stats); 

#endif
