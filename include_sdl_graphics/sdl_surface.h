/*
 * text_surface.h
 *
 *  Created on: 9 apr. 2012
 *      Author: mensfort
 */

#pragma once

#include "sdl_rect.h"
#include "sdl_graphics.h"
#include "sdl_dialog_object.h"
#include "SDL_video.h"
#include "SDL_ttf.h"
#include <string>
#include <vector>
#include "text_splitter.h"
#include "my_thread.h"

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
			      TTF_Font *font);

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
	void createSurface();
	void renderSurfaces();
	SDL_Surface * blend( utf8string &text, SDL_Color fg, TTF_Font *font, bool split);
	void setCursor( int cursor) { m_cursor =cursor; }
	// For printer.
	void calculateSize( TTF_Font *font);
	void createGraphics();

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
};

/* TEXT_SURFACE_H_ */
