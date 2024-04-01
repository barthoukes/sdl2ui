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
#include "sdl_audio.h"
#include "sdl_surface.h"
#include "sdl_graphics.h"
#include "sdl_swype_dialog.h"

/*----------------------------------------------------------------------------*/
/** Dialog to scroll objects left-right or up-down */
CswypeDialog::CswypeDialog( Cdialog *parent, const Crect &rect, keybutton firstKey, bool horizontal)
: Cdialog( parent, "scrollRect", rect)
, m_itemRect( 0,0,rect.width(),itemBlocks())
, m_listSize(0)
, m_sizes(0)
, m_scrollPosition(0)
, m_speed(0.0f)
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
, m_horizontal(horizontal)
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
, m_lowPrioCount(0)
{
	m_object =std::make_shared<CscrollObject>( this, 0, Csize(m_rect.width(), itemBlocks()));

	if ( m_graphics !=NULL && m_isOwner ==true)
	{
		delete m_graphics;
		m_isOwner =false;
	}
	m_graphics =m_mainGraph;
	m_listSize =(3*m_rect.height())/itemBlocks();
	setRect( rect);
	calculateItemRect();
}

/*----------------------------------------------------------------------------*/
/** @brief Change or init the rectangle to this size.
 *  @param rect [in] What position and size to use.
 */
void CswypeDialog::setRect( const Crect &rect)
{
    lock();
	m_rect.setLeft( rect.left());
	m_rect.setTop( rect.top());

	int py =rect.height()*8;
	int px =rect.width()*8;
	if (px==0) px=25*8;
	if (py==0) py=6*8;

	if ( m_rect.width() !=rect.width() || m_rect.height() !=rect.height()
	   || m_graphics ==NULL)
	{
		if ( m_graphics && m_isOwner)
		{
			delete m_graphics;
			m_graphics =m_mainGraph;
			m_isOwner =false;
		}
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
	    m_graphics =new Cgraphics( Csize(px, py), false);
	    if( m_graphics != NULL)
	    {
		    m_graphics->init();
		    m_isOwner =true;
	    }
	}
	unlock();
}

/*----------------------------------------------------------------------------*/
CswypeDialog::~CswypeDialog()
{
	CswypeDialog::clean();
	if ( m_graphics && m_isOwner)
	{
		delete m_graphics;
	}
	m_graphics =NULL;
	m_isOwner =false;
	m_object = nullptr;
}

/*----------------------------------------------------------------------------*/
void CswypeDialog::invalidate()
{
	resetPaintedArea();
}

/*----------------------------------------------------------------------------*/
/** @brief Get which index it is for the object under the mouse */
int CswypeDialog::getScrollIndex( keybutton sym)
{
    int retVal = -1;
    lock();
	if ( sym >= m_firstKey && sym <= m_firstKey+m_nrFkeys-1)
	{
        int key =(sym-m_firstKey);
        int invisible =m_firstVisibleUnit;
        int firstKey =(invisible%m_nrFkeys);
        int offset =(key-firstKey+m_nrFkeys)%m_nrFkeys+invisible;
        retVal = ( offset>=0 && offset<(int)rows()) ? offset:-1;
	}
	unlock();
	return retVal;
}

/*----------------------------------------------------------------------------*/
int CswypeDialog::getScrollOffset()
{
    return (int)m_scrollPosition;
}

/*----------------------------------------------------------------------------*/
/** @brief Paint a horizontal scrolling surface
 *  @param optional [in] Do or don't paint the optional fields
 */
