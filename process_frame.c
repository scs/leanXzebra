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

/*! @file process_frame.c
 * @brief Contains the actual algorithm and calculations.
 */

/* Definitions specific to this application. Also includes the Oscar main header file. */
#include "template.h"

OSC_ERR ProcessFrame(uint8 *pRawImg) {
OscFunctionBegin
	OSC_ERR err;
	enum EnBayerOrder enBayerOrder;

	/* Debayer to monochron image. Color sensor assumed. */	
	OscCall( OscCamGetBayerOrder, &enBayerOrder, 0, 0);
	OscCall( OscVisDebayer, pRawImg, OSC_CAM_MAX_IMAGE_WIDTH, OSC_CAM_MAX_IMAGE_HEIGHT, enBayerOrder, data.u8ResultImage);


	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
	/* |                                                                 */
	/* |                    Add your code here                           */
	/* |                                                                 */
	/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
OscFunctionCatch

OscFunctionEnd
}
