/*============================================================================*/
/**  @file      sdl_bar_graph.h
 **  @ingroup   user_interface
 **  @brief		Draw bar.
 **
 **  Show a vertical bar with info.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CbarGraph
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
#include "sdl_graphics.h"
#include "sdl_dialog_object.h"

/// @brief  Forward declaration.
class Cdialog;

/// @brief  Create and display buttons.
class CbarGraph : public CdialogObject
{
public:
	CbarGraph( Cdialog *parent, double value1, bool stacked, const std::string &text,
			   int spacing, int helplines, colour back);
	CbarGraph( Cdialog *parent, double value1, double value2, bool stacked,
			   const std::string &text, int spacing, int helplines, colour back);
	virtual ~CbarGraph();

public:
	void    onPaint( int touch) { (void)touch; }
	void	onPaint( const Crect &rect, int touch);
	void	setColours( int n, colour background);
	void 	setRange( double minimum, double maximum);
	void 	rotate();
	void setColourHelpLines( colour n);

private:
	std::vector<colour> m_colour; ///< Colour bar.

	std::vector<double> m_value; ///< Current y-value.
	double 				m_minimum; ///< Minimum slider value.
	double  			m_maximum; ///< Maximum slider value.
	bool				m_stacked;
	int					m_spacing; ///< Left + right spacing together.
	colour 				m_background; ///< Background of graph.
	std::string			m_text; ///< Text below graph.
	int					m_textHeight; ///< Height of the text.
	bool				m_rotate; ///< Rotate the text.
	colour				m_helpLines; ///< To show the level.

public:
	static colour		m_defaultColour[8]; ///< Default colours
};
