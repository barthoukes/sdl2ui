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
SsingleFont CtextFont::findFont( const std::string &fontName, int pixels)
{
	for ( int a=0; a<(int)m_fonts.size(); a++)
	{
		if ( m_fonts[a].pixels ==pixels && m_fonts[a].name ==fontName)
		{
			return m_fonts[a];
		}
	}
	std::string s=Cgraphics::m_defaults.font_path + fontName;
    if (!TTF_WasInit()) TTF_Init(); // Initilize SDL_ttf
	TTF_Font *font =TTF_OpenFont( s.c_str(), pixels);
	if (!font)
	{
		const char *err = TTF_GetError();
        return emptyFont;
	}
	SsingleFont sf;
	sf.font =font;
	sf.name =fontName;
	sf.pixels =pixels;
	m_fonts.push_back( sf);
	return sf;
}

/// @brief Construct a font.
CtextFont::CtextFont( std::string localFontName, int localPixels,
					  std::string chineseFontName, int chinesePixels)
{
	if ( localPixels ==0)
	{
		localPixels =chinesePixels;
		localFontName =chineseFontName;
	}
	m_font.local =findFont( localFontName, localPixels);
	if ( chineseFontName =="" || chinesePixels ==0)
	{
		m_font.chinese =m_font.local;
	}
	else
	{
		m_font.chinese =findFont( chineseFontName, chinesePixels);
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
	int chinesePixels =0;
	int localPixels =0;
	if ( Cgraphics::m_defaults.get_font)
	{
		Cgraphics::m_defaults.get_font( name, localFontName, chineseFontName, localPixels, chinesePixels, relative_to_screen);
	}
	if ( chineseFontName =="")
	{
		chineseFontName =localFontName;
	}
	if ( localPixels ==0)
	{
		localPixels =chinesePixels;
		localFontName =chineseFontName;
	}
	if ( chinesePixels ==0)
	{
		chinesePixels =localPixels;
		chineseFontName =localFontName;
	}
	m_font.local =findFont( localFontName, localPixels);
	m_font.chinese =findFont( chineseFontName, chinesePixels);
}

/// @brief Free all fonts to prevent memory leak.
void CtextFont::closeFonts()
{
	//Log.write( "CtextFont::closeFonts  Closing.");
	for ( int a=0; a<(int)m_fonts.size(); a++)
	{
		TTF_CloseFont( m_fonts[a].font);
		m_fonts[a].name ="";
		m_fonts[a].pixels =0;
		m_fonts[a].font =NULL;
	}
	m_fonts.clear();
	if (TTF_WasInit()) TTF_Quit(); // Deinitilize SDL_ttf
}


