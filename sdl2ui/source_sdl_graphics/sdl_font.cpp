/*============================================================================*/
/**  @file       sdl_font.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Default dialog.
 **
 **  Use fonts for our program
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CtextFont
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
#include "sdl_font.h"
#include "sdl_graphics.h"

/// @brief List of all fonts.
std::vector<SsingleFont> CtextFont::m_fonts;

SsingleFont emptyFont =
		{ "", 0, NULL };

/// @brief Find a single font.
SsingleFont CtextFont::findFont(const std::string &fontName, int fontSize)
{
	for ( int a=0; a<(int)m_fonts.size(); a++)
	{
		if (m_fonts[a].fontSize == fontSize && m_fonts[a].fontName == fontName)
		{
			return m_fonts[a];
		}
	}
	std::string s=Cgraphics::m_defaults.font_path + fontName;
    if (!TTF_WasInit()) TTF_Init(); // Initilize SDL_ttf
	TTF_Font *font =TTF_OpenFont(s.c_str(), fontSize);
	if (!font)
	{
        return emptyFont;
	}
	SsingleFont sf;
	sf.font =font;
	sf.fontName = fontName;
	sf.fontSize = fontSize;
	m_fonts.push_back( sf);
	return sf;
}

/// @brief Construct a font.
CtextFont::CtextFont( std::string localFontName, int localFontSize,
					  std::string chineseFontName, int chineseFontSize)
{
	if ( localFontSize == 0)
	{
		localFontSize = chineseFontSize;
		localFontName = chineseFontName;
	}
	m_font.local =findFont(localFontName, localFontSize);
	if ( chineseFontName == "" || chineseFontSize == 0)
	{
		m_font.chinese = m_font.local;
	}
	else
	{
		m_font.chinese =findFont( chineseFontName, chineseFontSize);
	}
}

TTF_Font *CtextFont::font()
{
	return m_font.local.font;
}

CtextFont::CtextFont( const char *fname, bool relative_to_screen)
{
	std::string name(fname);
	std::string chineseFontName;
	std::string localFontName;
	int chineseFontSize = 0;
	int localFontSize =0;
	if ( Cgraphics::m_defaults.get_font)
	{
		Cgraphics::m_defaults.get_font( name, localFontName, chineseFontName, localFontSize, chineseFontSize, relative_to_screen);
	}
	if ( chineseFontName =="")
	{
		chineseFontName =localFontName;
	}
	if (localFontSize == 0)
	{
		localFontSize = chineseFontSize;
		localFontName = chineseFontName;
	}
	if (chineseFontSize ==0)
	{
		chineseFontSize = localFontSize;
		chineseFontName = localFontName;
	}
	m_font.local =findFont(localFontName, localFontSize);
	m_font.chinese =findFont(chineseFontName, chineseFontSize);
}

/// @brief Free all fonts to prevent memory leak.
void CtextFont::closeFonts()
{
	//Log.write( "CtextFont::closeFonts  Closing.");
	for (auto font : m_fonts)
	{
		TTF_CloseFont( font.font);
	}
	m_fonts.clear();
	if (TTF_WasInit()) TTF_Quit(); // Deinitilize SDL_ttf
}


