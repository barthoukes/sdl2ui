/*============================================================================*/
/**  @file       sdl_swype_dialog.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Swype dialog 1D list
 **
 **  Create a default swype list 1 dimensional.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CswypeDialog
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
#include "sdl_swype_dialog.h"
#include "sdl_surface.h"
#include "sdl_graphics.h"

/** Dialog to scroll objects left-right or up-down */
CswypeDialog::CswypeDialog( Cdialog *parent, const Crect &rect, keybutton firstKey, bool horizontal)
: Cdialog( parent, "scrollRect", rect)
, m_itemRect( 0,0,rect.width(),itemBlocks())
, m_listSize(0)
, m_horizontal(horizontal)
, m_sizes(0)
, m_scroll(0)
, m_speed(0.0f)
, m_validBuffers(0)
, m_visibleBuffers(0)
, m_firstUnitPainted(0)
, m_lastUnitPainted(0)
, m_firstVisibleUnit(0)
, m_lastVisibleUnit(0)
, m_visibleSize(1)
, m_firstOptionalUnit(0)
, m_lastOptionalUnit(0)
, m_nrFkeys((firstKey==KEY_F1) ? 12:16)
, m_firstKey(firstKey)
, m_endMargin(0)
, m_itemBlocks( 6)
, m_startPosition(0)
, m_endPosition(0)
, m_moveTime(0)
, m_dialogSpeed(0)
, m_repaint(false)
, m_dragEnable(0)
, m_dragIndex(0)
, m_object(NULL)
, m_cursor(0)
{
	m_object =new CscrollObject( this, 0, Csize(m_rect.width(), itemBlocks()));

	m_graphics =parent->graphics();
	m_listSize =(3*m_rect.height())/itemBlocks();
	calculateSurfacePosition();
	setRect( rect);
}

/** @brief Change or init the rectangle to this size.
 *  @param rect [in] What position and size to use.
 */
void CswypeDialog::setRect( const Crect &rect)
{
	m_rect.setLeft( rect.left());
	m_rect.setTop( rect.top());
	int py =rect.height()*8;
	int px =rect.width()*8;

	if ( m_rect.width() !=rect.width() || m_rect.height() !=rect.height()
	   || m_graphics ==NULL)
	{
		m_rect =rect;
		if ( m_horizontal)
		{
			int pixelsReserved=3*m_rect.width()*8+itemBlocks()*8-1;
			m_endMargin =itemBlocks()*1*8;
			pixelsReserved =pixelsReserved -(pixelsReserved%itemBlocks());
		   /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
		       as expected by OpenGL for textures */
			px *=3;
			px =px-(px%(itemBlocks()*8));
		}
		else
		{
			int pixelsReserved=3*m_rect.height()*8+itemBlocks()*8-1;
			m_endMargin =itemBlocks()*1*8;
			pixelsReserved =pixelsReserved -(pixelsReserved%itemBlocks());
		   /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
		       as expected by OpenGL for textures */
			py *=3;
			py =py-(py%(itemBlocks()*8));
		}
	    m_graphics =std::make_shared<Cgraphics>(Csize(px, py), false);
	    m_graphics->init();
	}
}

CswypeDialog::~CswypeDialog()
{
	clean();
	if ( m_object)
	{
		delete m_object;
		m_object =NULL;
	}
}

void CswypeDialog::invalidate()
{
	resetPaintedArea();
}

/** @brief Get which index it is for the object under the mouse */
int CswypeDialog::getScrollIndex( keybutton sym)
{
	if ( sym<m_firstKey || sym>m_firstKey+m_nrFkeys-1)
	{
		return -1;
	}
	int key =(sym-m_firstKey);
	int invisible =m_firstVisibleUnit;
	int firstKey =(invisible%m_nrFkeys);
	int offset =(key-firstKey+m_nrFkeys)%m_nrFkeys+invisible;
	return ( offset>=0 && offset<(int)rows()) ? offset:-1;
}

/** @brief Paint a horizontal scrolling surface
 *  @param optional [in] Do or don't paint the optional fields
 */
