 /*============================================================================*/
/**  @file       sdl_surface.cpp
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

/*------------- Standard includes --------------------------------------------*/
#include <assert.h>
#include "sdl_surface.h"
#include "sdl_dialog_object.h"
#include <string>

CmyLock CtextSurface::m_lock;
#define SWAP(A,B,TYPE) {TYPE temp=A; A=B; B=temp;}

/*==============================================================================
**              CtextSurface::setColour
**============================================================================*/
///
/// @brief		Change the colour
///
/// @post       Colour correct.
///
/// @remarks    TYPED
///
/*============================================================================*/
void CtextSurface::setColour( colour text, colour back)
{
	m_textColour.r =(Uint8)((text & 0xFF0000)>>16);
	m_textColour.g =(Uint8)((text & 0x00FF00)>>8);
	m_textColour.b =(Uint8)(text & 0x0000FF);
	m_cursorColour.r =(Uint8)((back & 0xFF0000)>>16);
	m_cursorColour.g =(Uint8)((back & 0x00FF00)>>8);
	m_cursorColour.b =(Uint8)(back & 0x0000FF);
}

/*==============================================================================
**              CtextSurface::createSurfaces
**============================================================================*/
///
/// @brief		Calculate surfaces.
///
/// @post       Surfaces per line calculated.
///
/*============================================================================*/
void CtextSurface::createSurfaces( TTF_Font *font)
{
	m_lock.lock();
	m_index =0;
	int lineNr=0;
	int wordNr=0;
	int c=0;
	int w=0;
	int i=0;
	int h=0;
	//SDL_Colour sdc;
	//sdc.r =0; sdc.g =0; sdc.b =64;
	utf8string fit;

	SDL_Surface *bitmap =NULL;
	m_vertical_spacing =1;

	// For loop of all lines.
	for ( lineNr=0; lineNr<(int)m_split.size(); lineNr++)
	{
		// Log.write( "String %d %s", a, m_split[a].c_str());
		// Split one line into words and see how many words do fit...
		CtextSplitter word( m_split[lineNr], " ", "\t");
		if ( word.size() ==0)
		{
			continue;
		}
		fit =word[0];
		TTF_SizeUTF8( font, fit.c_str(), &w, &h);
		if ( w>m_rect.width() && m_rect.width() !=0)
		{
			// Even the first word of line a does not fit!
			fit ="";
			for ( i=0; i<(int)word[0].size(); i++)
			{
				// Check how many characters do fit!
				fit =word[0].sub( 0,i+1);
				TTF_SizeUTF8( font, fit.c_str(), &w, &h);
				if ( w>m_rect.width())
				{
					if ( i==0)
					{
						//Log.write( "Even one character does not fit!!");
						m_lock.unlock();
						return;
					}
					fit =word[0].sub( 0,i);
					bitmap =blend( fit, m_textColour, font, true);
					//bitmap =TTF_RenderUTF8_Blended( font ,fit.c_str(), m_textColour);
					if ( bitmap ==NULL)
					{
						// ( "CtextSurface::CtextSurface  Cannot create a surface to paint on!!");
						// Something wrong in memory, skip the rest.
						break;
					}
					m_height +=bitmap->h+m_vertical_spacing;
					m_index +=fit.size();
					fit =word[0].sub( i);
					m_split.insert( lineNr+1, fit);
					m_surfaces.push_back( bitmap);
					break;
				}
			}
			// Split the first word in a part that fits and a part that doesn't.
#if 0
			if ( i==0)
			{
				break;
			}
			fit =word[0].sub( 0,i);
			bitmap =blend( fit, m_textColour, font, true);
			//bitmap =TTF_RenderUTF8_Blended( font ,fit.c_str(), m_textColour);
			if ( bitmap ==NULL)
			{
				Log.error( "CtextSurface::CtextSurface  Cannot create a surface to paint on!!");
				// Something wrong in memory, skip the rest.
				break;
			}
			m_height +=bitmap->h+m_vertical_spacing;
			fit =word[0].sub( i+1);
			m_split.insert(1, fit);
			m_surfaces.push_back( bitmap);
			m_index +=fit.size();
#endif
			continue;
		}

		bool rendered =false;
		for ( wordNr=1; wordNr<word.size(); wordNr++)
		{
			int length =fit.size();
			fit =utf8string( fit.m_string+" "+(std::string)word[ wordNr]);
			TTF_SizeUTF8( font, fit.c_str(), &w, &h);
			if ( w>m_rect.width() && m_rect.width() !=0)
			{
				// This word doesn't fit anymore.
				fit =fit.sub( 0, length);
				bitmap =blend( fit, m_textColour, font, false);
				//bitmap =TTF_RenderUTF8_Blended( font ,fit.c_str(), m_textColour);
				if ( bitmap ==NULL)
				{
					// ( "CtextSurface::CtextSurface  Cannot create a surface to paint on!!");
					// Something wrong in memory, skip the rest.
					break;
				}
				m_height +=bitmap->h+m_vertical_spacing;
				m_surfaces.push_back( bitmap);
				m_index +=fit.size()+1;
				std::string toinsert;
				for ( c=wordNr; c<(int)word.size(); c++)
				{
					toinsert +=(std::string)word[c]+" ";
				}
				m_split.insert( lineNr+1, toinsert);
				rendered =true;
				break;
			}

		}
		if ( rendered ==false && fit.size()!=0)
		{
			//bitmap =TTF_RenderUTF8_Blended( font ,fit.c_str(), m_textColour);
			bitmap =blend( fit, m_textColour, font, false);
			if ( bitmap ==NULL)
			{
				// ( "CtextSurface::CtextSurface  Cannot create a surface to paint on!!");
				break;
			}
			m_height +=bitmap->h+m_vertical_spacing;
			m_surfaces.push_back( bitmap);
			m_index +=fit.size()+1;
		}
	}
	m_lock.unlock();
}

