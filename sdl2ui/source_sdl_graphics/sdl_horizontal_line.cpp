/*============================================================================*/
/**  @file       sdl_horizontal_line.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Default button implementation.
 **
 **  Create a default button. This is a basic template for all buttons.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              ChorizontalLine
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
#include "sdl_horizontal_line.hpp"

/*============================================================================*/
///
/// @brief Constructor to be expired...
//
/// @param x [in] Position.
/// @param y [in] Position.
/// @param width [in] Position.
/// @param height [in] Position.
///
/*============================================================================*/
ChorizontalLine::ChorizontalLine( Cdialog *parent,
			                      const Crect &rect,
			                      int lineWidth,
			                      keybutton code,
								  colour back1,
								  colour back2)
: CdialogObject( parent, rect, code)
, m_background( parent, rect, code, back1,
		        0, FILL_UNICOLOURED, back2)
, m_lineColour( Cgraphics::m_defaults.text_button_border)
, m_lineWidth( lineWidth)
{
}

/*============================================================================*/
///
/// @brief Destructor.
///
/*============================================================================*/
ChorizontalLine::~ChorizontalLine()
{
}

/*============================================================================*/
///
/// @brief Paint background to button.
///
/*============================================================================*/
void ChorizontalLine::paintBackground( int touch)
{
	m_background.setRect(m_rect);
	m_background.onPaint( touch);
}

/*----------------------------------------------------------------------------*/
void ChorizontalLine::roundedRectangle( int radius)
{
	m_background.setRadius( radius);
}

/*============================================================================*/
///
///  @brief 	Paint the line once.
///
///	 @pre		Called when dialog is invalidated.
///
///  @post		Button updated.
///
/*============================================================================*/
void ChorizontalLine::onPaint( int touch)
{
	paintBackground(touch);
	int y=(m_rect.top()*8+m_rect.bottom()*8)/2-(m_lineWidth/2);
	m_pGraphics->setColour(m_lineColour);
	m_pGraphics->bar(m_rect.left()*8, y, m_rect.right()*8, y+m_lineWidth-1);
}

/*============================================================================*/
///
/// @brief Set new colours.
///
/// @param text [in] Text colour.
/// @param background [in] Background colour.
///
/*============================================================================*/
void ChorizontalLine::setLineColour( colour b1)
{
	m_lineColour = b1;
}

/*============================================================================*/
///
/// @brief Set new colours.
///
/// @param text [in] Text colour.
///
/*============================================================================*/
void ChorizontalLine::setBackgroundColour( colour background, colour background2)
{
	m_background.setColours( background, background2);
}