void CswypeDialog::onPaintSurfaceHorizontal( bool optional)
{
	static int count =5;

	for (int mx=0; mx<200; mx++) // Remove endless loop.
	{
		// Check what to paint.
		switch ( visiblePainted())
		{
		case PAINT_LEFT:
			if (paintSurfaceLeft() ==false)
			{
				return;
			}
			m_repaint =true;
			break;

		case PAINT_RIGHT:
			if ( paintSurfaceRight() ==false)
			{
				return;
			}
			m_repaint =true;
			break;

		case PAINT_TOP:
			paintSurfaceTop();
			m_repaint =true;
			break;

		case PAINT_BOTTOM:
			paintSurfaceBottom();
			m_repaint =true;
			break;

		case PAINT_TOP_OPTIONAL:
			if ( !optional) return;
			if ( --count>0) return;
			paintSurfaceTop();
			count =5;
			return;

		case PAINT_BOTTOM_OPTIONAL:
			if ( !optional) return;
			if ( --count>0) return;
			paintSurfaceBottom();
			count =5;
			return;

		case PAINT_LEFT_OPTIONAL:
			if ( !optional) return;
			if ( --count>0) return;
			paintSurfaceLeft();
			count =5;
			return;

		case PAINT_RIGHT_OPTIONAL:
			if ( !optional) return;
			if ( --count>0) return;
			count =5;
			paintSurfaceRight();

		case PAINT_READY:
			return;
		default:
			break;
		}
	}
}

/** @brief Calculate the square of a single rectangle in our swipe dialog */
void CswypeDialog::calculateItemRect()
{
	if ( m_horizontal)
	{
		m_itemRect =Crect( 0,0, m_itemBlocks, m_rect.height());
	}
	else
	{
		m_itemRect =Crect( 0,0, m_rect.width(), m_itemBlocks);
	}
}

/// @brief The surface area should be painted, else not done!
Epainted CswypeDialog::visiblePainted()
{
	if (m_firstVisibleUnit >m_lastUnitPainted || m_lastVisibleUnit <m_firstUnitPainted)
	{
		m_firstUnitPainted =m_firstVisibleUnit;
		m_lastUnitPainted =m_firstVisibleUnit;
		m_visibleBuffers =0;
	}
	if ( m_firstVisibleUnit <m_firstUnitPainted)
	{
		return m_horizontal ? PAINT_LEFT:PAINT_TOP;
	}
	if ( m_lastUnitPainted <m_lastVisibleUnit)
	{
		return m_horizontal ? PAINT_RIGHT:PAINT_BOTTOM;
	}
	if ( m_firstUnitPainted >m_firstOptionalUnit && m_visibleBuffers<m_validBuffers)
	{
		return m_horizontal ? PAINT_LEFT_OPTIONAL:PAINT_TOP_OPTIONAL;
	}
	if ( m_lastUnitPainted <m_lastOptionalUnit && m_visibleBuffers<m_validBuffers)
	{
		return m_horizontal ? PAINT_RIGHT_OPTIONAL:PAINT_BOTTOM_OPTIONAL;
	}
	return PAINT_READY;
}

/** @brief Paint a vertical scrolling surface
 *  @param optional [in] Do or don't paint the optional fields
 */
void CswypeDialog::onPaintSurfaceVertical( bool optional)
{
	static int count =5;
	calculateSurfacePosition();

	for (int mx=0; mx<2000; mx++) // Remove endless loop.
	{
		// Check if we can paint?
		if ( m_firstUnitPainted>0 && (m_firstUnitPainted>m_lastVisibleUnit || m_lastUnitPainted<m_firstVisibleUnit))
		{
			resetPaintedArea();
		}
		// Check what to paint.
		switch ( visiblePainted())
		{
		case PAINT_TOP:
			paintSurfaceTop();
			m_repaint =true;
			break;

		case PAINT_BOTTOM:
			paintSurfaceBottom();
			m_repaint =true;
			break;

		case PAINT_TOP_OPTIONAL:
			if ( !optional) return;
			if ( --count>0) return;
			count=10;
			paintSurfaceTop();
			m_repaint =true;
			return;

		case PAINT_BOTTOM_OPTIONAL:
			if ( !optional) return;
			if ( --count>0) return;
			count=10;
			paintSurfaceBottom();
			m_repaint =true;
			return;

		case PAINT_READY:
			return;
		default:
			break;
		}
	}
}

/** @brief Insert a texture at the begin of the list
 *  @return Pointer to (new) object
 */
