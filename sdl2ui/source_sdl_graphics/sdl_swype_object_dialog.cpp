/*============================================================================*/
/**  @file       sdl_2Dswype_object_dialog.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Swype dialog with list.
 **
 **  Create a 2D list.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              C2DswypeDialog
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
#include "sdl_swype_object_dialog.h"
#include "sdl_surface.h"

/** Dialog constructor to scroll objects left-right or up-down
 *  @param parent [in] Parent dialog.
 *  @param rect [in] Rectangle to use.
 *  @param firstKey [in] Key to use.
 *  @param span [in] Number of keys horizontal.
 *  @param horizontal [in] True for horizontal, false for vertical swype dialog.
 */
CswypeObjectDialog::CswypeObjectDialog( Cdialog *parent, const Crect &rect, keybutton firstKey, int span,
		bool horizontal)
: CswypeDialog( parent, rect, firstKey, horizontal)
{
    (void)span;
	if ( horizontal)
	{
		m_nrFkeys =200;
	}
	//m_object =new C2DscrollObject( this, 0, Csize(rect.width()/span, itemBlocks()));
	//m_listSize =(3*m_rect.height()*m_span)/itemBlocks();
}

/** @brief Destructor */
CswypeObjectDialog::~CswypeObjectDialog()
{
}

/** @brief Get which index it is for the object under the mouse.
 *  @param sym [in] Which button found on screen.
 *  @return Item index found in the list. */
int CswypeObjectDialog::getScrollIndex( keybutton sym)
{
    (void)sym;
	return 0;
}


/** Decide which key in a scroll dialog.
 * 	@param p [in] What position on screen.
 * 	@return What key is found on screen.
 */
keybutton CswypeObjectDialog::findButton( const Cpoint &p)
{
    (void)p;
//	m_dialogSpeed =0;
//	Cpoint q=p/8;
//	if ( m_rect.inside( q)==true && m_visible ==true)
//	{
//		if ( m_horizontal)
//		{
//			int column =(p.x-m_rect.left()*8+m_scroll)/(8*itemBlocks());
//			int row =(q.y-m_rect.top())/(m_rect.height()/m_span);
//			int item =m_span*column+row;
//			if ( item >=(int)maxPosition())
//			{
//				return KEY_NONE;
//			}
//			keybutton key =(keybutton)(m_firstKey+(item%m_nrFkeys));
//			AUDIO_CLICK();
//			return key;
//		}
//		else
//		{
//			int row =(p.y-m_rect.top()*8+m_scroll)/(8*itemBlocks());
//			int column =(q.x-m_rect.left())/(m_rect.width()/m_span);
//			int item =m_span*row+column;
//			if ( item >=(int)size())
//			{
//				return KEY_NONE;
//			}
//			keybutton key =(keybutton)(m_firstKey+(item%m_nrFkeys));
//			AUDIO_CLICK();
//			return key;
//		}
//	}
	return KEY_NONE;
}

/** @brief Change cursor to other position.
 *  @param index [in] What index to set.
 */
//void CswypeObjectDialog::setCursor( int index)
//{
//	CswypeDialog::setCursor( index/m_span);
//	m_realIndex =index;
//	invalidate();
//}

/** @brief Find object for a certain point. Not in sub-dialogs!
 *  @param p [in] Find which object is on that location.
 *  @return Pointer to object. Not re-entrant, please use once.
 */
CdialogObjectPtr CswypeObjectDialog::findObject( const Cpoint &p)
{
	CdialogObjectPtr object;
    (void)p;
//	for ( CdialogObject *obj : m_swypeObjects)
//	{
//
//	}
	return object;
}

/** Paint again one of the items.
 *  @param index [in] What to paint again.
 */
//void CswypeObjectDialog::invalidateIndex( int index)
//{
//	m_repaint =true;
//	for ( int n=0; n<m_visibleBuffers; n++)
//	{
//		CswypeObject *obj =m_swypeObjects[ n];
//		if ( obj->index ==index)
//		{
//			// ( "C2DswypeDialog::invalidate  index=%d", index);
//			m_mainGraph->setRenderArea( obj->texture);
//			if ( m_horizontal)
//			{
//				Crect rect( 0, 0, itemBlocks(), m_rect.height());
//				onPaintUnit( index, rect);
//		  	}
//			else
//			{
//				Crect rect( 0,0, m_rect.width(), itemBlocks());
//				onPaintUnit( index, rect);
//			}
//			m_mainGraph->setRenderArea( NULL);
//			return;
//		}
//	}
//}

/** Paint on normal background.
 *  @param index [in] Which element to paint.
 *  @param location [in] Where to paint this one.
 */