void CswypeDialog::onPaintSurfaceHorizontal( bool optional)
{
	lock();
    calculateSurfacePosition();
    bool stop = false;

	for (int mx=0; mx<200 && !stop; mx++) // Remove endless loop.
	{
		// Check what to paint.
		switch ( visiblePainted())
		{
		case PAINT_LEFT:
			paintSurfaceLeft();
			m_repaint =true;
			break;

		case PAINT_RIGHT:
			paintSurfaceRight();
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
			if (optional)
			{
				paintSurfaceTop();
			}
			stop = true;
			break;

		case PAINT_BOTTOM_OPTIONAL:
			if (optional)
			{
				paintSurfaceBottom();
			}
			stop = true;
			break;

		case PAINT_LEFT_OPTIONAL:
			if (optional)
			{
				paintSurfaceLeft();
			}
			stop = true;
			break;

		case PAINT_RIGHT_OPTIONAL:
			if (optional)
			{
				paintSurfaceRight();
			}
			break;

		case PAINT_READY:
			stop = true;
			break;

		default:
			break;
		}
	}
	unlock();
}

/*----------------------------------------------------------------------------*/
/** @brief Calculate the square of a single rectangle in our swipe dialog */
void CswypeDialog::calculateItemRect()
{
    lock();
	if ( m_horizontal)
	{
		m_itemRect =Crect( 0,0, m_itemBlocks, m_rect.height());
	}
	else
	{
		m_itemRect =Crect( 0,0, m_rect.width(), m_itemBlocks);
	}
	unlock();
}

/*----------------------------------------------------------------------------*/
/// @brief The surface area should be painted, else not done!
Epainted CswypeDialog::visiblePainted()
{
	if ( m_firstVisibleUnit <m_firstUnitPainted)
	{
	    m_lowPrioCount = 1000;
		return m_horizontal ? PAINT_LEFT:PAINT_TOP;
	}
	if ( m_lastUnitPainted <m_lastVisibleUnit)
	{
	    m_lowPrioCount = 1000;
		return m_horizontal ? PAINT_RIGHT:PAINT_BOTTOM;
	}
	if (--m_lowPrioCount<0)
	{
	    m_lowPrioCount = 1000;
        int optionsPaintedFirst = m_firstUnitPainted - m_firstOptionalUnit;
        int optionsPaintedLast = (m_lastOptionalUnit - m_lastUnitPainted) -
                        (m_lastVisibleUnit - m_firstVisibleUnit);

        if (optionsPaintedFirst < optionsPaintedLast)
        {
            // First has prio.
            if ( m_firstUnitPainted > m_firstOptionalUnit)
            {
                return m_horizontal ? PAINT_LEFT_OPTIONAL:PAINT_TOP_OPTIONAL;
            }
        }
        if ( m_lastUnitPainted < m_lastOptionalUnit)
        {
            return m_horizontal ? PAINT_RIGHT_OPTIONAL:PAINT_BOTTOM_OPTIONAL;
        }
        if ( m_firstUnitPainted > m_firstOptionalUnit)
        {
            return m_horizontal ? PAINT_LEFT_OPTIONAL:PAINT_TOP_OPTIONAL;
        }
        m_spares.clear();
	}
	return PAINT_READY;
}

/*----------------------------------------------------------------------------*/
/** @brief Paint a vertical scrolling surface
 *  @param optional [in] Do or don't paint the optional fields
 */
