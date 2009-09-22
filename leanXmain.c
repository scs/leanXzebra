/*	
	leanXstream, a streaming video server for the LeanXcam.
	Copyright (C) 2009 Reto Baettig
	
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


#include "oscar.h"
#include "zebra.h"
#include "image.h"
#include "leanXoverlay.h"
#include "leanXip.h"
#include "leanXtools.h"
#include "leanXalgos.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define REG_AEC_AGC_ENABLE 0xaf
#define CAM_REG_RESERVED_0x20 0x20
#define CAM_REG_CHIP_CONTROL 0x07
#define BUF_SIZE 1000

/*! @brief The system state and buffers of this application. */
struct SYSTEM {
	void *hFileNameReader;

	int32 shutterWidth; /* Microseconds */
	uint8 frameBuffer1[OSC_CAM_MAX_IMAGE_WIDTH * OSC_CAM_MAX_IMAGE_HEIGHT]; 
	uint8 frameBuffer2[OSC_CAM_MAX_IMAGE_WIDTH * OSC_CAM_MAX_IMAGE_HEIGHT];
	uint8 doubleBufferIDs[2]; /* The frame buffer IDs of the frame
				   * buffers creating a double buffer. */
	zebra_image_t *zimg;
	zebra_image_scanner_t *zscan;
	
} sys;

/*********************************************************************//*!
 * @brief Initialize framework and system parameters
 *
 * @param s Pointer to the system state 
 *//*********************************************************************/
OscFunction( InitSystem, struct SYSTEM *s)

	/******* Create the framework **********/
	OscCall( OscCreate,
		&OscModule_log,
		&OscModule_bmp,
		&OscModule_cam,
		&OscModule_vis,
		&OscModule_gpio,
		&OscModule_jpg);

	OscCall( OscLogSetConsoleLogLevel, WARN);
	OscCall( OscLogSetFileLogLevel, WARN);
	
	#if defined(OSC_HOST)
		/* Setup file name reader (for host compiled version); read constant image */
		OscCall( OscFrdCreateConstantReader, &s->hFileNameReader, "EAN13Example.bmp");
		OscCall( OscCamSetFileNameReader, s->hFileNameReader);
	#endif
	
	/* Configure camera */
	OscCall( OscCamPresetRegs);
	/* Set AGC and AEC */
	OscCall( OscCamSetRegisterValue, REG_AEC_AGC_ENABLE, 0x3);
	/* Turn on continuous capture for this application. */
	OscCall( OscCamSetRegisterValue, CAM_REG_CHIP_CONTROL, 0x388);
	/* Set the undocumented reserved almighty Micron register to the "optimal" value. */
	OscCall( OscCamSetRegisterValue, CAM_REG_RESERVED_0x20, 0x3d5);

	OscCall( OscCamSetAreaOfInterest, 0, 0, OSC_CAM_MAX_IMAGE_WIDTH, OSC_CAM_MAX_IMAGE_HEIGHT);
	OscCall( OscCamSetupPerspective, OSC_CAM_PERSPECTIVE_180DEG_ROTATE);

	OscCall( OscCamSetFrameBuffer, 0, OSC_CAM_MAX_IMAGE_WIDTH * OSC_CAM_MAX_IMAGE_HEIGHT, s->frameBuffer1, TRUE);
	OscCall( OscCamSetFrameBuffer, 1, OSC_CAM_MAX_IMAGE_WIDTH * OSC_CAM_MAX_IMAGE_HEIGHT, s->frameBuffer2, TRUE);

	s->doubleBufferIDs[0] = 0;
	s->doubleBufferIDs[1] = 1;
	OscCall( OscCamCreateMultiBuffer, 2, s->doubleBufferIDs);

	OscCall( OscCamSetupCapture, OSC_CAM_MULTI_BUFFER);

	s->zimg = zebra_image_create();
	zebra_image_set_format(s->zimg, fourcc('Y','8','0','0'));
	s->zscan = zebra_image_scanner_create(0);
	zebra_image_scanner_enable_cache(s->zscan, 1);
OscFunctionEnd() /* initSystem */

/*********************************************************************//*!
 * @brief Shut down system and close framework
 *
 * @param s Pointer to the system state 
 *//*********************************************************************/