/*============================================================================*/
///
/// @brief		Render the text-line to one surface.
/// If the cursor is within this text, add a background cursor.
/// If the cursor is at the end of the surface, add a space first.
///
/// @post       One surface calculated.
///
/*============================================================================*/
SDL_Surface * CtextSurface::blend( utf8string &text, SDL_Color fg, TTF_Font *font, bool split)
{
	m_lock.lock();
	try
	{
	SDL_Surface *bitmap =NULL;
	int size =text.size();
	if ( split ==true) size--;
	int w1, h1, w2, h2;

	if ( m_cursor-m_index >=0 && m_cursor-m_index<=size)
	{
		// Blend with cursor.
		if ( m_cursor-m_index ==size)
		{
			text.push_back( " ");
		}

		bitmap =( m_blend) ? TTF_RenderUTF8_Blended( font , text.c_str(), fg):
							 TTF_RenderUTF8_Solid( font, text.c_str(), fg);
		int tcol=SDL_MapRGBA( bitmap->format, fg.r, fg.g, fg.b, 0);
		// Find out start position cursor.
		std::string t;
		if ( m_cursor-m_index ==0)
		{
			w1=0;
		}
		else
		{
			t =text.sub( 0, m_cursor-m_index);
			TTF_SizeUTF8( font, t.c_str(), &w1, &h1);
		}
		// Find out end position cursor.
		t =text.sub( 0, m_cursor-m_index+1);
		TTF_SizeUTF8( font, t.c_str(), &w2, &h2);
		if ( w2==w1) { w1=w1-1; }

		SDL_LockSurface( bitmap);
		int xx, yy;

		int *pix =(int*)(bitmap->pixels);
		int colr =SDL_MapRGB( bitmap->format, m_cursorColour.r, m_cursorColour.g, m_cursorColour.b);
		for ( yy =h2/5; yy<h2*7/8; yy++)
		for ( xx =w1; xx<w2; xx++)
		{
			int index=xx+yy*(bitmap->pitch/4);
			int pp=pix[index];
			if ( pp ==tcol)
			{
				pix[index] =colr;
			}
		}
		SDL_UnlockSurface( bitmap);
		m_lock.unlock();
		return bitmap;
	}
	// Blend without cursor.
	bitmap =( m_blend) ? TTF_RenderUTF8_Blended( font , text.c_str(), fg):
						 TTF_RenderUTF8_Solid( font, text.c_str(), fg);
	m_lock.unlock();
	return bitmap;
	}
	catch (...)
	{
		// ("Serious bitmap error!!");
	}
	m_lock.unlock();
	return NULL;
}

