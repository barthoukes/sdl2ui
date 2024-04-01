/*============================================================================*/
/**  @file      sdl_fast_arc.h
 **  @ingroup   user_interface
 **  @brief		hand-clock library.
 **
 **  Create timestamp.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CfastArc
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2011, 2014, 2018
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
#include <vector>
#include "sdl_types.h"

class Cgraphics;

typedef struct
{
	short x,y;
} Spixel;

#define ARC_FIELDS 30
class CfastArc
{
public:
	CfastArc(int width, int height, colour col);
	~CfastArc();
	void onPaint( Cgraphics *graphics, int touch, int centerX, int centerY, int angle);
	void setSize( int w, int h);

private:
	int pieColour( int dx, int dy);
private:
	colour colours[3]; // First colour, second colour and ball
	std::vector<Spixel> pixels[ARC_FIELDS];
	int width;
	int height;
};