OscFunction( CleanupSystem, struct SYSTEM *s)
	zebra_image_scanner_destroy(s->zscan);
	zebra_image_destroy(s->zimg);

	/* Destroy framework */
	OscDestroy();
OscFunctionEnd()

OscFunction( MainFunction, const int argc, const char * argv[])
	struct OSC_PICTURE calcPic;
	struct OSC_PICTURE rawPic;
	unsigned char *tmpbuf;
	int numsyms, num;
	zebra_symbol_t *sym;
	int y;	
	
	OscCall( InitSystem, &sys);

	ip_start_server();

	/* setup variables */
	rawPic.width = OSC_CAM_MAX_IMAGE_WIDTH;
	rawPic.height = OSC_CAM_MAX_IMAGE_HEIGHT;
	rawPic.type = OSC_PICTURE_GREYSCALE;

	/* calcPic width, height etc. are set in the debayering algos */
	calcPic.data = malloc(3 * OSC_CAM_MAX_IMAGE_WIDTH * OSC_CAM_MAX_IMAGE_HEIGHT);
	if (calcPic.data == 0)
		fatalerror("Did not get memory\n");
	tmpbuf = malloc(500000);
	if (tmpbuf == 0)
		fatalerror("Did not get memory\n");

	
	#if defined(OSC_TARGET)
		/* Take a picture, first time slower ;-) */
		usleep(10000); OscGpioTriggerImage(); usleep(10000);
		OscLog(DEBUG,"Triggered CAM ");
	#endif

	while(true) {

		OscCamReadPicture(OSC_CAM_MULTI_BUFFER, (void *) &rawPic.data, 0, 0);
		/* Take a picture */
		usleep(2000);
		OscCamSetupCapture(OSC_CAM_MULTI_BUFFER); 

		#if defined(OSC_TARGET)
			OscGpioTriggerImage();
		#else
			usleep(10000);
		#endif

		fastgrey(rawPic, &calcPic, NULL);

		zebra_image_set_data(sys.zimg, calcPic.data, calcPic.width*calcPic.height, NULL);
		zebra_image_set_size(sys.zimg, calcPic.width, calcPic.height);
		zebra_image_ref(sys.zimg, 1);
		numsyms = zebra_scan_image(sys.zscan, sys.zimg);
		if (numsyms>0) {
			sym = (zebra_symbol_t *)zebra_image_first_symbol(sys.zimg);
			y = 100;
			num = 1;
		    	while(sym) {
				zebra_symbol_type_t type = zebra_symbol_get_type(sym);
				int count = zebra_symbol_get_count(sym);
				printf("%s%s: %s (%s)\n",
					zebra_get_symbol_name(type),
					zebra_get_addon_name(type),
					zebra_symbol_get_data(sym),
					(count < 0) ? "uncertain" :
					(count > 0) ? "duplicate" : "new");

				overlay(&calcPic, 10, y, 3, (char *)zebra_symbol_get_data(sym));
				y+=10*3;
				sym = (zebra_symbol_t *)zebra_symbol_next(sym);
			}
		}
	
		if (ip_send_all((char *)calcPic.data, calcPic.width*calcPic.height*
                        OSC_PICTURE_TYPE_COLOR_DEPTH(calcPic.type)/8))
                        /*OscLog(ALERT, "Sent picture\n")*/;

                ip_do_work();
	
	}

	ip_stop_server();

	cleanupSystem(&sys);

OscFunctionCatch()
	/* Destruct framwork due to error above. */
	OscDestroy();
	OscMark_m( "Initialization failed!");

OscFunctionEnd()


/*********************************************************************//*!
 * @brief  The main program
 *
 * Opens the camera and reads pictures as fast as possible
 * Makes a debayering of the image
 * Writes the debayered image to a buffer which can be read by
 * TCP clients on Port 8111. Several concurrent clients are allowed.
 * The simplest streaming video client looks like this:
 *
 * nc 192.168.1.10 8111 | mplayer - -demuxer rawvideo -rawvideo w=376:h=240:format=bgr24:fps=100
 *
 * Writes every 10th picture to a .jpg file in the Web Server Directory
 *//*********************************************************************/
int main(const int argc, const char * argv[]) {
	if (MainFunction(argc, argv) == SUCCESS)
		return 0;
	else
		return 1;
}