/*==============================================================================
**              CtextSurface::createSurface
**============================================================================*/
///
/// @brief		Calculate a surface.
///
/// @post       Surface calculated.
///
/// @remarks    TYPED
///
/*============================================================================*/
void CtextSurface::createSurface()
{
 //   int bmask = 0x000000ff;
//    int gmask = 0x0000ff00;
//    int rmask = 0x00ff0000;
////    int amask = 0xff000000;
//	m_surface =SDL_CreateRGBSurface( SDL_SWSURFACE,
//									 m_rect.width(), m_rect.height(), 32,
//									 rmask, gmask, bmask, amask);
	//SDL_SetAlpha(m_surface,  SDL_SRCALPHA, SDL_ALPHA_TRANSPARENT);
	//SDL_SetAlpha(m_surface, SDL_SRCALPHA|SDL_RLEACCEL, 255);
	//SDL_SetAlpha(m_surface,  0, 0);
}

/*==============================================================================
**              CtextSurface::CtextSurface
**============================================================================*/
///
/// @brief		Calculate a surface.
///
/// @post       Surface calculated.
///
/// @remarks    TYPED
///
/*============================================================================*/
CtextSurface::CtextSurface( std::shared_ptr<Cgraphics> graphics,
		                    const std::string &text,
		                    const Crect &rect,
		                    int cursor,
		                    Egravity gravity,
		                    colour textColour,
		                    colour cursorColour,
		                    TTF_Font *font)
: m_height(0)
, m_gravity( gravity)
, m_split( text, ",", "\n")
, m_rect(rect)
, m_graphics( graphics)
, m_cursor(cursor)
, m_owner(false)
, m_blend(true)
{
	(void)cursor;
	setColour( textColour, cursorColour);
	std::string fit;
	// No cursor for now...
	// Is the cursor in the current line?
	//createSurface();

	//if ( m_surface ==NULL)
	//{
	///	Log.error( "CtextSurface::CtextSurface  Cannot create a final surface to paint on!!");
		// Something wrong in memory, should skip the rest.
	//}

	createSurfaces( font);
	renderSurfaces();
	clean();
}

/*==============================================================================
**              CtextSurface::renderSurfaces
**============================================================================*/
///
/// @brief		Render the text-lines together to one video memory.
///
/// @post       Final surface calculated.
///
/*============================================================================*/
void CtextSurface::renderSurfaces()
{
	m_lock.lock();
	// All lines are calcuated, now the display.
	int top_offset =0;
	if ( m_rect.height()>m_height)
	{
		switch ( m_gravity)
		{
		case GRAVITY_CENTER:
		case GRAVITY_LEFT_CENTER:
		case GRAVITY_RIGHT_CENTER:
			top_offset =( m_rect.height()-m_height)/2;
			break;

		case GRAVITY_BOTTOM_CENTER:
		case GRAVITY_LEFT_BOTTOM:
		case GRAVITY_RIGHT_BOTTOM:
			top_offset =(m_rect.height()-m_height);
			break;
		default:
			top_offset =0;
			break;
		}
	}
	int pixels_left =m_rect.height()-top_offset;

	for ( std::vector<SDL_Surface*>::iterator a=m_surfaces.begin(); a!=m_surfaces.end(); ++a)
	{
		SDL_Surface *s =*a;
		int x_offset =0;
		switch ( m_gravity)
		{
		case GRAVITY_RIGHT_CENTER:
		case GRAVITY_RIGHT:
		case GRAVITY_RIGHT_BOTTOM:
			x_offset =m_rect.width()-s->w;
			break;
		case GRAVITY_CENTER:
		case GRAVITY_TOP_CENTER:
		case GRAVITY_BOTTOM_CENTER:
			x_offset =(m_rect.width()-s->w)/2;
			break;
		case GRAVITY_LEFT_BOTTOM:
		case GRAVITY_LEFT_CENTER:
		default:
			x_offset =0;
			break;
		}
		int hblit =( pixels_left >=s->h) ? s->h:pixels_left;
		if (hblit >0)
		{
			m_graphics->renderSurface( *a, m_rect.left()+x_offset, m_rect.top()+top_offset);
			pixels_left -=hblit;
			top_offset +=hblit+m_vertical_spacing;
		}
	}
	m_lock.unlock();
}

/*==============================================================================
**              CtextSurface::~CtextSurface
**============================================================================*/
///
/// @brief		Destructor.
///
/*============================================================================*/
CtextSurface::~CtextSurface()
{
	clean();
}

void CtextSurface::clean()
{
	for ( int a=0; a<(int)m_surfaces.size(); a++)
	{
		SDL_FreeSurface( m_surfaces[a]);
	}
	m_surfaces.clear();
}

