/*
 * text_font.cpp
 *
 *  Created on: 9 apr. 2012
 *      Author: mensfort
 */

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
	//Log.write( "CtextFont::findFont  %s %d", fontName.c_str(), pixels);
	std::string s=Cgraphics::m_defaults.font_path + fontName;
    if (!TTF_WasInit()) TTF_Init(); // Initilize SDL_ttf
	TTF_Font *font =TTF_OpenFont( s.c_str(), pixels);
	if (!font)
	{
		// Cannot find font: %s %d !!", s.c_str(), pixels);
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

CtextFont::CtextFont( const char *fname)
{
	std::string name(fname);
	std::string chineseFontName;
	std::string localFontName;
	int chinesePixels;
	int localPixels;
	if ( Cgraphics::m_defaults.get_font)
	{
		Cgraphics::m_defaults.get_font( name, localFontName, chineseFontName, localPixels, chinesePixels);
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


