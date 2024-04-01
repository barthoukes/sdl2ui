/*============================================================================*/
/**  @file       sdl_hand_writer.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Create characters by handwriting.
 **
 **  Create a default image.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              ChandWriter
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
#include "sdl_hand_writer.h"
#include "sdl_surface.h"
#include "sdl_dialog_event.h"

/*============================================================================*/
///
/// @brief Constructor.
//
/// @param x [in] Position.
/// @param y [in] Position.
/// @param width [in] Position.
/// @param height [in] Position.
///
/*============================================================================*/
ChandWriter::ChandWriter( Cdialog *parent, const Crect &rect, const std::string &model, int distance)
: Cimage( parent, rect, KEY_NONE, "", BORDER_NONE, 0)
, m_stroke(0)
, m_distance(1000)
, m_started(false)
, m_lastPoint(0,0)
, m_recognizer( NULL)
, m_character( NULL)
, m_result( NULL)
, m_minimum_distance(distance)
{
	int options =SDL_SWSURFACE; //|SDL_NOFRAME;
	//options =SDL_SWSURFACE|SDL_NOFRAME;
    Uint32 rmask, gmask, bmask, amask;
    setGravity(GRAVITY_LEFT);
    //m_backgroundColour =Cgraphics::m_defaults.handwriting_background;
    m_background.setColours( Cgraphics::m_defaults.handwriting_background, Cgraphics::m_defaults.handwriting_background);
    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
	#if 0 //SDL_BYTEORDER ==SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		bmask = 0x000000ff;
		gmask = 0x0000ff00;
		rmask = 0x00ff0000;
		amask = 0; //0xff000000;
	#endif

	if ( Cgraphics::m_defaults.handwriting_detection_enabled)
	{
		m_surface =SDL_CreateRGBSurface( options, m_rect.width()*8, m_rect.height()*8,
										 ::m_mainGraph->bitsPerPixel(),
										 rmask, gmask, bmask, amask);
		m_recognizer = zinnia_recognizer_new();

		if (!zinnia_recognizer_open( m_recognizer, model.c_str()))
		{
		    //"ChandWriter::ChandWriter  ERROR: %s\n", zinnia_recognizer_strerror( m_recognizer));
			return;
		}
		m_character  = zinnia_character_new();
	 }
	 //zinnia_character_clear( m_character);
	 //zinnia_character_set_width( m_character, m_rect.width());
	 //zinnia_character_set_height( m_character, m_rect.height());
	 clearImage();
	 enablePainting(); //enableDrag();
}

/** @brief  Clear the handwriter background. */
void ChandWriter::clearImage()
{
	if ( Cgraphics::m_defaults.handwriting_detection_enabled)
	{
		SDL_Rect rect;
		rect.h =(Uint16)m_surface->h;
		rect.w =(Uint16)m_surface->w;
		rect.x =0;
		rect.y =0;
		SDL_FillRect( m_surface, &rect, m_background.getColour());
		m_stroke =0;
		m_index =0;
		m_lastPoint =Cpoint(-5,-100);
		if ( m_character)
		{
			zinnia_character_clear( m_character);
			zinnia_character_set_width( m_character, m_rect.width()*8);
			zinnia_character_set_height( m_character, m_rect.height()*8);
		}
	}
}

/*============================================================================*/
///
/// @brief Destructor.
///
/*============================================================================*/
ChandWriter::~ChandWriter()
{
	close();
	if ( Cgraphics::m_defaults.handwriting_detection_enabled)
	{
		if (m_character)
		{
			zinnia_character_destroy( m_character);
			m_character = NULL;
		}
		if (m_recognizer)
		{
			zinnia_recognizer_destroy( m_recognizer);
			m_recognizer = NULL;
		}
	}
}

/** @brief We drag our finger on the screen to another location.
 *  @param p [in] New location of our finger.
 *  @return true on success.
 */
bool ChandWriter::onPaintingMove( const Cpoint &point)
{
	Cpoint p =point-(m_rect.leftTop()*8);
	if ( m_surface ==NULL)
	{
		return false;
	}
	if ( !m_started)
	{
		m_started =true;
		addPoint( p.x, p.y);
		CtextSurface::pixel( m_surface, p);
		m_lastPoint =p;
		m_index =1;
	}
	else
	{
		// Not for small distances.
		if ( p.distance( m_lastPoint) >m_minimum_distance && ++m_index<1000)
		{
			// New line detected.
			addPoint( p.x, p.y);
			CtextSurface::line( m_surface, m_lastPoint, p);
			// pixel(%d,%d) to (%d,%d)", m_lastPoint.x, m_lastPoint.y, p.x, p.y);
			m_lastPoint =p;
		}
	}
	Cimage::onPaint(0);
	onUpdate();
	return true;
}

/** @brief Start with next stroke */
bool ChandWriter::onPaintingStart( const Cpoint &p)
{
	m_started =false;
	if ( m_result)
	{
		zinnia_result_destroy( m_result);
		m_result =NULL;
	}
	//m_started =false;
	onPaintingMove( p);
	return true;
}

/** @brief Drag finger on screen, now release your finger.
 *  @param p [in] Point where we release the finger.
 *  @return true when correct.
 */
bool ChandWriter::onPaintingStop( const Cpoint &point)
{
	if ( m_surface ==NULL)
	{
		return false;
	}
	//p =CdialogEvent::Instance()->lastMouse();
	Cpoint p =point-(m_rect.leftTop()*8);
	addPoint( p.x, p.y);
	//zinnia_character_add( m_character, m_stroke, p.x, p.y);
	CtextSurface::line( m_surface, m_lastPoint, p);
	m_started =false;
	m_stroke++;
	m_lastPoint =Cpoint(-100,-100);
	if ( Cgraphics::m_defaults.handwriting_detection_enabled)
	{
		if ( m_result)
		{
			zinnia_result_destroy( m_result);
			m_result =NULL;
		}
		m_result = zinnia_recognizer_classify( m_recognizer, m_character, 50);
	}
	return true;
}

/** @brief Add a point to the symbol line.
 *  @param x [in] X-coordinate of mouse.
 *  @param y [in] Y-coordinate of mouse.
 */
void ChandWriter::addPoint( int x, int y)
{
	if ( Cgraphics::m_defaults.handwriting_detection_enabled)
	{
		zinnia_character_add( m_character, m_stroke, x,y);
	}
}

/** @brief Paint, empty because we don't want to drag this one.
 */
void ChandWriter::onPaint( const Cpoint &p)
{
	// prevent painting.
	(void)p;
}

/** @brief After recognizing 14 symbols, we want to get number N in the list.
 *  @param offset [in] Which one 0..13
 *  @return String with that specific character.
 */
std::string ChandWriter::get( size_t offset)
{
	if ( m_result ==NULL || zinnia_result_size( m_result)<=offset)
	{
		return "";
	}
	if ( Cgraphics::m_defaults.handwriting_detection_enabled)
	{
		const char *s= zinnia_result_value( m_result, offset);
		//Log.write( "zinnia %d = %s", offset, s);
		return s;
	}
	return "";
}
