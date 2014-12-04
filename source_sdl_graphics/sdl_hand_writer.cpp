/*============================================================================*/
/**  @file       sdl_hand_writer.cpp
 **  @ingroup    zhongcan_sdl
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
 **  Copyright (c) Bart Houkes, 23 sep 2013
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

/*------------- Standard includes --------------------------------------------*/

#define USE_ZINNIA

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
ChandWriter::ChandWriter( Cdialog *parent, const Crect &rect)
: Cimage( parent, rect, KEY_NONE, "", BORDER_NONE, 0, "")
, m_stroke(0)
, m_distance(1000)
, m_started(false)
, m_lastPoint(0,0)
, m_recognizer( NULL)
, m_character( NULL)
, m_result( NULL)
{
	int options =SDL_SWSURFACE; //|SDL_NOFRAME;
	//options =SDL_SWSURFACE|SDL_NOFRAME;
    Uint32 rmask, gmask, bmask, amask;

    m_backgroundColour =Cgraphics::m_defaults.handwriting_background;
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
#ifdef USE_ZINNIA
		m_recognizer = zinnia_recognizer_new();

		if (!zinnia_recognizer_open( m_recognizer, "/home/mensfort/zhongcan/font/handwriting-zh_CN.model"))
		{
		    //"ChandWriter::ChandWriter  ERROR: %s\n", zinnia_recognizer_strerror( m_recognizer));
			return;
		}

		  m_character  = zinnia_character_new();
#endif
	 }
	 //zinnia_character_clear( m_character);
	 //zinnia_character_set_width( m_character, m_rect.width());
	 //zinnia_character_set_height( m_character, m_rect.height());
	 clearImage();
	 enableDrag();
}

/** @brief  Clear the handwriter background. */
void ChandWriter::clearImage()
{
	if ( Cgraphics::m_defaults.handwriting_detection_enabled)
	{
		SDL_Rect rect;
		rect.h =m_surface->h;
		rect.w =m_surface->w;
		rect.x =0;
		rect.y =0;
		SDL_FillRect( m_surface, &rect, m_backgroundColour);
		m_stroke =0;
		m_index =0;
		m_lastPoint =Cpoint(-5,-100);
#ifdef USE_ZINNIA
		if ( m_character)
		{
			zinnia_character_clear( m_character);
			zinnia_character_set_width( m_character, m_rect.width()*8);
			zinnia_character_set_height( m_character, m_rect.height()*8);
		}
#endif
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
#ifdef USE_ZINNIA
	if ( Cgraphics::m_defaults.handwriting_detection_enabled)
	{
		zinnia_character_destroy( m_character);
		zinnia_recognizer_destroy( m_recognizer);
	}
#endif
}

/** @brief We drag our finger on the screen to another location.
 *  @param p [in] New location of our finger.
 *  @return true on success.
 */
bool ChandWriter::onDrag( Cpoint p)
{
	p =CdialogEvent::Instance()->lastMouse();
	p =p-(m_rect.leftTop()*8);
	if ( m_surface ==NULL)
	{
		return false;
	}
	if ( !m_started)
	{
		m_started =true;
		addPoint( p.x, p.y);
		//zinnia_character_add( m_character, m_stroke, p.x, p.y);
		CtextSurface::pixel( m_surface, p);
		m_lastPoint =p;
		m_index =1;
	}
	else
	{
		// Not for small distances.
		if ( p.distance( m_lastPoint) >50 && p.distance( m_lastPoint)<50*50 && ++m_index<1000)
		{
			// New line detected.
			addPoint( p.x, p.y);
			CtextSurface::line( m_surface, m_lastPoint, p);
			// pixel(%d,%d) to (%d,%d)", m_lastPoint.x, m_lastPoint.y, p.x, p.y);
			m_lastPoint =p;
		}
	}
	return true;
}

/** @brief Drag finger on screen, now release your finger.
 *  @param p [in] Point where we release the finger.
 *  @return true when correct.
 */
bool ChandWriter::onDragEnd( Cpoint p)
{
	if ( m_surface ==NULL)
	{
		return false;
	}
	p =CdialogEvent::Instance()->lastMouse();
	p =p-(m_rect.leftTop()*8);
	addPoint( p.x, p.y);
	//zinnia_character_add( m_character, m_stroke, p.x, p.y);
	CtextSurface::line( m_surface, m_lastPoint, p);
	m_started =false;
	m_stroke++;
	m_lastPoint =Cpoint(-100,-100);
#ifdef USE_ZINNIA
	if ( Cgraphics::m_defaults.handwriting_detection_enabled)
	{
		if ( m_result)
		{
			zinnia_result_destroy( m_result);
			m_result =NULL;
		}
		m_result = zinnia_recognizer_classify( m_recognizer, m_character, 14);
	}
#endif
	return false;
}

/** @brief Add a point to the symbol line.
 *  @param x [in] X-coordinate of mouse.
 *  @param y [in] Y-coordinate of mouse.
 */
void ChandWriter::addPoint( int x, int y)
{
	if ( Cgraphics::m_defaults.handwriting_detection_enabled)
	{
		// ChandWriter::addPoint %d - %d  %d", m_stroke, x,y);
#ifdef USE_ZINNIA
		zinnia_character_add( m_character, m_stroke, x,y);
#endif
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
#ifdef USE_ZINNIA

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
#endif
	return "";
}