CswypeObject *CswypeDialog::insertAtBegin()
{
	sdlTexture *texture;
	CswypeObject *obj;
	// Erase first item?
	m_visibleSize =m_lastVisibleUnit-m_firstVisibleUnit+1;

	if ( m_visibleBuffers<m_validBuffers)
	{
		// Re-use last image in list if too many
		obj =m_swypeObjects[ m_validBuffers-1];
		texture =obj->texture;
		m_swypeObjects.erase( m_swypeObjects.begin()+m_validBuffers-1);
		m_swypeObjects.insert( m_swypeObjects.begin(), obj);
		m_visibleBuffers++;
	}
	else if ( m_visibleBuffers>=m_visibleSize*3)
	{
		// Re-use last valid image in list
		obj =m_swypeObjects[ m_validBuffers-1];
		texture =obj->texture;
		m_swypeObjects.erase( m_swypeObjects.begin()+m_validBuffers-1);
		m_swypeObjects.insert( m_swypeObjects.begin(), obj);
		// Need to correct last unit painted.
		int last =m_swypeObjects[m_validBuffers-1]->index;
		if ( last<m_lastUnitPainted)
		{
			m_lastUnitPainted =last+1;
		}
	}
	else
	{
		// Create new one.
		texture =createSurface();
		obj =new CswypeObject( texture, 0, m_itemRect);
		if ( !texture || !obj)
		{
			return NULL;
		}
		m_validBuffers++;
		m_visibleBuffers++;
		m_swypeObjects.insert( m_swypeObjects.begin(), obj);
	}
	obj->index =--m_firstUnitPainted;
	return obj;
}

/** @brief Create a surface for a part of the swipe to use
 *  @return Surface pointer
 */
sdlTexture *CswypeDialog::createSurface()
{
#ifdef USE_SDL2
	SDL_Texture *texture =SDL_CreateTexture( graphics()->getRenderer(),
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET, m_itemRect.width()*8, m_itemRect.height()*8); //, itemBlocks()*8, m_rect.height()*8);
	return texture;
#else
	int options =SDL_SWSURFACE; //|SDL_NOFRAME;
    Uint32 rmask, gmask, bmask, amask;

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

    SDL_Surface *surface =SDL_CreateRGBSurface( options, m_itemRect.width()*8, m_itemRect.height()*8,
    		                         32, //::m_mainGraph->m_renderSurface->format->BitsPerPixel,
                                     rmask, gmask, bmask, amask);
	return surface;
#endif
}

/** @brief Create a surface for a part of the swipe to use
 *  @return Surface pointer
 */
sdlTexture *CswypeDialog::createSurface( int w, int h)
{
#ifdef USE_SDL2
	(void)w;
	(void)h;
	SDL_Texture *texture =SDL_CreateTexture( graphics()->getRenderer(),
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET, m_itemRect.width()*8, m_itemRect.height()*8); //, itemBlocks()*8, m_rect.height()*8);
	return texture;
#else
	int options =SDL_SWSURFACE; //|SDL_NOFRAME;
    Uint32 rmask, gmask, bmask, amask;

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

    SDL_Surface *surface =SDL_CreateRGBSurface( options, w*8, h*8,
    		                         32, //::m_mainGraph->m_renderSurface->format->BitsPerPixel,
                                     rmask, gmask, bmask, amask);
	return surface;
#endif
}

/** @brief Insert a texture at the end of the list
 *  @return Pointer to (new) object
 */
CswypeObject *CswypeDialog::insertAtEnd()
{
	sdlTexture *texture;
	CswypeObject *obj;
	// Erase first item?
	m_visibleSize =m_lastVisibleUnit-m_firstVisibleUnit+1;
	if ( m_visibleBuffers< m_validBuffers)
	{
		int sz=(int)m_swypeObjects.size();
		if ( sz!=m_validBuffers)
		{
			return NULL;
		}
		obj =m_swypeObjects[ m_visibleBuffers++];
	}
	else if ( m_visibleBuffers>=m_visibleSize*3)
	{
		// Re-use first image in list if too many
		obj =m_swypeObjects[ 0];
		m_swypeObjects.erase( m_swypeObjects.begin());
		m_swypeObjects.push_back( obj);
		int first =m_swypeObjects[0]->index;
		if ( first>m_firstUnitPainted)
		{
			m_firstUnitPainted =first;
		}
	}
	else
	{
		// Create new one.
		texture =createSurface();
		obj =new CswypeObject( texture, 0, m_itemRect);
		if ( !texture || !obj)
		{
			// ("CswypeDialog::insertAtEnd  Cannot create texture+object");
			return NULL;
		}
		m_swypeObjects.push_back( obj);
		m_validBuffers++;
		m_visibleBuffers++;
	}
	obj->index =m_lastUnitPainted++;
	return obj;
}

