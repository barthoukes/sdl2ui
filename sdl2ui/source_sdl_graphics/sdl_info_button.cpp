/*============================================================================*/
/**  @file       sdl_button.cpp
 **  @ingroup    zhongcan_sdl
 **  @brief		 Default button implementation.
 **
 **  Create an information button. This is a button with text in front
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CinfoButton
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
#include "sdl_info_button.h"

/*============================================================================*/
///
/// @brief Constructor special types.
//
/// @param rect [in] Position.
/// @param type [in] What type of button.
/// @param key  [in] What key.
/// @param text [in] What text.
/// @param icon [in] Which icon.
///
/*============================================================================*/
CinfoButton::CinfoButton( Cdialog *parent,
			     const Crect &rect,
			     textId id,
			     keybutton code,
			     const std::string &defaultText)
: CdialogObject( parent, rect, code),
  m_title( parent,
		   Crect( rect.left(), rect.top(), Cgraphics::m_defaults.width/8/10, rect.height()),
		   code,
		   (Sfont)CtextFont("info_button"),
		   id,
		   BORDER_NONE,
		   GRAVITY_RIGHT_CENTER,
		   0,
		   FILL_GRADIENT),
  m_data( parent,
		  Crect( rect.left()+m_title.width(), rect.top(), rect.width()-m_title.width(), rect.height()),
		  code,
		  defaultText)
{
	m_title.setLabel("");
}

CinfoButton::~CinfoButton()
{
}

void CinfoButton::onPaint( int touch)
{
	m_title.onPaint( 0);
	m_data.onPaint( touch);
}

void CinfoButton::setText( const std::string &text)
{
	m_data.setText( text);
}

std::string CinfoButton::getText()
{
	return m_data.getText();
}

void CinfoButton::setTitleColours( colour infoText,
			            colour background1, colour background2)
{
	(void)infoText;
	(void)background1;
	(void)background2;
}

void CinfoButton::setDataColours( colour infoText,
			            colour background1, colour background2)
{
	m_data.setColours( infoText, background1, background2);
}

void CinfoButton::setTitleWidth( int width)
{
	(void)width;
}

