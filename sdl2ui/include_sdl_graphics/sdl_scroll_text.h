/*============================================================================*/
/**  @file       sdl_scroll_text.h
 **  @ingroup    sdl2ui
 **  @brief		 Text to display
 **
 **  Create a text object, with font, background, multi-language etc.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Ctext
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
#include <string>
#include "sdl_dialog_object.h"
#include "sdl_graphics.h"
#include "sdl_font.h"
#include "sdl_surface.h"

/// @brief Button to display.
class CscrollText : public CdialogObject
{
public:
	CscrollText( Cdialog *parent, const Crect &rect, keybutton code, const Sfont &font,
		   const std::string &value, colour text=0, int background=0);
	virtual ~CscrollText();
	virtual void onPaint( int touch);
	void 	onPaint( const std::string &text, int touch);
	void 	setColour( colour text);
	void	setText( const std::string &text);
	bool	empty();
	void    setShadow( colour x) { m_shadow =x; }
	void	setFont( const Sfont &font) { m_font =font; }
	void	setStyle( int style =TTF_STYLE_NORMAL) { m_style =style; }
	void    bold() { setStyle( TTF_STYLE_BOLD); }
	void setSpeed( double speed) { m_speed =speed; }

private:
	std::string 	m_value; ///< what to display.
	colour			m_colour; ///< Colour for text.
	Sfont			m_font; ///< font to use.
	int				m_style; ///< Fat/italic characters.
	int				m_horizontal_margin; ///< Horizontal margin.
	int				m_vertical_margin; ///< Vertical margin.
	int             m_shadow; ///< Schaduw.
	bool			m_rotate; ///< Rotate the text.
	double			m_speed; ///< Speed of scrolling text.
	double			m_scroll; ///< Scroll distance.
	CtextSurface	m_surface; ///< Surface to paint on
};
