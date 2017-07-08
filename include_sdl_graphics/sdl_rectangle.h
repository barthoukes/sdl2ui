/*
 * sdl_rectangle.h
 *
 *  Created on: 18 dec. 2011
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
#include "sdl_background.h"

/// @brief  Border type for buttons.
typedef enum
{
	BORDER_NONE,
	BORDER_THIN,
	BORDER_THIN_SHADOW,
	BORDER_THIN_SELECTED,
	BORDER_THICK,
	BORDER_THICK_SHADOW,
	BORDER_ITEM_THICK,
	BORDER_THICK_SELECTED,
	BORDER_THIN_BUTTON_SHADOW,
} EborderType;


/// @brief Rectangle to display on screen.
class Crectangle : public CdialogObject
{
public:
	Crectangle( Cdialog *parent,
			    const Crect &rect,
			    keybutton code,
			    EborderType border=BORDER_THIN,
			    colour back=COLOUR_BLACK,
			    int corner=0,
			    EfillType fill=FILL_UNICOLOURED,
			    colour col2=COLOUR_DARKGRAY);
	//Crectangle( const std::string &name, const Crect &rect, keybutton code, EborderType border =BORDER_NONE,
	//		    colour back=COLOUR_BLACK);
	virtual ~Crectangle();

	void    setBackground( EfillType pattern,
			               colour background1=COLOUR_BLACK,
			               colour foreground2=COLOUR_WHITE,
			               colour background2=COLOUR_DARKGRAY);
	virtual void onPaint( int touch);
	void	setCode( keybutton key);
	EborderType getBorder() { return m_border; }
	void 	setBorder( EborderType border) { m_border=border; }
	void    setFillStyle( EfillType pattern);
	void 	paintBorder();

private:
	void paintCross();

private:
	Cbackground	m_square;	///< Background square to display.
	EborderType m_border; 	///< What type of border we paint.
	colour		m_border1;  ///< First border colour.
	colour 		m_border2;  ///< Second border colour.
    EfillType   m_fill;     ///< Fill type.
};
