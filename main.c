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
	//	fprintf(stderr, "%s: Unable to create framework.\n", __func__);
	OscAssert_em( NULL == data.hFramework, EOUT_OF_MEMORY, "hFramework error");
	
	/******* Load the framework module dependencies. **********/
	OscCall( OscLoadDependencies, data.hFramework, deps, sizeof(deps)/sizeof(struct OSC_DEPENDENCY));
	// fprintf(stderr, "%s: ERROR: Unable to load dependencies! (%d)\n", __func__, err);

	
	/********* Seed the random generator *************/
	srand(OscSupCycGet());
	
#if defined(OSC_HOST) || defined(OSC_SIM)
	OscCall( OscFrdCreateConstantReader, &data.hFileNameReader, TEST_IMAGE_FN);
	//	OscLog(ERROR, "%s: Unable to create constant file name reader for %s! (%d)\n", __func__, TEST_IMAGE_FN, err);

	OscCall( OscCamSetFileNameReader, data.hFileNameReader);
	//	OscLog(ERROR, "%s: Unable to set file name reader for camera! (%d)\n", __func__, err);
#endif /* OSC_HOST or OSC_SIM */

	/* Set the camera registers to sane default values. */
	OscCall( OscCamPresetRegs);
	// OscLog(ERROR, "%s: Unable to preset camera registers! (%d)\n", __func__, err);

	OscCall( OscCamSetupPerspective, OSC_CAM_PERSPECTIVE_DEFAULT );
	
	/* Set up one frame buffer with enough space for the maximum
	 * camera resolution in cached memory. */
	OscCall( OscCamSetFrameBuffer, 0, OSC_CAM_MAX_IMAGE_WIDTH*OSC_CAM_MAX_IMAGE_HEIGHT, data.u8FrameBuffer, TRUE);
	//	OscLog(ERROR, "%s: Unable to set up frame buffer!\n", __func__);

	OscLogSetConsoleLogLevel(DEBUG);
	OscLogSetFileLogLevel(WARN);
	
	
OscFunctionCatch
	/* Unload and destory if a error was catched while above */
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
//	uint8 *pCurRawImg = NULL;
	
	OscCall( Init, argc, argv);
	
	/* Image acquisation loop */	
//	while( true)
//	{		
//		err = OscCamSetupCapture( 0);	
//		err = OscGpioTriggerImage();
//		
//		err = OscCamReadPicture( 0, &pCurRawImg, 0, CAMERA_TIMEOUT);
//		
//		ProcessFrame( pCurRawImg);	
//	}	


OscFunctionCatch
	OscUnloadDependencies(data.hFramework, deps, sizeof(deps)/sizeof(struct OSC_DEPENDENCY));
	OscDestroy(data.hFramework);
	
	OscLog(INFO, "Quit application abnormally.\n");

OscFunctionEnd
}