/** @brief		Create text surface.
 *  @param      text [in] What to write
 *  @param		font [in] Which font to use
 *  @param 		size [in] Size of the surface
 *  @param      gravity [in] What gravity to use
 */
CtextSurface::CtextSurface( const std::string &text,
		                    TTF_Font *font,
		                    const Csize &size,
		                    Egravity gravity
		                    )
: m_height( size.height())
, m_gravity( gravity)
, m_split( text, ",", "\n")
, m_rect(0,0,0,0) // unused.
, m_vertical_spacing(1)
, m_graphics(NULL)
, m_index(0)
, m_cursor(-1)
, m_owner(false)
, m_blend(false)
{
	m_textColour.b =8;
	m_textColour.r =8;
	m_textColour.g =8;
#ifndef USE_SDL2
	m_textColour.unused =128;
#endif
	createSurfaces( font);
	createGraphics();
	renderSurfaces();
}

SDL_Surface *CtextSurface::first()
{
	if (m_surfaces.size()>0)
	{
		return m_surfaces[0];
	}
	return NULL;
}

/** Create a graphic surface, mainly for printing text.
 */
void CtextSurface::createGraphics()
{
	int wdth =0;
	int hght =0;
	int a;
	for ( a=0; a<(int)m_surfaces.size(); a++)
	{
		Csize sz(m_surfaces[a]->w, m_surfaces[a]->h);
		hght +=sz.height();
		if ( a) hght +=m_vertical_spacing;
		if ( wdth<sz.width())
		{
			wdth =sz.width();
		}
	}
	m_graphics =NULL;
	m_rect =Crect(0,0, wdth, hght);
	if ( wdth>0 && hght>0)
	{
		m_graphics =std::make_shared<Cgraphics>( Csize( wdth, hght), false);
		m_graphics->init();
		m_owner =true;
	}
}

/* set pixel in graph.
 *          XXX
 *         XxxXX
 *        XxxxxXX
 *        Xxx0xXX
 *        XXxxXXX
 *         XXXXX
 *          XXX
 * */
void CtextSurface::pixel( SDL_Surface *surface, const Cpoint &pixel)
{
	int dx[]={ -3,-3,-3, -2,-2,-2,-2,-2, -1,-1,-1,-1,-1,-1,-1, 0,0,0,0,0,0,0,    1,1,1,1,1,1,1,    2,2,2,2,2,   3,3,3  };
	int dy[]={ -1, 0, 1, -2,-1, 0, 1, 2, -3,-2,-1, 0, 1, 2, 3, -3,-2,-1,0,1,2,3, -3,-2,-1,0,1,2,3, -2,-1,0,1,2, -1,0,1  };

	Uint32 *pixels =(Uint32*)surface->pixels;
	for ( int n=0; n<37; n++)
	{
		int xx=pixel.x+dx[n];
		int yy=pixel.y+dy[n];
		if ( xx>=0 && xx<surface->pitch && yy>=0 && yy<surface->h)
		{
			int offs =yy*surface->w+xx;
			pixels[offs] =0;
		}
	}
}

/** Set a line. */
void CtextSurface::line( SDL_Surface *surface, const Cpoint &from, const Cpoint &to)
{
	//Log.write( "Cgraphics::line");
	int x1=from.x, x2=to.x, y1=from.y, y2=to.y;
	int dx = abs( x1-x2), dy = abs( y1-y2), p;
	int inc;
    if (dx >= dy)
	{
		p = 2 * dy - dx;
		if (x1 >= x2)
		{
			SWAP(x1, x2, int)
			SWAP(y1, y2, int)
		}
		pixel( surface, Cpoint( x1,y1));
		if (y2 >= y1)
			inc = 1;
		else
			inc = -1;

		while (x1 < x2)
		{
			x1++;
			if (p < 0)
				p += 2 * dy;
			else
			{
				y1 += inc;
				p += 2 * (dy - dx);
			}
			pixel( surface, Cpoint( x1,y1));
		}
	}
	else
	{
		p = 2 * dx - dy;
		if (y1 >= y2)
		{
			SWAP(x1, x2, int)
			SWAP(y1, y2, int)
		}
		pixel( surface, Cpoint( x1,y1));

		if (x2 >= x1)
			inc = 1;
		else
			inc = -1;

		while (y1 < y2)
		{
			y1++;
			if (p < 0)
				p += 2 * dx;
			else
			{
				x1 += inc;
				p += 2 * (dx - dy);
			}
			pixel( surface, Cpoint( x1,y1));
		}
	}
}