/** @brief Paint a row on top of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
void CswypeDialog::paintSurfaceTop()
{
	CswypeObject *obj =insertAtBegin();
	// Actual painting
	m_graphics->setRenderArea( obj->texture);
	onPaintUnit( obj->index, m_itemRect);
	m_graphics->setRenderArea( NULL);
}

/** @brief Paint a column on the left of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
bool CswypeDialog::paintSurfaceLeft()
{
	CswypeObject *obj =insertAtBegin();
	// Actual painting
	graphics()->setRenderArea( obj->texture);
	onPaintUnit( obj->index, m_itemRect);
	graphics()->setRenderArea( NULL);
	return true;
}

/** @brief Paint a row on the bottom of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
void CswypeDialog::paintSurfaceBottom()
{
	CswypeObject *obj =insertAtEnd();
	if (obj)
	{
		// Actual painting
		m_graphics->setRenderArea( obj->texture);
		onPaintUnit( obj->index, m_itemRect);
		m_graphics->setRenderArea( NULL);
	}
}

/** @brief Paint a column on the right of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
bool CswypeDialog::paintSurfaceRight()
{
	CswypeObject *obj =insertAtEnd();
	if (obj)
	{
		// Actual painting
		graphics()->setRenderArea( obj->texture);
		onPaintUnit( obj->index, m_itemRect);
		graphics()->setRenderArea( NULL); // Back to main window
		return true;
	}
	return false;
}

/// @brief Paint all things.
void CswypeDialog::onPaint()
{
	SDL_Rect rct;
	SDL_Rect dlg;
	if ( !m_visible || !m_repaint)
	{
		return;
	}
	m_repaint =false;
	graphics()->setPixelOffset(0,0);
	if (m_horizontal)
	{
		onPaintSurfaceHorizontal( true);
	}
	else
	{
		onPaintSurfaceVertical( true);
	}
	rct.x =(Sint16)(m_rect.left()*8+m_dialogOffset.x);
	rct.y =(Sint16)(m_rect.top()*8+m_dialogOffset.y);
	rct.w =(Uint16)(m_rect.width()*8);
	rct.h =(Uint16)(m_rect.height()*8);
	dlg.x =rct.x;
	dlg.y =rct.y;
	dlg.w =rct.w;
	dlg.h =rct.h;
	m_graphics->setColour( m_backgroundColour);
	m_graphics->setViewport( &rct);
	m_graphics->bar( rct.x, rct.y, rct.x+rct.w, rct.y+rct.h);
	if ( m_visibleBuffers >0)
	{
		if ( m_horizontal)
		{
			int width =itemBlocks()*8;
			int x=rct.x+m_swypeObjects[0]->index*width-(int)m_scroll;
			// Draw horizontal items
			rct.w =(Uint16)(m_swypeObjects[0]->destination.width()*8);
			rct.h =(Uint16)(m_swypeObjects[0]->destination.height()*8);
			for ( int n=0; n<m_lastUnitPainted-m_firstUnitPainted; n++)
			{
				CswypeObject *obj =m_swypeObjects[n];
				rct.x =(Sint16)x;
				x +=width;
				renderCopy( obj->texture, &rct);
			}
		}
		else
		{
			int height =itemBlocks()*8;
			int y=rct.y+m_swypeObjects[0]->index*height-(int)m_scroll;
			// Draw horizontal items
			rct.w =(Uint16)(m_itemRect.width()*8);
			rct.h =(Uint16)height;
			for ( int n=0; n<m_visibleBuffers; n++)
			{
				CswypeObject *obj =m_swypeObjects[n];
				rct.y =(Sint16)y;
				y +=height;
				renderCopy( obj->texture, &rct);
			}
		}
	}
	// Put back old viewport
	m_graphics->setViewport( NULL);
	m_graphics->update(Crect(dlg.x, dlg.y, dlg.w, dlg.h));
}

/** @brief Render a surface
 *  @param surface [in] Swype unit to paint
 *  @param rect [in] Rectangle to use
 */
