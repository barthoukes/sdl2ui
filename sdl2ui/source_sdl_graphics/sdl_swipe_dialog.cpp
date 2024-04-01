/*============================================================================*/
/**  @file       sdl_swipe_dialog.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Swype dialog 1D list
 **
 **  Create a default swype list 1 dimensional.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CswipeDialog
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2011, 2014, 2020
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
#include "sdl_swipe_dialog.h"

/** Dialog to scroll objects left-right or up-down */
template <class Sprite>
CswipeDialog<Sprite>::CswipeDialog( Cdialog *parent, const Crect &rect, keybutton firstKey, bool horizontal)
: Cdialog( parent, "scrollRect", rect)
, m_itemRect( 0,0,rect.width(),itemBlocks())
, m_listSize(0)
, m_horizontal(horizontal)
, m_sizes(0)
, m_scrollPosition(0)
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
	m_object =new std::shared_ptr<Sprite>( this, 0, Csize(m_rect.width(), itemBlocks()));

	if ( m_graphics !=NULL && m_isOwner ==true)
	{
		delete m_graphics;
		m_isOwner =false;
	}
	m_graphics =m_mainGraph;
	m_listSize =(3*m_rect.height())/itemBlocks();
	calculateSurfacePosition();
	setRect( rect);
}

/** @brief Change or init the rectangle to this size.
 *  @param rect [in] What position and size to use.
 */
template <class Sprite>
void CswipeDialog<Sprite>::setRect( const Crect &rect)
{
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
}

template <class Sprite>
CswipeDialog<Sprite>::~CswipeDialog()
{
	CswipeDialog::clean();
	if ( m_graphics && m_isOwner)
	{
		delete m_graphics;
	}
	m_graphics =NULL;
	m_isOwner =false;
	if ( m_object)
	{
		delete m_object;
		m_object =NULL;
	}
}

template <class Sprite>
void CswipeDialog<Sprite>::invalidate()
{
	resetPaintedArea();
}

/** @brief Get which index it is for the object under the mouse */
template <class Sprite>
int CswipeDialog<Sprite>::getScrollIndex( keybutton sym)
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
template <class Sprite>
void CswipeDialog<Sprite>::onPaintSurfaceHorizontal( bool optional)
{
	static int count =5;

	for (int mx=0; mx<200; mx++) // Remove endless loop.
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
			if ( !optional) return;
			if ( --count>0) return;
			paintSurfaceTop();
			count =50;
			return;

		case PAINT_BOTTOM_OPTIONAL:
			if ( !optional) return;
			if ( --count>0) return;
			paintSurfaceBottom();
			count =50;
			return;

		case PAINT_LEFT_OPTIONAL:
			if ( !optional) return;
			if ( --count>0) return;
			paintSurfaceLeft();
			count =50;
			return;

		case PAINT_RIGHT_OPTIONAL:
			if ( !optional) return;
			if ( --count>0) return;
			count =50;
			paintSurfaceRight();
			return;

		case PAINT_READY:
			return;
		default:
			break;
		}
	}
}

/** @brief Calculate the square of a single rectangle in our swipe dialog */
template <class Sprite>
void CswipeDialog<Sprite>::calculateItemRect()
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
template <class Sprite>
Epainted CswipeDialog<Sprite>::visiblePainted()
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
template <class Sprite>
void CswipeDialog<Sprite>::onPaintSurfaceVertical( bool optional)
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
			count=50;
			m_repaint =true;
			break;

		case PAINT_BOTTOM:
			paintSurfaceBottom();
			count=50;
			m_repaint =true;
			break;

		case PAINT_TOP_OPTIONAL:
			if ( !optional) return;
			if ( --count>0) return;
			count=50;
			paintSurfaceTop();
			m_repaint =true;
			return;

		case PAINT_BOTTOM_OPTIONAL:
			if ( !optional) return;
			if ( --count>0) return;
			count=50;
			paintSurfaceBottom();
			m_repaint =true;
			return;

		case PAINT_READY:
			count=50;
			return;
		default:
			break;
		}
	}
}

