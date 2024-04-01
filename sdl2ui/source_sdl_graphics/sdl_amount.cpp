/*============================================================================*/
/**  @file       sdl_amount.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Default dialog.
 **
 **  Create a default amount box.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Camount
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
#include "sdl_amount.h"
#include "sdl_rectangle.h"

/*============================================================================*/
///
/// @brief Constructor complex version.
//
/// @param x [in] Position.
/// @param y [in] Position.
/// @param width [in] Position.
/// @param height [in] Position.
///
/*============================================================================*/
CamountButton::CamountButton( Cdialog *parent, const Crect &rect, keybutton code, const Sfont &font,
		   const Cmoney &amount, colour text)
: Cbutton( parent, rect, code, (Sfont)CtextFont("amounts"), (std::string)amount,
		   BORDER_NONE, GRAVITY_RIGHT_CENTER)
{
	(void)font;
	roundedRectangle(0);
	m_textColour =text;
}

/*============================================================================*/
///
/// @brief Constructor simple version.
//
/// @param rect [in] Position.
/// @param value [in] Amount to display.
///
/*============================================================================*/
CamountButton::CamountButton( Cdialog *parent, const Crect &rect, const Cmoney &amount)
: Cbutton( parent, rect, KEY_NONE, (Sfont)CtextFont("amounts"), (std::string)amount,
		   BORDER_NONE, GRAVITY_RIGHT_CENTER)
{
	roundedRectangle(0);
}

CamountButton::~CamountButton() {}

void CamountButton::setAmount( const Cmoney &amount)
{
	setText( (std::string)amount);
}
