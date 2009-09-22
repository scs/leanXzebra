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

/*!@file leanXalgos.c
 * @Common tools for the leanXtogg application
 */

#include "oscar.h"
#include "leanXalgos.h"

/* fastdebayerBGR
 * Very simple debayering. Makes one colour pixel out of 4 bayered pixels
 * This means that the resulting image is only width/2 by height/2 pixels
 * Image size is reduced by a factor of 4!
 * This needs about 6ms for a full 750x480 frame on leanXcam
 * And returns the image in BGR24 Format
 */
int fastdebayerBGR(const struct OSC_PICTURE pRaw, 
		struct OSC_PICTURE *pOut, struct ImgStats *stats) 
{
	uint16 x,y;
	uint32 outPos=0;
	unsigned char *in  = (unsigned char *)pRaw.data;
	unsigned char *out = (unsigned char *)pOut->data;
	uint32 mean=0;
	uint32 rowmean;
	uint16 R1, G1, B1;

	for (y=0; y<pRaw.height; y+=2) {
		rowmean=0;
		for (x=0; x<pRaw.width; x+=2) {
			R1 = (unsigned char)in[(y+1)*pRaw.width+x+1];
			G1 = (unsigned char)in[y*pRaw.width+x+1];
			B1 = (unsigned char)in[y*pRaw.width+x];
			/* Blue */
			out[outPos++]=B1;
			/* Green */
			out[outPos++]=G1;
			/* Red */
			out[outPos++]=R1;
			rowmean = rowmean + R1 +G1 +B1;
		} /* for x */
		mean = mean + (rowmean / (pRaw.width/2) / 3);
	} /* for y */
	pOut->width  = pRaw.width/2;
	pOut->height = pRaw.height/2; 
	pOut->type  = OSC_PICTURE_BGR_24;

	if (stats != 0) {
		stats->mean = mean / (pRaw.height / 2);
	}
	return 0;
} /* fastdebayer */

/* fastdebayerRGB
 * Very simple debayering. Makes one colour pixel out of 4 bayered pixels
 * This means that the resulting image is only width/2 by height/2 pixels
 * Image size is reduced by a factor of 4!
 * This needs about 6ms for a full 750x480 frame on leanXcam
 * And returns the image in RGB24 Format
 */
int fastdebayerRGB(const struct OSC_PICTURE pRaw, 
		struct OSC_PICTURE *pOut, struct ImgStats *stats) 
{
	uint16 x,y;
	uint32 outPos=0;
	unsigned char *in  = (unsigned char *)pRaw.data;
	unsigned char *out = (unsigned char *)pOut->data;
	uint32 mean=0;
	uint32 rowmean;
	uint16 R1, G1, B1;

	for (y=0; y<pRaw.height; y+=2) {
		rowmean=0;
		for (x=0; x<pRaw.width; x+=2) {
			R1 = (unsigned char)in[(y+1)*pRaw.width+x+1];
			G1 = (unsigned char)in[y*pRaw.width+x+1];
			B1 = (unsigned char)in[y*pRaw.width+x];
			/* Red */
			out[outPos++]=R1;
			/* Green */
			out[outPos++]=G1;
			/* Blue */
			out[outPos++]=B1;
			rowmean = rowmean + R1 +G1 +B1;
		} /* for x */
		mean = mean + (rowmean / (pRaw.width/2) / 3);
	} /* for y */
	pOut->width  = pRaw.width/2;
	pOut->height = pRaw.height/2; 
	pOut->type  = OSC_PICTURE_RGB_24;
	if (stats != 0) {
		stats->mean = mean / (pRaw.height / 2);
	}
	return 0;
} /* fastdebayer */

/* fastdebayerYUV444
 * Very simple debayering. Makes one colour pixel out of 4 bayered pixels
 * This means that the resulting image is only width/2 by height/2 pixels
 * Image size is reduced by a factor of 4!
 * And returns the image in YUV444 Format
 * The macro pixel is stored in YUV order 
 */
int fastdebayerYUV444(const struct OSC_PICTURE pRaw, 
		struct OSC_PICTURE *pOut, struct ImgStats *stats) 
{
	uint16 x,y;
	uint32 outPos=0;
	int16 pY1, pU1, pV1;
	int16 R1, G1, B1;
	unsigned char *in  = (unsigned char *)pRaw.data;
	unsigned char *out = (unsigned char *)pOut->data;
	uint32 rowmean;
	uint32 mean=0;