void CswypeDialog::renderCopy( sdlTexture *surface, SDL_Rect *rect)
{
#ifdef USE_SDL2
	graphics()->renderTexture( surface, rect->x, rect->y, rect->w, rect->h);
#else
	graphics()->renderSurface( surface, rect->x, rect->y, rect->w, rect->h);
#endif
}

/** @brief Clean dialog afterwards */
void CswypeDialog::onCleanup()
{
	clean();
	m_firstUnitPainted =0;
	m_lastUnitPainted =0;
	m_firstVisibleUnit =0;
	m_lastVisibleUnit =0;
}

/// @brief  Scroll to absolute position.
/// @param offset [in] Where to scroll to in absolute pixels
/// @param time [in] How long it takes to go there
/// @return True if changed value.
bool CswypeDialog::scrollToPixel( int offset, int time)
{
	int maxScroll =scrollMax();
	offset =gLimit(offset, 0,maxScroll);
	if ( time<=0)
	{
		m_moveTime =0;
		return scrollRelative( (double)offset-m_scroll, false);
	}
	else
	{
		m_startPosition =(int)m_scroll;
		m_endPosition =offset;
		m_timer.setTime( time, 100,3);
		m_moveTime =time;
		return true;
	}
}

/** @brief Calculate the maximum scroll distance.
 *  @return Number of rows in the scrolling dialog.
 */
int CswypeDialog::scrollMax()
{
	m_sizes =rows();
	int mx =m_sizes*itemBlocks()*8+m_endMargin-
			( m_horizontal ? (m_rect.width()*8):(m_rect.height()*8) );
	if ( mx<0) mx=0;
	return mx;
}

/// @brief Calculate the maximum scroll distance
/// @return Maximum scroll distance for left edge of screen rectangle
int CswypeDialog::surfaceMax()
{
	return m_horizontal ? ( m_graphics->width()-m_rect.width()*8-m_itemBlocks*8):
			              ( m_graphics->height()-m_rect.height()*8-m_itemBlocks*8);
}

/** @brief  Scroll a certain distance.
 *  @param distance [in] Distance to move, either positive or negative
 *  @param flow [in] Do we need to flow afterwards, or stop immediately
 *  @return true when done
 */
bool CswypeDialog::scrollRelative( double distance, bool flow)
{
	if ( distance >-0.1 && distance<0.1)
	{
		return false;
	}
	int db =(int)m_scroll;
	m_scroll +=distance;
	if ( m_scroll<0)
	{
		m_scroll =0;
		m_speeding.clear();
		flow =false;
	}
	double mx=scrollMax();
	if ( m_scroll >mx)
	{
		m_scroll =mx;
		m_speeding.clear();
		flow =false;
	}
	if ( flow)
	{
		m_speeding.addPosition( m_scroll);
	}
	if ( (int)(m_scroll+0.5) ==db)
	{
		return true;
	}
	m_repaint =true;
	calculateSurfacePosition();
	return true;
}

/** Remove all buffers to see. */
void CswypeDialog::clean()
{
	for ( int n=0; n<(int)m_swypeObjects.size(); n++)
	{
		delete m_swypeObjects[n];
	}
	m_swypeObjects.clear();
	m_validBuffers =0;
	m_visibleBuffers =0;
	m_firstUnitPainted =0;
	m_lastUnitPainted =0;
	m_firstOptionalUnit =0;
	m_lastOptionalUnit =0;
}

/// @brief Calculate a new paintedArea.
void CswypeDialog::resetPaintedArea()
{
	m_firstUnitPainted =0;
	m_lastUnitPainted =0;
	m_firstOptionalUnit =0;
	m_lastOptionalUnit =0;

	m_lastUnitPainted =0;
	m_visibleBuffers =0;
	calculateItemRect();
	calculateSurfacePosition();
	m_repaint =true;
}

/** @brief Make sure the item is visible.
 *  @param row [in] For horizontal it is the column, for vertical scrolling it is a row.
 */
