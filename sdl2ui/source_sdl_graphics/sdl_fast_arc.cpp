/*============================================================================*/
/**  @file       sdl_fast_arc.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Paint any shape made of pixels
 **
 **  Create a timestamp.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CfastArc
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

/*------------- Standard includes --------------------------------------------*/
#include <math.h>
#include "sdl_fast_arc.h"
#include "sdl_dialog_event.h"
#include "sdl_background.h"

CfastArc::CfastArc( int width, int height, colour col)
: width(width)
, height(height)
{
	colours[0] =col;
	colours[1] =Cgraphics::brighter( col, -40);
	colours[2] =Cgraphics::brighter( col, 40);

	setSize( width, height);
}

CfastArc::~CfastArc()
{
	for (int n=0; n<ARC_FIELDS; n++)
	{
		pixels[n].clear();
	}
}

void CfastArc::setSize( int w,  int h)
{
	width =w;
	height =h;
	w/=2;
	h/=2;
	double ellipse =(double)h/(double)w;
	Spixel px;
	for (px.y=0; px.y<height; px.y++)
	{
		int v=(int)( ellipse*sqrt( (double)(h*h-px.y*px.y))+0.5);
		for (px.x=0; px.x <v; px.x++)
		{
			double corner = atan2( px.y, px.x);
			int fields =corner*(double)ARC_FIELDS/(double)(M_PI/2.0);
			if (fields>=ARC_FIELDS) fields=ARC_FIELDS-1;
			if (fields<0) fields=0;
			pixels[fields].push_back(px);
		}
	}
}

void CfastArc::onPaint( Cgraphics *graph, int touch, int centerX, int centerY, int corner)
{
	(void)touch;
	int factor = 90.0/(double)ARC_FIELDS;

	// Quadrant 0-30 degrees, 150-180 degrees, 180-210 degrees and 330-360 degrees
	for (int n=0; n<ARC_FIELDS; n++)
	{
		int corner1 = (n+1)*factor;
		int corner2 = 90+corner1;
		int corner3 = 90+corner2;
		int corner4 = 90+corner3;
		colour c =(corner>=corner1) ? colours[0]:colours[1];
		graph->setColour( c);
		for ( Spixel &p: pixels[n])
		{
			graph->setPixel(centerX+p.y, centerY-p.x);
		}
		colour c2 = (corner>=corner2) ? colours[0]:colours[1];
		graph->setColour(c2);
		for ( Spixel &p: pixels[n])
		{
			graph->setPixel(centerX+p.x, centerY+p.y);
		}
		colour c3 = (corner>=corner3) ? colours[0]:colours[1];
		graph->setColour(c3);
		for ( Spixel &p: pixels[n])
		{
			graph->setPixel(centerX-p.y, centerY+p.x);
		}
		colour c4 = (corner>=corner4) ? colours[0]:colours[1];
		graph->setColour(c4);
		for ( Spixel &p: pixels[n])
		{
			graph->setPixel(centerX-p.x, centerY-p.y);
		}
	}
}