	for (y=0; y<pRaw.height; y+=2) {
		rowmean=0;
		for (x=0; x<pRaw.width; x+=2) {
			R1 = in[(y+1)*pRaw.width+x+1];
			G1 = in[y*pRaw.width+x+1];
			B1 = in[y*pRaw.width+x];
			/* Fixed-Point: Faktoren * 128 */
			/* Y = 0.299*R+0.587*G+0.114*B) */

			pY1 = ((38*R1 + 75*G1 + 15*B1) >> 7);	
			pU1 = (((B1 - pY1)*63)>>7)+128;
			pV1 = (((R1 - pY1)*112)>>7)+128;

			rowmean = rowmean + pY1;

			out[outPos++]=pY1;
			out[outPos++]=pU1;
			out[outPos++]=pV1;
		} /* for x */
		mean = mean + rowmean / (pRaw.width / 2);
	} /* for y */
	pOut->width  = pRaw.width/2;
	pOut->height = pRaw.height/2; 
	pOut->type  = OSC_PICTURE_YUV_444;
	if (stats != 0)
		stats->mean = mean / pOut->height;
	return 0;
} /* fastdebayer */


/* fastdebayerYUV422
 * Very simple debayering. Makes one colour pixel out of 4 bayered pixels
 * This means that the resulting image is only width/2 by height/2 pixels
 * Image size is reduced by a factor of 4!
 * And returns the image in YUV422 Format
 * The macro pixel is stored in UYVY order (equal to Y422 and UYNV and HDYC 
 * according to www.fourcc.org. The fourcc hexcode is 0x59565955
 */
int fastdebayerYUV422(const struct OSC_PICTURE pRaw, 
		struct OSC_PICTURE *pOut, struct ImgStats *stats) 
{
	uint16 x,y;
	uint32 outPos=0;
	int16 pY1, pY2, pU1, pV1;
	int16 R1, G1, B1, R2, G2, B2;
	unsigned char *in  = (unsigned char *)pRaw.data;
	unsigned char *out = (unsigned char *)pOut->data;
	uint32 rowmean;
	uint32 mean=0;

	for (y=0; y<pRaw.height; y+=2) {
		rowmean=0;
		for (x=0; x<pRaw.width; x+=4) {
			R1 = in[(y+1)*pRaw.width+x+1];
			G1 = in[y*pRaw.width+x+1];
			B1 = in[y*pRaw.width+x];
			R2 = in[(y+1)*pRaw.width+x+3];
			G2 = in[y*pRaw.width+x+3];
			B2 = in[y*pRaw.width+x+2];
			/* Fixed-Point: Faktoren * 128 */
			/* Y = 0.299*R+0.587*G+0.114*B) */

			pY1 = ((38*R1 + 75*G1 + 15*B1) >> 7);	
			pU1 = (((B1 - pY1)*63)>>7)+128;
			pV1 = (((R1 - pY1)*112)>>7)+128;

			pY2 =  (38*R2 + 75*G2 + 15*B2) >> 7;	

			rowmean = rowmean + pY1 + pY2;

			out[outPos++]=pU1;
			out[outPos++]=pY1;
			out[outPos++]=pV1;
			out[outPos++]=pY2;
		} /* for x */
		mean = mean + rowmean / (pRaw.width / 2);
	} /* for y */
	pOut->width  = pRaw.width/2;
	pOut->height = pRaw.height/2; 
	pOut->type  = OSC_PICTURE_YUV_422;
	if (stats != 0)
		stats->mean = mean / pOut->height;
	return 0;
} /* fastdebayer */

/* fastdebayerChromU
 * Very simple debayering. Makes one chrominance "U" pixel out of 4 bayered pixels
 * This means that the resulting image is only width/2 by height/2 pixels
 * Image size is reduced by a factor of 4!
 * And returns the image in 8 Bit/Pixel Format
 */
int fastdebayerChromU(const struct OSC_PICTURE pRaw, 
		struct OSC_PICTURE *pOut, struct ImgStats *stats) 
{
	uint16 x,y;
	uint32 outPos=0;
	int16 pY1, pU1;
	int16 R1, G1, B1;
	unsigned char *in  = (unsigned char *)pRaw.data;
	unsigned char *out = (unsigned char *)pOut->data;
	uint32 rowmean;
	uint32 mean=0;

