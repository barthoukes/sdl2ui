 /*============================================================================*/
/**  @file       sdl_surface.h
 **  @ingroup    sdl2ui
 **  @brief		 Create surface for text
 **
 **  Align text in many ways inside a rectangle, break into words, add cursor...
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CtextSurface
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
#include "sdl_rect.h"
#include "sdl_graphics.h"
#include "sdl_dialog_object.h"
#include "SDL_video.h"
#include "SDL_ttf.h"
#include <string>
#include <vector>
#include "text_splitter.hpp"
#include "my_thread.hpp"

/// @brief Create a text surface for printing, display etc.
class CtextSurface
{
public:
	// For screen
	CtextSurface( Cgraphics *graphics,
			      const std::string &text,
			      const Crect &rect,
			      int cursor,
			      Egravity gravity,
			      colour textColour,
			      colour cursorColour,
			      TTF_Font *font,
				  bool renderIfFit);

	// For printer
	CtextSurface( const std::string &text,
			      TTF_Font *font,
			      const Csize &size,
			      Egravity gravity);

	virtual ~CtextSurface();
	int width() { return m_graphics ? m_graphics->width():0; }
	Cgraphics *graphics() { return m_graphics; }
	static void line( SDL_Surface *surface, const Cpoint &from, const Cpoint &to);
	static void pixel( SDL_Surface *surface, const Cpoint &pixel);

private:
	void setColour( colour text, colour back);
	void clean();
	void createSurfaces( TTF_Font *font);
	void renderSurfaces();
	SDL_Surface * blend( utf8string &text, SDL_Color fg, TTF_Font *font, bool split);
	void setCursor( int cursor) { m_cursor =cursor; }
	// For printer.
	void calculateSize( TTF_Font *font);
	void createGraphics();
	bool isFit();

public:
	SDL_Surface *first();
	bool doesNotFit() { return(!m_doesFit); }

private:
	SDL_Colour   m_textColour; ///< Font color (R,G,B)
	SDL_Colour   m_cursorColour; ///< Colour cursor.
	std::vector<SDL_Surface*> m_surfaces; ///< One surface per line.
	int			 m_height; ///< height for text.
	Egravity	 m_gravity; ///< Align text.
	CtextSplitter    m_split; ///< Text split into lines.
	Crect		 m_rect; ///< Size of graphic.
	int			 m_vertical_spacing; ///< Spacing vertical.
	Cgraphics	 *m_graphics;
	int			 m_index;
	int 		 m_cursor;
	bool		 m_owner; ///< Should I remove the graphics in the end?
	bool 		 m_blend;
	static CmyLock m_lock;
	bool         m_doesFit;
};

/* TEXT_SURFACE_H_ */
