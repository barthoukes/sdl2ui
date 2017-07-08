/*============================================================================*/
/**  @file       sdl_rect.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Rectangle for our system
 **
 **  Simple rectangle, simple size and simple implementation of a point
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cpoint, Csize, Crect
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
#include <stdlib.h>
#include "sdl_rect.h"

Cpoint & Cpoint::operator /=(int n)
{
	x /=n;
	y /=n;
	return *this;
}

Cpoint & Cpoint::operator -=(const Cpoint &v)
{
	x -=v.x;
	y -=v.y;
	return *this;
}

int Cpoint::horizontalDistance( const Cpoint &v) const
{
	return abs( v.x-x);
}

int Cpoint::verticalDistance( const Cpoint &v) const
{
	return abs( v.y-y);
}


