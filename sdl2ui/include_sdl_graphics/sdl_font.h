/*
 * text_font.h
 *
 *  Created on: 9 apr. 2012
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
#include "SDL_ttf.h"
#include <vector>
#include <string>

/// @brief structure to put loaded fonts in an array for quick access.
typedef struct
{
public:
	std::string 	fontName;
	int				fontSize;
	TTF_Font		*font;
} SsingleFont;

/// @brief structure to put loaded fonts in an array for quick access.
typedef struct
{
public:
	SsingleFont		local;
	SsingleFont		chinese;
} Sfont;

/// @brief Font loader.
class CtextFont
{
public:
	CtextFont(std::string localFontName, int localFontSize,
			  std::string chineseFontName, int chineseFontSize); // FONT_GKAI00, 24
	CtextFont( const char *name, bool relative_to_screen=true);
	virtual ~CtextFont() {}
	operator Sfont () { return m_font; }
	TTF_Font *font();
	static void closeFonts();

protected:
	static SsingleFont findFont( const std::string &fontName, int fontSize);

private:

	static std::vector<SsingleFont> m_fonts;
	Sfont  m_font;

	friend class Ctext;
};

/* TEXT_FONT_H_ */
