/*============================================================================*/
/**  @file      sdl_horizontal_line.hpp
 **  @ingroup   user_interface
 **  @brief		Draw horizontal line.
 **
 **  Create and show buttons.
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

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <memory>
#include <string>
#include "sdl_background.h"
//#include "sdl_keybutton.h"
//#include "sdl_graphics.h"
#include "sdl_dialog_object.h"
//#include "sdl_rectangle.h"
//#include "sdl_text.h"
//#include "sdl_label.h"
//#include "sdl_image.h"

/// @brief  Forward declaration.
class Cdialog;

/// @brief  Create and display buttons.
class ChorizontalLine : public CdialogObject
{
public:
	ChorizontalLine( Cdialog *parent,
			         const Crect &rect,
			         int lineWidth,
			         keybutton code,
					 colour back1,
					 colour back2);
	virtual ~ChorizontalLine();

public:
	virtual void onPaint( int touch);
	void 	setLineColour( colour b1);
	void	setBackgroundColour( colour background, colour background2=COLOUR_DARKGRAY);
	void 	roundedRectangle( int radius);

protected:
	void 	paintBackground( int touch);
	void	paintBorder( EborderType border, int spacing, int radius, int touch);

protected:
	Cbackground			m_background;	///< Square inside.
	colour		 		m_lineColour; ///< Colour text. Almost obsolete.
	int					m_lineWidth; ///< Width of the lines.
};

typedef std::shared_ptr<ChorizontalLine> ChorizontalLinePtr;