void CswypeDialog::makeSureRowVisible( int row, int time)
{
	// Check if item y is on screen.
	int pixels =itemBlocks()*8;
    row =row*pixels;
	if ( m_horizontal)
	{
		int width =(m_rect.width()*8)/pixels; // Number of items horizontal
		int margin =(width*2)/3;
		if ( row<(int)m_scroll)
		{
			scrollToPixel( row-pixels*(width/3), time);
		}
		if ( row>(int)m_scroll+m_rect.width()*8-pixels)
		{
			scrollToPixel( row-pixels*margin, time);
		}
	}
	else
	{
		int height =(m_rect.height()*8)/pixels;	// Number of items vertical
		int margin =(height*2)/3;

		if ( row<(int)(m_scroll+0.5))
		{
			scrollToPixel( row-pixels*(height/3), time);
			return;
		}
		if ( row>(int)(m_scroll+0.5)+m_rect.height()*8-pixels)
		{
			scrollToPixel( row-pixels*margin, time);
		}
	}
}

/** @brief Stop the swyping and set to top */
void CswypeDialog::topStop()
{
	scrollIndex(0,0);
	m_speeding.clear();
}

/** @brief Is the row visible?
 *  @param row [in] For horizontal it is the column, for vertical scrolling it is a row.
 *  @return TRUE on visible.
 */
bool CswypeDialog::isRowVisible( int row)
{
	// Check if item y is on screen.
	int pixels =itemBlocks()*8;

	if ( m_horizontal)
	{
		row=row*pixels;
		if ( row<(int)(m_scroll+0.5))
		{
			return false;
		}
		if ( row>(int)(m_scroll+0.5)+m_rect.width()*8-pixels)
		{
			return false;
		}
	}
	else
	{
		row=row*pixels;
		if ( row<(int)(m_scroll+0.5))
		{
			return false;
		}
		if ( row>(int)(m_scroll+0.5)+m_rect.height()*8-pixels)
		{
			return false;
		}
	}
	return true;
}

/// @brief  Depending on the scroll offset and painted area, we calculate the drawing area.
void CswypeDialog::calculateSurfacePosition()
{
	int nrRows =(int)rows();
	int siz;
	if ( m_horizontal)
	{
		siz =m_itemRect.width()*8;
		m_firstVisibleUnit =(int)(m_scroll/siz);
		m_lastVisibleUnit =(int)((m_scroll+m_rect.width()*8+siz-1)/siz);
	}
	else
	{
		siz =m_itemRect.height()*8;
		m_firstVisibleUnit =(int)(m_scroll/siz);
		m_lastVisibleUnit =(int)((m_scroll+m_rect.height()*8+siz-1)/siz);
	}
	m_firstOptionalUnit =m_firstVisibleUnit-m_listSize/4;
	m_lastOptionalUnit =m_lastVisibleUnit+m_listSize/4;
	if ( m_firstOptionalUnit<0)
	{
		m_firstOptionalUnit=0;
	}
	if ( m_lastOptionalUnit+m_endMargin>nrRows)
	{
		m_lastOptionalUnit=nrRows-m_endMargin;
	}
	//printf("First=%d, Last=%d,  Optional=%d-%d\n", m_firstVisibleUnit, m_lastVisibleUnit, m_firstOptionalUnit, m_lastOptionalUnit);
}

/// @brief Set index to a certain item in the list.
/// @return True if changed.
bool CswypeDialog::scrollIndex( double row, int time)
{
	bool retVal =scrollToPixel( (int)(row*((float)itemBlocks()*8.0)), time);
	return retVal;
}


/** @brief Calculate number of rows we have.
 *  @return Number of rows.
 */
size_t CswypeDialog::rows()
{
    return 10;
}


/** @brief Calculate height of a single row/item.
 *  @return Height of a single item/row.
 */
int CswypeDialog::itemBlocks()
{
    return m_itemBlocks;
}

