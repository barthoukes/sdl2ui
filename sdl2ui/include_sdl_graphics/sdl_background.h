/*
 * sdl_background.h
 *
 *  Created on: 23 dec. 2011
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
#include "sdl_dialog_object.h"
#include "sdl_graphics.h"
#include "sdl_rect.h"

/// @brief  Fill pattern.
typedef enum
{
	FILL_NONE, ///< No fill.
	FILL_UNICOLOURED,	///< No fill.
	FILL_2COLOURS, ///< Complete fill.
	FILL_GRADIENT, ///< Fill colour 1 to 2.
	FILL_PYRAMID, ///< First colour in middle.
	FILL_IMAGE, ///< Fill with image.
	FILL_CIRCULAR, ///< Fill with circular background
	FILL_PIE ///< Fill pie with 2 parts
} EfillType;

// Light factors.
#define LIGHT_HALF		-50
#define LIGHT_ACCENT	-125

class Cbackground : public CdialogObject
{
public:
	Cbackground( Cdialog *parent,
			 const Crect &rect,
			 keybutton code=KEY_NONE,
			 colour col=COLOUR_BLACK,
			 int radius=0,
			 EfillType fill=FILL_UNICOLOURED,
			 colour col2=COLOUR_BLUE);
	void setColours( colour col1, colour col2=COLOUR_DARKGRAY);
	void setFillStyle( EfillType style) { m_pattern=style; }
	void onPaint( int touch);
	void onPaintImageButton();
	void setRadius( int radius) { m_radius=radius; }
	int getRadius() { return m_radius; }
	bool pieColour( int dx, int dy);
	void setBackground( const std::string &image)
	{
		m_image =image;
		m_pattern =FILL_IMAGE;
	}
	virtual ~Cbackground();
	void setBorderWidth( int width) { m_borderWidth=width; }
	void setCode( keybutton key) { m_key=key; }
	colour getColour() { return m_col1; }
	void setCorner(int cr) { m_pieCorner =cr%360; }

private:
	colour calcColour( colour col1, colour col2, double fraction);

private:
	colour	  	m_col1;			///< Fill colour.
	colour		m_col2;         ///< Colour 2.
	EfillType 	m_pattern;		///< Pattern to fill.
	keybutton 	m_key;			///< What key?
	int         m_radius;       ///< Corner size.
	std::string m_image;		///< Background for button.
	int m_R1, m_G1, m_B1, m_R2, m_G2, m_B2;
	int			m_pieCorner;	///< Corner pie.
public:
	int			m_borderWidth;  ///< To reduce the painting.
};

