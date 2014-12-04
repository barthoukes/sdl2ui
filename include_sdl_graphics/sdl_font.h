/*
 * text_font.h
 *
 *  Created on: 9 apr. 2012
 *      Author: mensfort
 */

#pragma once

#include "SDL_ttf.h"
#include <vector>
#include <string>
//#include "zhongcan_defines.h"

/// @brief structure to put loaded fonts in an array for quick access.
typedef struct
{
	std::string 	name;
	int				pixels;
	TTF_Font		*font;
} SsingleFont;

/// @brief structure to put loaded fonts in an array for quick access.
typedef struct
{
	SsingleFont		local;
	SsingleFont		chinese;
} Sfont;

/// @brief Font loader.
class CtextFont
{
public:
	CtextFont( std::string localFontName, int localPixels,
			  std::string chineseFontName, int chinesePixels); // FONT_GKAI00, 24
	CtextFont( const char *name);
	virtual ~CtextFont() {}
	operator Sfont () { return m_font; }
	TTF_Font *font();
	static void closeFonts();

private:
	static SsingleFont findFont( const std::string &fontName, int pixels);

	static std::vector<SsingleFont> m_fonts;
	Sfont  m_font;
};

/* TEXT_FONT_H_ */