/** @brief Insert a texture at the begin of the list
 *  @return Pointer to (new) object
 */
template <class Sprite>
std::shared_ptr<Sprite> CswipeDialog<Sprite>::insertAtBegin()
{
	std::shared_ptr<Sprite> obj;

	// Erase first item?
	m_visibleSize =m_lastVisibleUnit-m_firstVisibleUnit+1;

	if ( m_visibleBuffers< m_validBuffers)
	{
		// Re-use last image in list if too many
		obj =m_swypeObjects[ m_validBuffers-1];
		m_swypeObjects.erase( m_swypeObjects.begin()+m_validBuffers-1);
		m_swypeObjects.insert( m_swypeObjects.begin(), obj);
		m_visibleBuffers++;
	}
	else if ( m_visibleBuffers>= m_visibleSize*3)
	{
		// Re-use last valid image in list
		obj =m_swypeObjects[ m_validBuffers-1];
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
		obj =std::make_shared<CswypeObject>( m_itemRect, 0, m_itemRect);
		m_validBuffers++;
		m_visibleBuffers++;
		m_swypeObjects.insert( m_swypeObjects.begin(), obj);
	}
	obj->index =--m_firstUnitPainted;
	return obj;
}

/** @brief Insert a texture at the end of the list
 *  @return Pointer to (new) object
 */
template <class Sprite>
std::shared_ptr<Sprite> CswipeDialog<Sprite>::insertAtEnd()
{
	std::shared_ptr<Sprite> obj;
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
	else if ( m_visibleBuffers>= m_visibleSize*3)
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
		obj =std::make_shared<CswypeObject>( m_itemRect, 0, m_itemRect);
		m_swypeObjects.push_back( obj);
		m_validBuffers++;
		m_visibleBuffers++;
	}
	obj->index =m_lastUnitPainted++;
	return obj;
}

/** @brief Insert a texture at the end of the list
 *  @return Pointer to (new) object
 */
template <class Sprite>
void CswipeDialog<Sprite>::popFirst()
{
	m_visibleSize =m_lastVisibleUnit-m_firstVisibleUnit+1;
	// Erase first item?
	if (m_visibleSize <=0)
	{
		return;
	}
	m_swypeObjects.erase( m_swypeObjects.begin());
	int first =m_swypeObjects[0]->index;
	if ( first>m_firstUnitPainted)
	{
		m_firstUnitPainted =first;
	}
	m_validBuffers--;
	m_visibleBuffers--;
}

/** @brief Paint a row on top of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
template<class Sprite>
void CswipeDialog<Sprite>::paintSurfaceTop()
{
	paintSurfaceLeft();
}

/** @brief Paint a column on the left of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
template<class Sprite>
void CswipeDialog<Sprite>::paintSurfaceLeft()
{
	std::shared_ptr<Sprite> obj =insertAtBegin();
	// Actual painting
	m_graphics->setRenderArea( obj->texture);
	obj->onPaint( obj->index, m_itemRect);
	//onPaintUnit( obj->index, m_itemRect);
	m_graphics->setRenderArea( NULL);
}

/** @brief Paint a row on the bottom of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
template<class Sprite>
void CswipeDialog<Sprite>::paintSurfaceBottom()
{
	paintSurfaceRight();
}

/** @brief Paint a column on the right of a scrolling dialog.
 *  @param row [in] Which row or element.
 *  @param location [in] Location in a temporary surface.
 */
template<class Sprite>
void CswipeDialog<Sprite>::paintSurfaceRight()
{
	std::shared_ptr<Sprite> obj =insertAtEnd();
	// Actual painting
	m_graphics->setRenderArea( obj->texture);
	obj->onPaint( obj->index, m_itemRect);
	m_graphics->setRenderArea( NULL); // Back to main window
}