/*============================================================================*/
///
/// @brief 		Loop for the dialog.
///
/// @post       Dialog changed.
///
/*============================================================================*/
bool CswypeDialog::onLoop()
{
	if ( m_visible ==false)
	{
		return m_alive;
	}
	//double speed;
	switch ( CdialogEvent::Instance()->getStatus())
	{
	case MOUSE_START_DRAG:
	case MOUSE_START_SCROLL_OR_DRAG:
		m_dialogSpeed =0;
		m_speeding.clear();
		break;

	case MOUSE_RELEASED:
		//speed =m_speeding.getRelative();
		//Log.write( "Scroll speed =%f", speed);
		if ( m_moveTime !=0)
		{
			m_speeding.clear();
		}
		scrollRelative( m_speeding.getSpeed(), false);
		break;

	case MOUSE_SCROLL:
	case MOUSE_DRAG:
	case MOUSE_PRESS:
	case MOUSE_DRAG_DIALOG:
	default:
		break;
	}
	if ( m_moveTime >0)
	{
		int timer =m_timer.elapsed();
		if ( timer>=m_moveTime)
		{
			// Scroll immediately to this position.
			if ( scrollToPixel( m_endPosition, 0)==true)
			{
				onUpdate();
			}
		}
		else
		{
			double offset =((m_endPosition-m_startPosition)*timer/m_moveTime)+m_startPosition;
			int maxY =scrollMax();
			offset =gLimit(offset, 0,maxY);
			double relative =offset-m_scroll;
			if ( m_endPosition+relative >-10.0 && m_endPosition+relative<10.0)
			{
				// ( "Reach end!");
				if ( scrollToPixel( m_endPosition, 0)==true)
				{
					onUpdate();
				}
				// Finish scrolling.
			}
			else
			{
				scrollRelative( relative, false);
			}
		}
	}
	onPaint();
	return m_alive;
}

/** @brief Stop auto scroll of the swipe dialog */
void CswypeDialog::clearSpeed()
{
	m_speed =0;
	m_speeding.clear();
}

/*============================================================================*/
///
/// @brief		Left button pressed for mouse.
///
/// @post       Mouse event handled.
///
/*============================================================================*/
Estatus CswypeDialog::onButton( keymode mod, keybutton sym)
{
	(void)mod;
	(void)sym;
	Estatus stat =DIALOG_EVENT_OPEN;
	return stat;
}

/** @brief Set the height of a single item
 *  @param rowHeight [in] Set item blocks
 */
void CswypeDialog::setItemBlocks( int rowHeight)
{
	if (m_itemBlocks==0 || m_itemBlocks>400)
	{
		m_itemBlocks=5;
	}
	m_itemBlocks =rowHeight;
	calculateItemRect();
}

/** Decide which key in a scroll dialog.
 *  @param p [in] Position of button
 */
keybutton CswypeDialog::findButton( const Cpoint &p)
{
	if ( m_visible ==false)
	{
		return KEY_NONE;
	}
	m_dialogSpeed =0;
	Cpoint q=p/8;
	if ( m_rect.inside( q)==true && m_visible ==true)
	{
		int item =(int)((p.y-m_rect.top()*8+m_scroll)/(8*itemBlocks()));
		keybutton key =(keybutton)(m_firstKey+(item%m_nrFkeys));
		Caudio::Instance()->click();
		return key;
	}
	return KEY_NONE;
}

/// @brief Roll the wheel in the middle.
void CswypeDialog::wheelUp( int mx, int my)
{
	(void)mx;
	(void)my;
	scrollRelative(-2, false);
}

/// @brief Roll the wheel in the middle.
void CswypeDialog::wheelDown( int mx, int my)
{
	(void)mx;
	(void)my;
	scrollRelative(+2, false);
}

/// @brief constructor.
Cspeeding::Cspeeding()
: m_measurements(0)
, m_previousTime(0)
{
	m_friction =Cgraphics::m_defaults.swype_friction;
}

/** @brief Add a new position to the speeding
 *  @param position [in] Relative position to add to the speed
 */
void Cspeeding::addPosition( double position)
{
	// ( "Add position %f", position);
	if ( (int)position ==(int)m_position[0])
	{
		return; // same position.
	}
	int now =Ctimeout::GetTickCount();
	int delta_t=m_previousTime-now;
	if ( delta_t<0) { delta_t =now-m_previousTime; }
	m_previousTime =now;
	if ( delta_t>4000)
	{
		m_measurements =0;
	}
	if ( delta_t<4)
	{
		// Same time, no use to record timestamps.
		m_position[0]=position;
		return;
	}
	int n=( m_measurements>=MEASUREMENTS) ? MEASUREMENTS-1:m_measurements;
	for ( int x=n-1; x>=0; x--)
	{
		m_position[x+1] =m_position[x];
		m_time[x+1] =m_time[x];
	}
	m_position[0] =position;
    m_time[0] =(double)delta_t;
    if ( m_measurements<MEASUREMENTS) m_measurements++;
}

