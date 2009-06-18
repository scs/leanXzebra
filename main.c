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

/*! @file main.c
 * @brief Main file of the template application. Contains initialization
 * and image acquisition loop.
 */

#include "template.h"
#include <string.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

/*! @brief This stores all variables needed by the algorithm. */
struct TEMPLATE data;

/*! @brief The framework module dependencies of this application. */
struct OSC_DEPENDENCY deps[] = {
	{ "log", OscLogCreate, OscLogDestroy },
	{ "sup", OscSupCreate, OscSupDestroy },
	{ "bmp", OscBmpCreate, OscBmpDestroy },
	{ "cam", OscCamCreate, OscCamDestroy },
	{ "vis", OscVisCreate, OscVisDestroy },
	{ "gpio", OscGpioCreate, OscGpioDestroy }
};

/*********************************************************************//*!
 * @brief Initialize everything so the application is fully operable
 * after a call to this function.
 * 
 * @return SUCCESS or an appropriate error code.
 *//*********************************************************************/
static OSC_ERR Init(const int argc, const char * argv[]) {
OscFunctionBegin	
	
	memset(&data, 0, sizeof(struct TEMPLATE));
	
	/******* Create the framework **********/
	OscCall( OscCreate, &data.hFramework);	
	OscCall( OscLoadDependencies, data.hFramework, deps, sizeof(deps)/sizeof(struct OSC_DEPENDENCY));

	OscLogSetConsoleLogLevel(ERROR);
	OscLogSetFileLogLevel(ERROR);

	
	/* Seed the random generator */
	srand(OscSupCycGet());
	
	/* Set the camera registers to sane default values. */
	OscCall( OscCamPresetRegs);
	OscCall( OscCamSetupPerspective, OSC_CAM_PERSPECTIVE_DEFAULT);

	/* Configure camera emulation on host */
#if defined(OSC_HOST) || defined(OSC_SIM)
	OscCall( OscFrdCreateConstantReader, &data.hFileNameReader, TEST_IMAGE_FN);
	OscCall( OscCamSetFileNameReader, data.hFileNameReader);
#endif /* OSC_HOST or OSC_SIM */
	
	/* Set up one frame buffer for maximum image size. Cached memory. */
	OscCall( OscCamSetFrameBuffer, 0, OSC_CAM_MAX_IMAGE_WIDTH*OSC_CAM_MAX_IMAGE_HEIGHT, data.u8FrameBuffer, TRUE);	
	
OscFunctionCatch
	/* Unload and destory if a error was catched above */
	OscUnloadDependencies(data.hFramework, deps, sizeof(deps)/sizeof(struct OSC_DEPENDENCY));
	OscDestroy(data.hFramework);
	OscMark_m( "Initialization failed!");
	
OscFunctionEnd
};

/*********************************************************************//*!
 * @brief Program entry
 * 
 * @param argc Command line argument count.
 * @param argv Command line argument strings.
 * @return 0 on success
 *//*********************************************************************/
int main(const int argc, const char * argv[]) {
OscFunctionBegin
	uint8 *pCurRawImg = NULL;
	OSC_ERR err;
	
	/* Initialize system */
	OscCall( Init, argc, argv);
	
	/* Image acquisation loop */	
	while( true)
	{		
		err = OscCamSetupCapture( 0);	
		err = OscGpioTriggerImage();
		
		while( true)
		{
			err = OscCamReadPicture( 0, &pCurRawImg, 0, CAMERA_TIMEOUT);
			if(err == SUCCESS)
			{ 
				break;
			}
		}
		
		ProcessFrame( pCurRawImg);
	}	


OscFunctionCatch
	OscUnloadDependencies(data.hFramework, deps, sizeof(deps)/sizeof(struct OSC_DEPENDENCY));
	OscDestroy(data.hFramework);
	
	OscLog(INFO, "Quit application abnormally!\n");

OscFunctionEnd
}