//void CswypeObjectDialog::onPaintBackground( int index, const Crect &location)
//{
//#ifdef USE_SDL2
//	(void)index;
//	(void)location;
//	assert(0);
//#else
//	(void)index;
//	Cgraphics *previous =m_graphics;
//	//m_object->m_parent =NULL;
//	Cdialog *parent =m_parent;
//	m_graphics =m_mainGraph;
//	m_parent =NULL;
//	m_object->m_graphics =m_mainGraph;
//	m_graphics =m_mainGraph;
//
//	onPaintUnit( m_dragIndex, location);
//	m_graphics =previous;
//	m_parent =parent;
//#endif
//}

//void CswypeObjectDialog::setSpan( int x)
//{
//	m_span =x;
//	resetPaintedArea();
//	clean();
//	calculateItemRect();
//}

/** @brief make sure this is the top/left item to show.
 *  @param index [in] which one to show.
 *  @param time [in] how much time left.
 */
//bool CswypeObjectDialog::scrollToIndex( int index, int time)
//{
//	return CswypeDialog::scrollToRow( index/m_span, time);
//}

/// @brief  Depending on the scroll offset and painted area, we calculate the drawing area.
//void CswypeObjectDialog::calculateSurfacePosition()
//{
//	int size;
//	int row =(int)rows();
//	if ( m_horizontal)
//	{
//		size =m_itemRect.width()*8;
//		m_firstVisibleUnit =m_span*(int)(m_scroll/size);
//		m_lastVisibleUnit =(m_scroll+m_rect.width()*8+size-1)/size;
//	}
//	else
//	{
//		size =m_itemRect.height()*8;
//		m_firstVisibleUnit =m_span*(int)(m_scroll/size);
//		m_lastVisibleUnit =m_span*(int)((m_scroll+m_rect.height()*8+size-1)/size);
//	}
//	if ( m_lastVisibleUnit >row*m_span)
//	{
//		m_lastVisibleUnit =row*m_span;
//	}
//	m_firstOptionalUnit =m_firstVisibleUnit-m_listSize/(4*m_span);
//	m_lastOptionalUnit =m_lastVisibleUnit+m_listSize/(4*m_span);
//	if ( m_firstOptionalUnit<0)
//	{
//		m_firstOptionalUnit=0;
//	}
//	if ( m_lastOptionalUnit>row*m_span)
//	{
//		m_lastOptionalUnit =row*m_span;
//	}
//}

/// @brief Paint all things from textures to the real screen.
//void CswypeObjectDialog::onPaint()
//{
//			rct.w =m_swypeObjects[0]->destination.width()*8;
//			for ( int n=0; n<size; n++)
//			{
//				CswypeObject *obj =m_swypeObjects[n];
//				rct.x =offx+width*(int)(obj->index/m_span)-(int)m_scroll;
//				int y =obj->index%m_span;
//				rct.y =y*height/m_span;
//				rct.h =(y+1)*height/m_span-rct.y;
//				rct.y +=offy;
//				renderCopy( m_swypeObjects[n]->texture, &rct);
//			}
//		}
//		else
//		{
//			int height =itemBlocks()*8;
//			int width =m_rect.width()*8;
//			// Draw horizontal items
//			rct.h =m_swypeObjects[0]->destination.height()*8;
//			for ( int n=0; n<size; n++)
//			{
//				CswypeObject *obj =m_swypeObjects[n];
//				rct.y =offy+height*(int)(obj->index/m_span)-(int)m_scroll;
//				int x =obj->index%m_span;
//				rct.x =x*width/m_span;
//				rct.w =(x+1)*width/m_span-rct.x;
//				rct.x +=offx;
//				renderCopy( m_swypeObjects[n]->texture, &rct);
//			}
//		}
//	}
//	// Put back old viewport
//	m_mainGraph->setViewport(NULL);
//	m_mainGraph->update();
//}

//void CswypeObjectDialog::paintSurfaceTop()
//{
//	CswypeObject *obj =insertAtBegin();
//	// Actual painting
//	m_graphics->setRenderArea( obj->texture);
//	onPaintUnit( obj->index, m_itemRect);
//	m_graphics->setRenderArea( NULL);
//}

//bool CswypeObjectDialog::paintSurfaceRight()
//{
//	CswypeObject *obj =insertAtEnd();
//	if ( !obj)
//	{
//		return false;
//	}
//	// Actual painting
//	m_mainGraph->setRenderArea( obj->texture);
//	onPaintUnit( obj->index, m_itemRect);
//	m_mainGraph->setRenderArea( NULL);
//	return true;
//}

//bool CswypeObjectDialog::paintSurfaceLeft()
//{
//	CswypeObject *obj =insertAtBegin();
//	if ( !obj)
//	{
//		return false;
//	}
//	// Actual painting
//	m_graphics->setRenderArea( obj->texture);
//	onPaintUnit( obj->index, m_itemRect);
//	m_graphics->setRenderArea( NULL);
//	return true;
//}

//void CswypeObjectDialog::paintSurfaceBottom()
//{
//	CswypeObject *obj =insertAtEnd();
//	// Actual painting
//	m_graphics->setRenderArea( obj->texture);
//	onPaintUnit( obj->index, m_itemRect);
//	m_graphics->setRenderArea( NULL);
//}