/// @brief Paint all things.
template<class Sprite>
void CswipeDialog<Sprite>::onPaint()
{
	SDL_Rect rct;
	SDL_Rect dlg;
#if 1
	if ( !m_visible || !m_repaint)
	{
		return;
	}
#else
	CswipeDialog::clean();
#endif
	m_repaint =false;
	m_mainGraph->setPixelOffset(0,0);
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
	m_mainGraph->setColour( m_backgroundColour);
	m_mainGraph->setViewport( &rct);
	m_mainGraph->bar( rct.x, rct.y, rct.x+rct.w, rct.y+rct.h);
	if ( m_visibleBuffers >0)
	{
		if ( m_horizontal)
		{
			int width =itemBlocks()*8;
			int x=rct.x+m_swypeObjects[0]->index*width-(int)m_scrollPosition;
			// Draw horizontal items
			rct.w =(Uint16)(m_swypeObjects[0]->destination.width()*8);
			rct.h =(Uint16)(m_swypeObjects[0]->destination.height()*8);
			for ( int n=0; n<m_lastUnitPainted-m_firstUnitPainted; n++)
			{
				rct.x =(Sint16)x;
				x +=width;
				renderCopy( m_swypeObjects[n]->texture, &rct);
			}
		}
		else
		{
			int height =itemBlocks()*8;
			int y=rct.y+m_swypeObjects[0]->index*height-(int)m_scrollPosition;
			// Draw horizontal items
			rct.w =(Uint16)(m_itemRect.width()*8);
			rct.h =(Uint16)height;
			for ( int n=0; n<m_visibleBuffers; n++)
			{
				rct.y =(Sint16)y;
				y +=height;
				renderCopy( m_swypeObjects[n]->texture, &rct);
			}
		}
	}
	// Put back old viewport
	m_mainGraph->setViewport( NULL);
	m_mainGraph->update(dlg.x, dlg.y, dlg.w, dlg.h);
}

/** @brief Render a surface
 *  @param surface [in] Swype unit to paint
 *  @param rect [in] Rectangle to use
 */
template<class Sprite>
void CswipeDialog<Sprite>::renderCopy( SDL_Surface *surface, SDL_Rect *rect)
{
	m_mainGraph->surface( surface, rect->x, rect->y, rect->w, rect->h);
}

/** @brief Clean dialog afterwards */
template<class Sprite>
void CswipeDialog<Sprite>::onCleanup()
{
	CswipeDialog::clean();
	m_firstUnitPainted =0;
	m_lastUnitPainted =0;
	m_firstVisibleUnit =0;
	m_lastVisibleUnit =0;
}

/// @brief  Scroll to absolute position.
/// @param offset [in] Where to scroll to in absolute pixels
/// @param time [in] How long it takes to go there
/// @return True if changed value.
template<class Sprite>
bool CswipeDialog<Sprite>::scrollToPixel( int offset, int time)
{
	int maxScroll =scrollMax();
	offset =gLimit(offset, 0,maxScroll);
	if ( time<=0)
	{
		m_moveTime =0;
		return scrollRelative( (double)offset-m_scrollPosition, false);
	}
	else
	{
		m_startPosition =(int)m_scrollPosition;
		m_endPosition =offset;
		m_timer.setTime( time, 100,3);
		m_moveTime =time;
		return true;
	}
}

/** @brief Calculate the maximum scroll distance.
 *  @return Number of rows in the scrolling dialog.
 */
template<class Sprite>
int CswipeDialog<Sprite>::scrollMax()
{
	m_sizes =rows();
	int mx =m_sizes*itemBlocks()*8+m_endMargin-
			( m_horizontal ? (m_rect.width()*8):(m_rect.height()*8) );
	if ( mx<0) mx=0;
	return mx;
}

/// @brief Calculate the maximum scroll distance
/// @return Maximum scroll distance for left edge of screen rectangle
template<class Sprite>
int CswipeDialog<Sprite>::surfaceMax()
{
	return m_horizontal ? ( m_graphics->width()-m_rect.width()*8-m_itemBlocks*8):
			              ( m_graphics->height()-m_rect.height()*8-m_itemBlocks*8);
}