/** @brief Get speed to run now
 *  @return speed in pixels
 */
double Cspeeding::getSpeed()
{
	if ( m_measurements==0)
	{
		return 0;
	}
	double totalSpeed =0;
	/// Calculate speeds.
	for ( int n=0; n<m_measurements-1; n++)
	{
		m_speed[n] =(m_position[n+1]-m_position[n])/(m_time[n]+0.001);
		totalSpeed -=m_speed[n];
	}
	int now=Ctimeout::GetTickCount();
	int delta_t=m_previousTime-now;
	if ( delta_t<0) { delta_t =now-m_previousTime; }
	if ( delta_t<20 || m_friction>1000)
	{
		return 0;
	}
	//totalSpeed/=(1+m_friction*y);
	return totalSpeed*1000.0f/(1000.0f+(delta_t*m_friction)/10.0f);
}

/** @brief Paint one item in the list again
 *  @param row [in] What item to paint
 */
void CswypeDialog::invalidate( int row)
{
	if ( row<0 || m_visibleBuffers<=0)
	{
		m_repaint =true;
		return;
	}
	int idx;
	CswypeObject *obj;
	int sz=(int)m_swypeObjects.size();
	for ( idx=0; idx<sz; idx++)
	{
		obj =m_swypeObjects[ idx];
		if ( obj && obj->index ==row)
		{
			graphics()->setRenderArea( obj->texture);
			Crect rect( 0, 0, obj->destination.width(), obj->destination.height());
			onPaintUnit( row, rect);
			graphics()->setRenderArea( NULL);
			m_repaint =true;
			break;
		}
	}
}

/** @brief Change cursor to other position
 *  @param row [in] New cursor position
 */
void CswypeDialog::setCursor( int unit)
{
    m_speeding.clear();
	if ( unit ==m_cursor)
	{
		invalidate( m_cursor);
		makeSureRowVisible( unit, 1000);
		return;
	}
	int prev =m_cursor;
	m_cursor =unit;

	invalidate( prev);
	invalidate( unit);
	if ( unit>=0)
	{
		makeSureRowVisible( unit, 1000);
	}
}

/** @brief Find object for a certain point. Not in sub-dialogs!
 *  @return Object found at certain location
 */
CdialogObject * CswypeDialog::findObject( const Cpoint &p)
{
	Cpoint q=p/8;
	if ( m_rect.inside( q)==false || m_visible ==false)
	{
		return NULL;
	}
	int itemPixelHeight =8*itemBlocks();
	int row =(int)((p.y-m_rect.top()*8+m_scroll)/itemPixelHeight);
	if ( row<0 || row>=(int)rows())
	{
		return NULL;
	}
	m_dragIndex =row;
	m_object->m_rect.setTop( (int)(m_rect.top()+(m_dragIndex*itemPixelHeight-m_scroll)/8) );
	return m_object;
}

/** Paint on normal background
 *  @param row [in] What to paint
 *  @param location [in] Where to paint it
 */
void CswypeDialog::onPaintBackground( int row, const Crect &location)
{
#ifdef USE_SDL2
	(void)row;
	onPaintUnit( m_dragIndex, location);
#else
	(void)row;
	Cgraphics *previous =m_graphics;
	Cdialog *parent =m_parent;
	m_graphics =m_mainGraph;
	m_parent =NULL;
	m_object->m_graphics =m_mainGraph;
	m_graphics =m_mainGraph;
	onPaintUnit( m_dragIndex, location);
	m_graphics =previous;
	m_parent =parent;
#endif
}

/** @brief Scroll to a certain row (or column).
 *  @param row [in] What row for vertical scrolling, what column for horizontal scrolling.
 *  @param time [in] Time to get to that row.
 *  @return If we have a go!
 */
bool CswypeDialog::scrollToRow( int row, int time)
{
	return scrollToPixel( row*itemBlocks()*8, time);
}
