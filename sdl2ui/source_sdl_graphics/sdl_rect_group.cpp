/*============================================================================*/
/**  @file       sdl_rect_group.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Low level group of rectangles
 **
 **  Group of rectangles horizontal for now.
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

/*------------- Standard includes --------------------------------------------*/
#include "sdl_rect.h"
#include "sdl_rect_group.hpp"

CrectGroup::CrectGroup(const Crect &rect, int horizontal_groups, int vertical_groups)
: m_rect(rect)
, m_horizontal(horizontal_groups)
, m_vertical(vertical_groups)
, m_width(rect.width())
, m_height(rect.height())
, m_left(rect.left())
, m_top(rect.top())
, m_bottom(rect.bottom())
{
}

CrectGroup::~CrectGroup()
{}

int CrectGroup::size()
{
	return m_horizontal * m_vertical;
}

Crect CrectGroup::operator [](int index)
{
    int hor = index % m_horizontal;
    int ver = index / m_horizontal;
	int left = m_left+hor*m_width/m_horizontal;
	int right = m_left+(hor+1)*m_width/m_horizontal;
	int top = m_top+ver*m_height/m_vertical;
	int bottom = m_top+(ver+1)*m_height/m_vertical;
	Crect c(left, top, right-left, bottom-top);
	return c;
}