/** @brief  Scroll a certain distance.
 *  @param distance [in] Distance to move, either positive or negative
 *  @param flow [in] Do we need to flow afterwards, or stop immediately
 *  @return true when done
 */
template<class Sprite>
bool CswipeDialog<Sprite>::scrollRelative( double distance, bool flow)
{
	if ( distance >-0.1 && distance<0.1)
	{
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
	if ( (int)(m_scrollPosition+0.5) ==db)
	{
		return true;
	}
	m_repaint =true;
	calculateSurfacePosition();
	return true;
}

/** Remove all buffers to see. */
template<class Sprite>
void CswipeDialog<Sprite>::clean()
{
	m_swypeObjects.clear();
	Cgraphics::m_defaults.log("CswipeDialog::clean  swype clean");
	m_validBuffers =0;
	m_visibleBuffers =0;
	m_firstUnitPainted =0;
	m_lastUnitPainted =0;
	m_firstOptionalUnit =0;
	m_lastOptionalUnit =0;
}

/// @brief Calculate a new paintedArea.
template<class Sprite>
void CswipeDialog<Sprite>::resetPaintedArea()
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
template<class Sprite>
void CswipeDialog<Sprite>::makeSureRowVisible( int row, int time)
{
	// Check if item y is on screen.
	int pixels =itemBlocks()*8;
	if ( pixels == 0)
	{
		return;
	}
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
			return;
		}
		if ( row>(int)(m_scrollPosition+0.5)+m_rect.height()*8-pixels)
		{
			scrollToPixel( row-pixels*margin, time);
		}
	}
}

/** @brief Stop the swyping and set to top */
template<class Sprite>
void CswipeDialog<Sprite>::topStop()
{
	scrollIndex(0,0);
	m_speeding.clear();
}

/** @brief Is the row visible?
 *  @param row [in] For horizontal it is the column, for vertical scrolling it is a row.
 *  @return TRUE on visible.
 */
template<class Sprite>
bool CswipeDialog<Sprite>::isRowVisible( int row)
{
	// Check if item y is on screen.
	int pixels =itemBlocks()*8;

	if ( m_horizontal)
	{
		row=row*pixels;
		if ( row<(int)(m_scrollPosition+0.5))
		{
			return false;
		}
		if ( row>(int)(m_scrollPosition+0.5)+m_rect.width()*8-pixels)
		{
			return false;
		}
	}
	else
	{
		row=row*pixels;
		if ( row<(int)(m_scrollPosition+0.5))
		{
			return false;
		}
		if ( row>(int)(m_scrollPosition+0.5)+m_rect.height()*8-pixels)
		{
			return false;
		}
	}
	return true;
}

/// @brief  Depending on the scroll offset and painted area, we calculate the drawing area.
template<class Sprite>
void CswipeDialog<Sprite>::calculateSurfacePosition()
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
	//printf("First=%d, Last=%d,  Optional=%d-%d\n", m_firstVisibleUnit, m_lastVisibleUnit, m_firstOptionalUnit, m_lastOptionalUnit);
}

/// @brief Set index to a certain item in the list.
/// @return True if changed.
template<class Sprite>
bool CswipeDialog<Sprite>::scrollIndex( double row, int time)
{
	bool retVal =scrollToPixel( (int)(row*((float)itemBlocks()*8.0)), time);
	return retVal;
}

/** @brief Calculate number of rows we have.
 *  @return Number of rows.
 */
template<class Sprite>
size_t CswipeDialog<Sprite>::rows()
{
    return 0;
}

/** @brief Calculate height of a single row/item.
 *  @return Height of a single item/row.
 */
template<class Sprite>
int CswipeDialog<Sprite>::itemBlocks()
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
template<class Sprite>
bool CswipeDialog<Sprite>::onLoop()
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
	return m_alive;
}

