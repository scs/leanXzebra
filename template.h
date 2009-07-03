/*	A collection of example applications for the LeanXcam platform.
	Copyright (C) 2008 Supercomputing Systems AG
	
	This library is free software; you can redistribute it and/or modify it
	under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation; either version 2.1 of the License, or (at
	your option) any later version.
	
	This library is distributed in the hope that it will be useful, but
	WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
	General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with this library; if not, write to the Free Software Foundation,
	Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*! @file template.h
 * @brief Global header file for the template application.
 */
#ifndef TEMPLATE_H_
#define TEMPLATE_H_

/*--------------------------- Includes -----------------------------*/
#include <stdio.h>
#include "oscar.h"

/*--------------------------- Settings ------------------------------*/
/*! @brief Timeout (ms) when waiting for a new picture. */
#define CAMERA_TIMEOUT 1

/*! @brief The file name of the test image on the host. */
#define TEST_IMAGE_FN "test.bmp"

/*------------------- Main data object and members ------------------*/



/*! @brief The structure storing all important variables of the application.
 * */
struct TEMPLATE
{
	/*! @brief The frame buffer for the frame capture device driver.*/
	uint8 u8FrameBuffer[ OSC_CAM_MAX_IMAGE_HEIGHT*OSC_CAM_MAX_IMAGE_WIDTH];
	struct OSC_PICTURE pictureRaw;
	/*! @brief A buffer to hold a full size color image. */
	uint8 u8ColorImage[ 3*OSC_CAM_MAX_IMAGE_WIDTH*OSC_CAM_MAX_IMAGE_HEIGHT];
	struct OSC_PICTURE pictureColor;
	/*! @brief A buffer to hold a quarter size grey image. */
	uint8 u8GreyImage[ OSC_CAM_MAX_IMAGE_WIDTH*OSC_CAM_MAX_IMAGE_HEIGHT/4];
	struct OSC_PICTURE pictureGrey;
		
#if defined(OSC_HOST) || defined(OSC_SIM)
	/*! @brief File name reader for camera images on the host. */
	void *hFileNameReader;
#endif /* OSC_HOST or OSC_SIM */
	/*! @brief The last raw image captured. Always points to the frame
	 * buffer. */
	uint8* pRawImg;

};

extern struct TEMPLATE data;

/*-------------------------- Functions --------------------------------*/

/*********************************************************************//*!
 * @brief Process a newly captured frame.
 * 
 * In the case of this template, this consists just of debayering the
 * image and writing the result to the result image buffer. Debayer
 * to full size color and quater size grey is applied. This should be
 * the starting point where you add your code.
 * 
 * @return SUCCESS or appropriate error code.
 *//*********************************************************************/
OSC_ERR ProcessFrame(void);

#endif /*TEMPLATE_H_*/
