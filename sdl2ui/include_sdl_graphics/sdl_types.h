/*
 * sdl_types.h
 *
 *  Created on: 23 dec. 2011
 *      Author: mensfort
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2011, 2014, 2015
 ** Houkes Horeca Applications
 **
 ** This file is part of the SDL2UI Library.  This library is free
 ** software; you can redistribute it and/or modify it under the
 ** terms of the GNU General Public License as published by the
 ** Free Software Foundation; either version 3, or (at your option)
 ** any later version.

 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.

 ** Under Section 7 of GPL version 3, you are granted additional
 ** permissions described in the GCC Runtime Library Exception, version
 ** 3.1, as published by the Free Software Foundation.

 ** You should have received a copy of the GNU General Public License and
 ** a copy of the GCC Runtime Library Exception along with this program;
 ** see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
 ** <http://www.gnu.org/licenses/>
 **===========================================================================*/

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include "sdl_keybutton.h"

#if 0
/// Colours by default.
enum Ecolours
{
    COLOUR_BLACK 		= 0x00000000,          /* dark colors */
    COLOUR_DARKGRAY     = 0x00404040,       /* light colors */
    COLOUR_MEDIUMGREY   = 0x00989898,
    COLOUR_GREYEDOUT    = 0x00c4c4c4,
    COLOUR_LIGHTGREY	= 0x00D3D3D3,
    COLOUR_WHITE		= 0x00EEEEEE,
    COLOUR_NONE			= 0x8FFFFFFF,
    COLOUR_DARKBLUE		= 0x000000C0,
    COLOUR_BLUE  		= 0x000000FF,
    COLOUR_LIGHTBLUE	= 0x00ADD8E6,
    COLOUR_VERY_LIGHTBLUE = 0x00CDECF3,

    COLOUR_GREEN 		= 0x00008000,
    COLOUR_DARKGREEN 	= 0x00002000,
    COLOUR_LIGHTGREEN		= 0x0020B2AA,
    COLOUR_VERY_LIGHTGREEN	= 0x0090DCD8,

    COLOUR_DARKCYAN		= 0x00006060,
    COLOUR_CYAN			= 0x0000F0F0,
    COLOUR_LIGHT_CYAN  	= 0x0060FFFF,
    COLOUR_VERY_LIGHTCYAN	= 0x00D0FFFF,

    COLOUR_BORDEAUX			= 0x0060000,
    COLOUR_RED				= 0x00E00000,
    COLOUR_LIGHTRED	    	= 0x00FF3030,
    COLOUR_VERY_LIGHTRED 	= 0x00FFC0C0,
    COLOUR_INDIAN_LIGHTRED 	= 0x00DE7272,
    COLOUR_INDIAN_RED   	= 0x00C86464,

    COLOUR_DARKMAGENTA	= 0x00600060,
    COLOUR_MAGENTA		= 0x00C000C0,
    COLOUR_LIGHTMAGENTA= 0x00FF30FF,
    COLOUR_VERY_LIGHTMAGENTA =0x00FFB0FF,

    COLOUR_DARKBROWN	= 0x00521515,
    COLOUR_BROWN		= 0x00A52A2A,
    COLOUR_LIGHTBROWN	= 0x00B54A4A,
    COLOUR_VERY_LIGHTBROWN	= 0x00C56A6A,

    COLOUR_DARKYELLOW   = 0x00A0A000,
    COLOUR_YELLOW		= 0x00E0E000,
    COLOUR_LIGHTYELLOW			= 0x00FFFF20,
    COLOUR_VERY_LIGHTYELLOW		= 0x00FFFF80
};

/// Colour sets for a different look.
enum EcolourMode
{
	MODE_ZUIHAO,
	MODE_BLACK_WHITE,
	MODE_MINT,
	MODE_3,
	MODE_4,
	MODE_5,
	MODE_6,
	MAX_COLOUR_MODE,
};


#define SOLID_FILL 		1
#define INTERLEAVE_FILL 2
#endif

/// All colours.
typedef int colour;


/// @brief Status for message when sent to any client.
typedef enum
{
	DIALOG_EVENT_PROCESSED,		///< Event processed, stop sending.
	DIALOG_EVENT_OPEN,			///< Event still open.
	DIALOG_EVENT_GENERAL,		///< Dialog can be sent to all.
	DIALOG_EVENT_EXIT			///< Dialog request to stop the dialog.
} Estatus;

/* SDL_TYPES_H_ */