template<class Sprite>
bool CswipeDialog<Sprite>::hasReachedEndPosition()
{
	return m_endPosition == m_startPosition;
}


/** @brief Stop auto scroll of the swipe dialog */
template<class Sprite>
void CswipeDialog<Sprite>::clearSpeed()
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
template<class Sprite>
Estatus CswipeDialog<Sprite>::onButton( SDLMod mod, keybutton sym)
{
	(void)mod;
	(void)sym;
	Estatus stat =DIALOG_EVENT_OPEN;
	return stat;
}

/** @brief Set the height of a single item
 *  @param rowHeight [in] Set item blocks
 */
template<class Sprite>
void CswipeDialog<Sprite>::setItemBlocks( int rowHeight)
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
template<class Sprite>
keybutton CswipeDialog<Sprite>::findButton( const Cpoint &p)
{
	if ( m_visible ==false)
	{
		return KEY_NONE;
	}
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

		keybutton key =(keybutton)(m_firstKey+(item%m_nrFkeys));
		CLICK_SOUND();
		return key;
	}
	return KEY_NONE;
}

/// @brief Roll the wheel in the middle.
template<class Sprite>
void CswipeDialog<Sprite>::wheelUp( int mx, int my)
{
	(void)mx;
	(void)my;
	scrollRelative(-2, false);
}

/// @brief Roll the wheel in the middle.
template<class Sprite>
void CswipeDialog<Sprite>::wheelDown( int mx, int my)
{
	(void)mx;
	(void)my;
	scrollRelative(+2, false);
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
	double speed = totalSpeed*1000.0f/(1000.0f+(delta_t*m_friction)/10.0f);
	//totalSpeed/=(1+m_friction*y);
	return speed;
}

/** @brief Paint one item in the list again
 *  @param row [in] What item to paint
 */
template<class Sprite>
void CswipeDialog<Sprite>::invalidate( int row)
{
	if ( row<0 || m_visibleBuffers<=0)
	{
		m_repaint =true;
		return;
	}
	int idx;

	int sz=(int)m_swypeObjects.size();
	for ( idx=0; idx<sz; idx++)
	{
		const std::shared_ptr<CswypeObject> &obj =m_swypeObjects[ idx];
		if ( obj && obj->index ==row)
		{
			m_mainGraph->setRenderArea( obj->texture);
			Crect rect( 0, 0, obj->destination.width(), obj->destination.height());
			onPaintUnit( row, rect);
			m_mainGraph->setRenderArea( NULL);
			m_repaint =true;
			break;
		}
	}
}

/** @brief Change cursor to other position
 *  @param row [in] New cursor position
 */
template<class Sprite>
void CswipeDialog<Sprite>::setCursor( int unit)
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
template<class Sprite>
CdialogObjectPtr CswipeDialog<Sprite>::findObject( const Cpoint &p)
{
	Cpoint q=p/8;
	if ( m_rect.inside( q)==false || m_visible ==false)
	{
		return NULL;
	}
	int itemPixelHeight =8*itemBlocks();
	int row =(int)((p.y-m_rect.top()*8+m_scrollPosition)/itemPixelHeight);
	if ( row<0 || row>=(int)rows())
	{
		return NULL;
	}
	m_dragIndex =row;
	m_object->m_rect.setTop( (int)(m_rect.top()+(m_dragIndex*itemPixelHeight-m_scrollPosition)/8) );
	return m_object;
}

/** Paint on normal background
 *  @param row [in] What to paint
 *  @param location [in] Where to paint it
 */
template<class Sprite>
void CswipeDialog<Sprite>::onPaintBackground( int row, const Crect &location)
{
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
}

/** @brief Scroll to a certain row (or column).
 *  @param row [in] What row for vertical scrolling, what column for horizontal scrolling.
 *  @param time [in] Time to get to that row.
 *  @return If we have a go!
 */
template<class Sprite>
bool CswipeDialog<Sprite>::scrollToRow( int row, int time)
{
	return scrollToPixel( row*itemBlocks()*8, time);
}
