/*============================================================================*/
/**  @file       sdl_rect_group.hpp
 **  @ingroup    sdl2ui
 **  @brief		 Rectangle group for our system
 **
 **  Rectangle group to find rectangles in an area
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CrectGroup
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2011, 2014, 2019
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
#include "sdl_rect.h"

/// Class for rectangles
class CrectGroup
{
public:
	CrectGroup( const Crect &rect, int horizontal_groups, int vertical_groups);
	~CrectGroup();
	int size();

public:
	Crect m_rect;
	int m_horizontal;
	int m_vertical;
	int m_width;
	int m_height;
	int m_left;
	int m_top;
	int m_bottom;
	Crect operator[]( int index);
};

/* SDL_RECT_GROUP_H_ */