	for (y=0; y<pRaw.height; y+=2) {
		rowmean=0;
		for (x=0; x<pRaw.width; x+=2) {
			R1 = in[(y+1)*pRaw.width+x+1];
			G1 = in[y*pRaw.width+x+1];
			B1 = in[y*pRaw.width+x];
			/* Fixed-Point: Faktoren * 128 */
			/* Y = 0.299*R+0.587*G+0.114*B) */

			pY1 = ((38*R1 + 75*G1 + 15*B1) >> 7);	
			pU1 = (((B1 - pY1)*63)>>7)+128;

			rowmean = rowmean + pY1;

			out[outPos++]=pU1;
		} /* for x */
		mean = mean + rowmean / (pRaw.width / 2);
	} /* for y */
	pOut->width  = pRaw.width/2;
	pOut->height = pRaw.height/2; 
	pOut->type  = OSC_PICTURE_CHROM_U;
	if (stats != 0)
		stats->mean = mean / pOut->height;
	return 0;
} /* fastdebayer */

/* fastdebayerChromV
 * Very simple debayering. Makes one chrominance "V" pixel out of 4 bayered pixels
 * This means that the resulting image is only width/2 by height/2 pixels
 * Image size is reduced by a factor of 4!
 * And returns the image in 8 Bit/Pixel Format
 */
int fastdebayerChromV(const struct OSC_PICTURE pRaw, 
		struct OSC_PICTURE *pOut, struct ImgStats *stats) 
{
	uint16 x,y;
	uint32 outPos=0;
	int16 pY1, pV1;
	int16 R1, G1, B1;
	unsigned char *in  = (unsigned char *)pRaw.data;
	unsigned char *out = (unsigned char *)pOut->data;
	uint32 rowmean;
	uint32 mean=0;

	for (y=0; y<pRaw.height; y+=2) {
		rowmean=0;
		for (x=0; x<pRaw.width; x+=2) {
			R1 = in[(y+1)*pRaw.width+x+1];
			G1 = in[y*pRaw.width+x+1];
			B1 = in[y*pRaw.width+x];
			/* Fixed-Point: Faktoren * 128 */
			/* Y = 0.299*R+0.587*G+0.114*B) */

			pY1 = ((38*R1 + 75*G1 + 15*B1) >> 7);	
			pV1 = (((R1 - pY1)*112)>>7)+128;

			rowmean = rowmean + pY1;

			out[outPos++]=pV1;
		} /* for x */
		mean = mean + rowmean / (pRaw.width / 2);
	} /* for y */
	pOut->width  = pRaw.width/2;
	pOut->height = pRaw.height/2; 
	pOut->type  = OSC_PICTURE_CHROM_U;
	if (stats != 0)
		stats->mean = mean / pOut->height;
	return 0;
} /* fastdebayer */

/* fastgrey
 * Very simple debayering. Makes one grey pixel out of 4 bayered pixels
 * This means that the resulting image is only width/2 by height/2 pixels
 * Image size is reduced by a factor of 4!
 * This needs about 6ms for a full 750x480 frame on leanXcam and returns
 * the image in 8Bit per pixel greyscale format
 * The resulting image is also the Luminance part of a YUV image
 */
int fastgrey(   const struct OSC_PICTURE pRaw, 
		struct OSC_PICTURE *pOut, struct ImgStats *stats) 
{
	uint16 x,y;
	uint32 outPos=0;
	int16 pY1;
	int16 R1, G1, B1;
	uint32 rowmean;
	uint32 mean=0;

	unsigned char *in  = (unsigned char *)pRaw.data;
	unsigned char *out = (unsigned char *)pOut->data;

	for (y=0; y<pRaw.height; y+=2) {
		rowmean=0;
		for (x=0; x<pRaw.width; x+=2) {
			R1 = in[(y+1)*pRaw.width+x+1];
			G1 = in[y*pRaw.width+x+1];
			B1 = in[y*pRaw.width+x];
			/* Fixed-Point: Faktoren * 128 */
			/* Y = 0.299*R+0.587*G+0.114*B) */

			pY1 = ((38*R1 + 75*G1 + 15*B1) >> 7);	

			rowmean = rowmean + pY1;

			out[outPos++]=pY1;
		} /* for x */
		mean = mean + rowmean / (pRaw.width / 2);
	} /* for y */
	pOut->width  = pRaw.width/2;
	pOut->height = pRaw.height/2; 
	pOut->type  = OSC_PICTURE_GREYSCALE;
	if (stats != 0)
		stats->mean = mean / pOut->height;
	return 0;
} /* fastdebayer */