void CswypeDialog::onPaintSurfaceVertical( bool optional)
{
	calculateSurfacePosition();

	// Check if we should reset the view?
	if ( m_firstUnitPainted>=0 && (m_firstUnitPainted>m_lastVisibleUnit || m_lastUnitPainted<m_firstVisibleUnit))
	{
		resetPaintedArea();
	}

	// Paint up to 200 things.
	for (int mx=0; mx<200; mx++) // Remove endless loop.
	{
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
			paintSurfaceTop();
			m_repaint =true;
			return;

		case PAINT_BOTTOM_OPTIONAL:
			if ( !optional) return;
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

/*----------------------------------------------------------------------------*/
/** @brief Insert a texture at the begin of the list
 *  @return Pointer to (new) object
 */
std::shared_ptr<CswypeObject> CswypeDialog::insertAtBegin(const Crect &rect)
{
    std::shared_ptr<CswypeObject> obj;

    lock();
	try
	{
        int rows = (int)m_swypeObjects.size();
        // Erase first item?
        m_visibleSize =m_lastVisibleUnit-m_firstVisibleUnit+1;

        if ( !m_spares.empty())
        {
            // Re-use one of the spares
            obj =m_spares[0];
            m_spares.erase( m_spares.begin());
        }
        else if (m_lastUnitPainted > m_lastOptionalUnit && rows)
        {
            obj =m_swypeObjects.at(rows - 1);
            m_swypeObjects.erase( m_swypeObjects.begin() + rows - 1);
            m_lastUnitPainted--;
        }
        Csize size = rect.size();
        if ( !obj.get() || !obj->compareSize(size))
        {
            // Create new one.
            obj =std::make_shared<CswypeObject>( size, 0, rect);
        }
        m_swypeObjects.insert( m_swypeObjects.begin(), obj);
        obj->index =--m_firstUnitPainted;
	}
    catch (...)
    {
        Cgraphics::m_defaults.log("ERROR CswypeDialog::insertAtBegin");
    }

	unlock();
	return obj;
}

/*----------------------------------------------------------------------------*/
/** @brief Insert a texture at the end of the list
 *  @return Pointer to (new) object
 */
CswypeObjectPtr CswypeDialog::insertAtEnd(const Crect &rect)
{
    CswypeObjectPtr obj = nullptr;

    lock();
	try
	{
	    // Erase first item?
	    m_visibleSize =m_lastVisibleUnit-m_firstVisibleUnit+1;

	    if (!m_spares.empty())
        {
	        // Re-use one of the spares
            obj = m_spares[0];
            m_spares.erase( m_spares.begin() );
        }
	    else if (m_firstUnitPainted < m_firstOptionalUnit && !m_swypeObjects.empty())
	    {
            obj =m_swypeObjects[0];
            m_swypeObjects.erase(m_swypeObjects.begin());
            m_firstUnitPainted++;
	    }
        Csize size = rect.size();
        if ( !obj.get() || !obj->compareSize(size))
	    {
	        // Create new one.
	        obj =std::make_shared<CswypeObject>( size, 0, rect);
	    }
        m_swypeObjects.push_back( obj);
        obj->index =m_lastUnitPainted++;
	}
	catch (...)
    {
        Cgraphics::m_defaults.log("ERROR CswypeDialog::insertAtEnd");
    }
	unlock();
	return obj;
}

/*----------------------------------------------------------------------------*/
/** @brief Insert a texture at the end of the list
 *  @return Pointer to (new) object
 */
void CswypeDialog::popFirst()
{
	lock();
    try
    {
	m_visibleSize =m_lastVisibleUnit-m_firstVisibleUnit+1;
	// Erase first item?
	if (m_visibleSize >0)
	{
		m_swypeObjects.erase( m_swypeObjects.begin());
		int first =m_swypeObjects.at(0)->index;
		if ( first>m_firstUnitPainted)
		{
			m_firstUnitPainted =first;
		}
	}
    }
    catch (...)
    {
        Cgraphics::m_defaults.log("ERROR CswypeDialog::popFirst");
    }
	unlock();
}

/** @brief Paint a row on top of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
void CswypeDialog::paintSurfaceTop()
{
	paintSurfaceLeft();
}

/*----------------------------------------------------------------------------*/
/** @brief Paint a column on the left of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
void CswypeDialog::paintSurfaceLeft()
{
	CswypeObjectPtr obj =insertAtBegin(m_itemRect);
	// Actual painting
	m_graphics->setRenderArea( obj->getTexture());
	onPaintUnit( obj->index, m_itemRect);
	m_graphics->setRenderArea( NULL);

	//Cgraphics::m_defaults.log("CswypeDialog::paintSurfaceLeft %d", obj->index);
}

/*----------------------------------------------------------------------------*/
/** @brief Paint a row on the bottom of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
void CswypeDialog::paintSurfaceBottom()
{
	paintSurfaceRight();
}

/*----------------------------------------------------------------------------*/
/** @brief Paint a column on the right of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
void CswypeDialog::paintSurfaceRight()
{
	CswypeObjectPtr obj =insertAtEnd(m_itemRect);

	SDL_Surface *surface = NULL;
	if (obj.get())
	{
	    surface = obj->getTexture();
	}
	if (surface)
	{
        // Actual painting
        m_graphics->setRenderArea( obj->getTexture());
        onPaintUnit( obj->index, m_itemRect);
        m_graphics->setRenderArea( NULL); // Back to main window
	}
}

/*----------------------------------------------------------------------------*/
/// @brief Paint all things.
void CswypeDialog::onPaint()
{
	SDL_Rect rct;
	SDL_Rect dlg;
	lock();

	if ( m_visible && m_repaint)
    try
    {
        m_repaint =false;
        m_mainGraph->setPixelOffset(0,0);
        // Prepare to draw more buffers:
        m_horizontal ? onPaintSurfaceHorizontal( true):onPaintSurfaceVertical( true);

        rct.x =(Sint16)(m_rect.left()*8+m_dialogOffset.x);
        rct.y =(Sint16)(m_rect.top()*8+m_dialogOffset.y);
        rct.w =(Uint16)(m_rect.width()*8);
        rct.h =(Uint16)(m_rect.height()*8);
        dlg.x =rct.x;
        dlg.y =rct.y;
        dlg.w =rct.w;
        dlg.h =rct.h;
        m_mainGraph->setColour( m_backgroundColour);
        m_mainGraph->setViewport( &rct);
        m_mainGraph->bar( rct.x, rct.y, rct.x+rct.w, rct.y+rct.h);
        m_horizontal ? onPaintHorizontal(rct):onPaintVertical(rct);
        // Put back old viewport
        m_mainGraph->setViewport( NULL);
        m_mainGraph->update(dlg.x, dlg.y, dlg.w, dlg.h);
    }
    catch (const std::exception& err)
    {
        m_mainGraph->setViewport( NULL);
        Cgraphics::m_defaults.log("ERROR CswypeDialog::onPaint %s", err.what());
    }
	unlock();
}

/*----------------------------------------------------------------------------*/
/// @brief Paint horizontal dialog items.
void CswypeDialog::onPaintHorizontal(SDL_Rect &rct)
{
    int width =itemBlocks()*8;
    int x=rct.x+m_swypeObjects.at(0)->index*width-(int)m_scrollPosition;
    // Draw horizontal items
    rct.w =(Uint16)(m_swypeObjects.at(0)->destination.width()*8);
    rct.h =(Uint16)(m_swypeObjects.at(0)->destination.height()*8);
    for ( auto obj : m_swypeObjects)
    {
        rct.x =(Sint16)x;
        x +=width;
        renderCopy( obj->getTexture(), &rct);
    }
}

/*----------------------------------------------------------------------------*/
/// @brief Paint vertical dialog items.
void CswypeDialog::onPaintVertical(SDL_Rect &rct)
{
    int height =itemBlocks()*8;
    int y=rct.y+m_swypeObjects.at(0)->index*height-(int)m_scrollPosition;
    // Draw horizontal items
    rct.w =(Uint16)(m_itemRect.width()*8);
    rct.h =(Uint16)height;
    for ( auto obj : m_swypeObjects)
    {
        rct.y =(Sint16)y;
        y +=height;
        renderCopy( obj->getTexture(), &rct);
    }
}

/*----------------------------------------------------------------------------*/
/** @brief Render a surface
 *  @param surface [in] Swype unit to paint
 *  @param rect [in] Rectangle to use
 */
void CswypeDialog::renderCopy( SDL_Surface *surface, SDL_Rect *rect)
{
	m_mainGraph->surface( surface, rect->x, rect->y, rect->w, rect->h);
}

/*----------------------------------------------------------------------------*/
/** @brief Clean dialog afterwards */
void CswypeDialog::onCleanup()
{
	lock();
    m_spares.clear();
    m_swypeObjects.clear();

	m_firstUnitPainted =0;
	m_lastUnitPainted =0;
	m_firstVisibleUnit =0;
	m_lastVisibleUnit =0;
    m_firstOptionalUnit =0;
    m_lastOptionalUnit =0;

    unlock();
}

/*----------------------------------------------------------------------------*/
/// @brief  Scroll to absolute position.
/// @param offset [in] Where to scroll to in absolute pixels
/// @param time [in] How long it takes to go there
/// @return True if changed value.
bool CswypeDialog::scrollToPixel( int offset, int time)
{
	lock();
	bool retVal = true;

	int maxScroll =scrollMax();
	offset =gLimit(offset, 0,maxScroll);
	if ( time<=0)
	{
		m_moveTime =0;
		retVal = scrollRelative( (double)offset-m_scrollPosition, false);
	}
	else
	{
		m_startPosition =(int)m_scrollPosition;
		m_endPosition =offset;
		m_timer.setTime( time, 100,3);
		m_moveTime =time;
		retVal = true;
	}
	unlock();
	return retVal;
}

/*----------------------------------------------------------------------------*/
/** @brief Calculate the maximum scroll distance.
 *  @return Number of rows in the scrolling dialog.
 */
int CswypeDialog::scrollMax()
{
	lock();

	m_sizes =rows();
	int mx =m_sizes*itemBlocks()*8+m_endMargin-
			( m_horizontal ? (m_rect.width()*8):(m_rect.height()*8) );
	if ( mx<0) mx=0;

	unlock();
	return mx;
}

/*----------------------------------------------------------------------------*/
/// @brief Calculate the maximum scroll distance
/// @return Maximum scroll distance for left edge of screen rectangle
int CswypeDialog::surfaceMax()
{
	return m_horizontal ? ( m_graphics->width()-m_rect.width()*8-m_itemBlocks*8):
			              ( m_graphics->height()-m_rect.height()*8-m_itemBlocks*8);
}

/*----------------------------------------------------------------------------*/
/** @brief  Scroll a certain distance.
 *  @param distance [in] Distance to move, either positive or negative
 *  @param flow [in] Do we need to flow afterwards, or stop immediately
 *  @return true when done
 */
bool CswypeDialog::scrollRelative( double distance, bool flow)
{
	lock();

	if ( distance >-0.1 && distance<0.1)
	{
	    unlock();
		return false;
	}
	int db =(int)(m_scrollPosition+0.5);
	m_scrollPosition +=distance;
	if ( m_scrollPosition<0)
	{
		m_scrollPosition =0;
		m_speeding.clear();
		flow =false;
	}
	double mx=scrollMax();
	if ( m_scrollPosition >mx)
	{
		m_scrollPosition =mx;
		m_speeding.clear();
		flow =false;
	}
	if ( flow)
	{
		m_speeding.addPosition( m_scrollPosition);
	}
	if ( (int)(m_scrollPosition+0.5) !=db)
	{
		m_repaint =true;
		calculateSurfacePosition();
	}
	unlock();
	return true;
}

/*----------------------------------------------------------------------------*/
/** Remove all buffers to see. */
void CswypeDialog::clean()
{
    Cgraphics::m_defaults.log("CswypeDialog::clean  swype clear %s <<", typeid(this).name());
    lock();

    try
    {
        m_spares = m_swypeObjects;
        m_swypeObjects.clear();
        m_firstUnitPainted =0;
        m_lastUnitPainted =0;
        m_firstOptionalUnit =0;
        m_lastOptionalUnit =0;
    }
    catch (...)
    {
        Cgraphics::m_defaults.log("ERROR CswypeDialog::clear %s", typeid(this).name());
    }

    unlock();
    Cgraphics::m_defaults.log("CswypeDialog::clean  swype clear >>");
}

/*----------------------------------------------------------------------------*/
/// @brief Calculate a new paintedArea.
void CswypeDialog::resetPaintedArea()
{
	lock();
	m_firstUnitPainted =m_firstVisibleUnit;
	m_lastUnitPainted =m_firstVisibleUnit;
	m_firstOptionalUnit =0;
	m_lastOptionalUnit =0;
    m_spares = m_swypeObjects;
    m_swypeObjects.clear();

	calculateItemRect();
	calculateSurfacePosition();
	m_repaint =true;
	unlock();
}

/*----------------------------------------------------------------------------*/
/** @brief Make sure the item is visible.
 *  @param row [in] For horizontal it is the column, for vertical scrolling it is a row.
 */
void CswypeDialog::makeSureRowVisible( int row, int time)
{
	// Check if item y is on screen.
	int pixels =itemBlocks()*8;
	if ( pixels == 0)
	{
		return;
	}
	lock();

    row =row*pixels;
	if ( m_horizontal)
	{
		int width =(m_rect.width()*8)/pixels; // Number of items horizontal
		int margin =(width*2)/3;
		if ( row<(int)m_scrollPosition)
		{
			scrollToPixel( row-pixels*(width/3), time);
		}
		if ( row>(int)m_scrollPosition+m_rect.width()*8-pixels)
		{
			scrollToPixel( row-pixels*margin, time);
		}
	}
	else
	{
		int height =(m_rect.height()*8)/pixels;	// Number of items vertical
		int margin =(height*2)/3;

		if ( row<(int)(m_scrollPosition+0.5))
		{
			scrollToPixel( row-pixels*(height/3), time);
		}
		else if ( row>(int)(m_scrollPosition+0.5)+m_rect.height()*8-pixels)
		{
			scrollToPixel( row-pixels*margin, time);
		}
	}
	unlock();
}

/*----------------------------------------------------------------------------*/
/** @brief Stop the swyping and set to top */
void CswypeDialog::topStop()
{
    lock();
	scrollIndex(0,0);
	m_speeding.clear();
	unlock();
}

/*----------------------------------------------------------------------------*/
/** @brief Is the row visible?
 *  @param row [in] For horizontal it is the column, for vertical scrolling it is a row.
 *  @return TRUE on visible.
 */
bool CswypeDialog::isRowVisible( int row)
{
	lock();

	bool retVal = true;
	// Check if item y is on screen.
	int pixels =itemBlocks()*8;

	if ( m_horizontal)
	{
		row=row*pixels;
		if ( row<(int)(m_scrollPosition+0.5))
		{
			retVal = false;
		}
		else if ( row>(int)(m_scrollPosition+0.5)+m_rect.width()*8-pixels)
		{
			retVal = false;
		}
	}
	else
	{
		row=row*pixels;
		if ( row<(int)(m_scrollPosition+0.5))
		{
			retVal = false;
		}
		else if ( row>(int)(m_scrollPosition+0.5)+m_rect.height()*8-pixels)
		{
			retVal = false;
		}
	}
	unlock();
	return retVal;
}

/*----------------------------------------------------------------------------*/
/// @brief  Depending on the scroll offset and painted area, we calculate the drawing area.
void CswypeDialog::calculateSurfacePosition()
{
	int nrRows =(int)rows();
	int siz;
	if ( m_horizontal)
	{
		siz =m_itemRect.width()*8;
		m_firstVisibleUnit =(int)(m_scrollPosition/siz);
		m_lastVisibleUnit =(int)((m_scrollPosition+m_rect.width()*8+siz-1)/siz);
	}
	else
	{
		siz =m_itemRect.height()*8;
		m_firstVisibleUnit =(int)(m_scrollPosition/siz);
		m_lastVisibleUnit =(int)((m_scrollPosition+m_rect.height()*8+siz-1)/siz);
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
}

/*----------------------------------------------------------------------------*/
/// @brief Set index to a certain item in the list.
/// @return True if changed.
bool CswypeDialog::scrollIndex( double row, int time)
{
	bool retVal =scrollToPixel( (int)(row*((float)itemBlocks()*8.0)), time);
	return retVal;
}

/*----------------------------------------------------------------------------*/
/** @brief Calculate number of rows we have.
 *  @return Number of rows.
 */
size_t CswypeDialog::rows()
{
    return 0;
}

/*----------------------------------------------------------------------------*/
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
	lock();

	//double speed;
	switch ( CdialogEvent::Instance()->getStatus())
	{
	case MOUSE_START_DRAG:
	case MOUSE_START_SCROLL_OR_DRAG:
		m_dialogSpeed =0;
		m_speeding.clear();
		break;

	case MOUSE_RELEASED:
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
			double relative =offset-m_scrollPosition;
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
	unlock();
	return m_alive;
}

/*----------------------------------------------------------------------------*/
bool CswypeDialog::hasReachedEndPosition()
{
	return m_endPosition == m_startPosition;
}

/*----------------------------------------------------------------------------*/
void CswypeDialog::setDirection(bool horizontal)
{
	m_horizontal = horizontal;
}

/*----------------------------------------------------------------------------*/
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
Estatus CswypeDialog::onButton( SDLMod mod, keybutton sym)
{
	(void)mod;
	(void)sym;
	Estatus stat =DIALOG_EVENT_OPEN;
	return stat;
}

/*----------------------------------------------------------------------------*/
/** @brief Set the height of a single item
 *  @param rowHeight [in] Set item blocks
 */
void CswypeDialog::setItemBlocks( int rowHeight)
{
	lock();

	if (m_itemBlocks==0 || m_itemBlocks>400)
	{
		m_itemBlocks=5;
	}
	m_itemBlocks =rowHeight;
	calculateItemRect();

	unlock();
}

/*----------------------------------------------------------------------------*/
void CswypeDialog::setFirstKey( keybutton key)
{
    m_firstKey = key;
}

/*----------------------------------------------------------------------------*/
/** Decide which key in a scroll dialog.
 *  @param p [in] Position of button
 */
keybutton CswypeDialog::findButton( const Cpoint &p)
{
    lock();
	keybutton retVal = KEY_NONE;

	if ( m_visible)
	{
		m_dialogSpeed =0;
		Cpoint q=p/8;
		if ( m_rect.inside( q)==true && m_visible ==true)
		{
			int item;
			if (m_horizontal)
			{
				item = (int)((p.x-m_rect.left()*8+m_scrollPosition)/(8*itemBlocks()));
			}
			else
			{
				item = (int)((p.y-m_rect.top()*8+m_scrollPosition)/(8*itemBlocks()));
			}

			retVal = (keybutton)(m_firstKey+(item%m_nrFkeys));
			CLICK_SOUND();
		}
	}
	unlock();

	return retVal;
}

/*----------------------------------------------------------------------------*/
/// @brief Roll the wheel in the middle.
void CswypeDialog::wheelUp( int mx, int my)
{
	(void)mx;
	(void)my;
	scrollRelative(-2, false);
}

/*----------------------------------------------------------------------------*/
double CswypeDialog::getScrollIndex()
{
    lock();
    int blocks = itemBlocks();
    int retVal = blocks ? m_scrollPosition/(double)(itemBlocks()*8):0;
    unlock();
    return retVal;
}

/*----------------------------------------------------------------------------*/
/// @brief Roll the wheel in the middle.
void CswypeDialog::wheelDown( int mx, int my)
{
	(void)mx;
	(void)my;
	scrollRelative(+2, false);
}

/*----------------------------------------------------------------------------*/
/// @brief constructor.
Cspeeding::Cspeeding()
: m_measurements(0)
, m_previousTime(0)
{
	m_friction =Cgraphics::m_defaults.swype_friction;
}

/*----------------------------------------------------------------------------*/
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
	}
	else
	{
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
}

/*----------------------------------------------------------------------------*/
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
	double speed = totalSpeed*1000.0f/(1000.0f+(delta_t*m_friction)/10.0f);
	//totalSpeed/=(1+m_friction*y);
	return speed;
}

/*----------------------------------------------------------------------------*/
/** @brief Paint one item in the list again
 *  @param row [in] What item to paint
 */
void CswypeDialog::invalidate( int row)
{
	lock();
	try
	{
	if ( row<0)
	{
		m_repaint =true;
	}
	else
	{
		for ( auto obj : m_swypeObjects)
		{
			if (obj->index ==row)
			{
				m_mainGraph->setRenderArea( obj->getTexture());
				Crect rect( 0, 0, obj->destination.width(), obj->destination.height());
				onPaintUnit( row, rect);
				m_mainGraph->setRenderArea( NULL);
				m_repaint =true;
				break;
			}
		}
	}
	}
	catch (...)
	{
	    Cgraphics::m_defaults.log("ERROR CswypeDialog::invalidate");
	}
	unlock();
}

/** @brief Change cursor to other position
 *  @param row [in] New cursor position
 */
void CswypeDialog::setCursor( int unit)
{
	lock();

    m_speeding.clear();
	if ( unit ==m_cursor)
	{
		invalidate( m_cursor);
		makeSureRowVisible( unit, 1000);
	}
	else
	{
		int prev =m_cursor;
		m_cursor =unit;

		invalidate( prev);
		invalidate( unit);
		if ( unit>=0)
		{
			makeSureRowVisible( unit, 1000);
		}
	}
	unlock();
}

/*----------------------------------------------------------------------------*/
void CswypeDialog::enableDrag( bool drag)
{
	m_dragEnable=drag;
}

/*----------------------------------------------------------------------------*/
bool CswypeDialog::isScrollDragDialog( const Cpoint &p)
{
	(void)p;
	return m_dragEnable;
}

/*----------------------------------------------------------------------------*/
bool CswypeDialog::isHorizontalScrollDialog( const Cpoint &p)
{
	(void)p;
	return m_horizontal;
}

/*----------------------------------------------------------------------------*/
int CswypeDialog::getDragIndex()
{
	return m_dragIndex;
}

/*----------------------------------------------------------------------------*/
void CswypeDialog::repaint()
{
	m_repaint=true; CswypeDialog::onPaint();
}

/*----------------------------------------------------------------------------*/
void CswypeDialog::setMargin( double margin)
{
	m_endMargin =(int)( margin*8.0f*itemBlocks());
}

/*----------------------------------------------------------------------------*/
/** @brief Find object for a certain point. Not in sub-dialogs!
 *  @return Object found at certain location
 */
CdialogObjectPtr CswypeDialog::findObject( const Cpoint &p)
{
    lock();
	CdialogObjectPtr retVal;

	try
	{
	    Cpoint q=p/8;
        if ( m_rect.inside( q) && m_visible)
        {
            int itemPixelHeight =8*itemBlocks();
            int row =(int)((p.y-m_rect.top()*8+m_scrollPosition)/itemPixelHeight);
            if ( row >=0 && row<(int)rows())
            {
                retVal = m_swypeObjects.at(row)->getDialogObject();
            }
            m_dragIndex =row;
            m_object->m_rect.setTop( (int)(m_rect.top()+(m_dragIndex*itemPixelHeight-m_scrollPosition)/8) );
            retVal = m_object;
        }
	}
    catch (...)
    {
        Cgraphics::m_defaults.log("ERROR CswypeDialog::findObject");
    }
	unlock();
	return retVal;
}

/*----------------------------------------------------------------------------*/
/** Paint on normal background
 *  @param row [in] What to paint
 *  @param location [in] Where to paint it
 */
void CswypeDialog::onPaintBackground( int row, const Crect &location)
{
    lock();
	(void)row;
	Cgraphics *previous =m_graphics;
	Cdialog *parent =m_parent;
	m_graphics =m_mainGraph;
	m_parent =NULL;
	m_object->setGraphics(m_mainGraph);
	m_graphics =m_mainGraph;
	onPaintUnit( m_dragIndex, location);
	m_graphics =previous;
	m_parent =parent;
    unlock();
}

/*----------------------------------------------------------------------------*/
/** @brief Scroll to a certain row (or column).
 *  @param row [in] What row for vertical scrolling, what column for horizontal scrolling.
 *  @param time [in] Time to get to that row.
 *  @return If we have a go!
 */
bool CswypeDialog::scrollToRow( int row, int time)
{
	return scrollToPixel( row*itemBlocks()*8, time);
}
