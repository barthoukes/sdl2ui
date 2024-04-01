/*============================================================================*/
/**  @file       sdl_2Dswype_dialog.cpp
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
#include <assert.h>
#include "sdl_audio.h"
#include "sdl_surface.h"
#include "sdl_swype_object.h"
#include "sdl_2Dswype_dialog.h"

/** Dialog constructor to scroll objects left-right or up-down
 *  @param parent [in] Parent dialog.
 *  @param rect [in] Rectangle to use.
 *  @param firstKey [in] Key to use.
 *  @param span [in] Number of keys horizontal.
 *  @param horizontal [in] True for horizontal, false for vertical swype dialog.
 */
C2DswypeDialog::C2DswypeDialog( Cdialog *parent, const Crect &rect, keybutton firstKey, int span,
		bool horizontal)
: CswypeDialog( parent, rect, firstKey, horizontal)
, m_span( (span>0) ? span:1)
, m_realIndex(0)
{
	if ( firstKey==KEY_MENU_ITEM1)
	{
		m_nrFkeys =span*40;
	}
	else if ( horizontal)
	{
		m_nrFkeys =span*(1+ rect.width()/itemBlocks());
		m_object =std::make_shared<C2DscrollObject>( this, 0, Csize(rect.width()/span, itemBlocks()));
		m_listSize =(3*m_rect.height()*m_span)/itemBlocks();
	}
	else
	{
		m_nrFkeys =span*(1+ rect.height()/itemBlocks());
		m_object =std::make_shared<C2DscrollObject>( this, 0, Csize(itemBlocks(), rect.height()/span));
		m_listSize =(3*m_rect.width()*m_span)/itemBlocks();
	}
}

/** @brief Destructor */
C2DswypeDialog::~C2DswypeDialog()
{
	CswypeDialog::clean();
	m_object = nullptr;
}

/** @brief Get which index it is for the object under the mouse.
 *  @param sym [in] Which button found on screen.
 *  @return Item index found in the list.
 */
int C2DswypeDialog::getScrollIndex( keybutton sym)
{
	if ( sym<m_firstKey || sym>m_firstKey+m_nrFkeys-1)
	{
		return -1;
	}
	int key =(sym-m_firstKey);
	int invisible;
	if ( m_horizontal)
	{
		invisible =(int)(m_scrollPosition/(itemBlocks()*8));
	}
	else
	{
		invisible =(int)(m_scrollPosition/(itemBlocks()*8));
	}
	invisible *=m_span;
	int firstKey =(invisible%m_nrFkeys);
	int offset =(key-firstKey+m_nrFkeys)%m_nrFkeys+invisible;
	return ( offset>=0 && offset<(int)maxPosition()) ? offset:-1;
}

/** @brief Calculate how many cells we have.
 *  @return Number of cells.
 */
size_t C2DswypeDialog::size()
{
	return 10;
}

/** @brief Calculate number of rows we have.
 *  @return Number of rows.
 */
size_t C2DswypeDialog::rows()
{
	return (int)((size()+m_span-1)/m_span);
}

/** @brief Make sure the item is visible.
 *  @param index [in] Which item we want to see.
 *  @param time [in] Time in milliseconds to arrive at location.
 */
void C2DswypeDialog::makeSureIndexVisible( int index, int time)
{
	makeSureRowVisible( index/m_span, time);
}

/**
 * @brief Calculate rectangle for 1 single item.
 */
void C2DswypeDialog::calculateItemRect()
{
	if ( m_horizontal)
	{
		m_itemRect =Crect( 0,0, itemBlocks(), m_rect.height()/m_span);
	}
	else
	{
		m_itemRect =Crect( 0,0, m_rect.width()/m_span, itemBlocks());
	}
}

/** Decide which key in a scroll dialog.
 * 	@param p [in] What position on screen.
 * 	@return What key is found on screen.
 */
keybutton C2DswypeDialog::findButton( const Cpoint &p)
{
	m_dialogSpeed =0;
	Cpoint q=p/8;
	if ( m_rect.inside( q)==true && m_visible ==true)
	{
		if ( m_horizontal)
		{
			int column =(int)((p.x-m_rect.left()*8+m_scrollPosition)/(8*itemBlocks()));
			int row =(q.y-m_rect.top())/(m_rect.height()/m_span);
			int item =m_span*column+row;
			if ( item >=(int)maxPosition())
			{
				return KEY_NONE;
			}
			keybutton key =(keybutton)(m_firstKey+(item%m_nrFkeys));
			CLICK_SOUND();
			return key;
		}
		else
		{
			int row =(int)((p.y-m_rect.top()*8+m_scrollPosition)/(8*itemBlocks()));
			int column =(q.x-m_rect.left())/(m_rect.width()/m_span);
			int item =m_span*row+column;
			if ( item >=(int)size())
			{
				return KEY_NONE;
			}
			keybutton key =(keybutton)(m_firstKey+(item%m_nrFkeys));
			CLICK_SOUND();
			return key;
		}
	}
	return KEY_NONE;
}

/** @brief Change cursor to other position.
 *  @param index [in] What index to set.
 */
void C2DswypeDialog::setCursor( int index)
{
	CswypeDialog::setCursor( index/m_span);
	m_realIndex =index;
	invalidate();
}

/** @brief Find object for a certain point. Not in sub-dialogs!
 *  @param p [in] Find which object is on that location.
 *  @return Pointer to object. Not re-entrant, please use once.
 */
CdialogObjectPtr C2DswypeDialog::findObject( const Cpoint &p)
{
	if ( m_horizontal)
	{
		Cpoint q=p/8;
		if ( m_rect.inside( q)==false)
		{
			return nullptr;
		}
		int itemPixelWidth =8*itemBlocks();
		int column =(int)((p.x-m_rect.left()*8+m_scrollPosition)/itemPixelWidth);
		int row =(q.y-m_rect.top())/(m_rect.height()/m_span);
		int item =row+column*m_span;
		if ( item>=(int)size())
		{
			return nullptr;
		}
		m_dragIndex =item;
		if (m_object.get())
		{
            m_object->setLeft( (int)(m_rect.left()+((column*itemPixelWidth)-m_scrollPosition)/8));
            m_object->setTop( m_rect.top()+row*m_rect.height()/m_span);
            m_object->setHeight( m_rect.top()+(row+1)*m_rect.height()/m_span);
            m_object->setWidth( itemBlocks());
		}
		return m_object;
	}
	else
	{
		Cpoint q=p/8;
		if ( m_rect.inside( q)==false)
		{
			return nullptr;
		}
		int itemPixelHeight =8*itemBlocks();
		int row =(int)((p.y-m_rect.top()*8+m_scrollPosition)/itemPixelHeight);
		int column =(q.x-m_rect.left())/(m_rect.width()/m_span);
		int item =row*m_span+column;
		if ( item>=(int)size())
		{
			return nullptr;
		}
		m_dragIndex =item;
		if (m_object.get())
		{
            m_object->setTop( (int)(m_rect.top()+((row*itemPixelHeight)-m_scrollPosition)/8));
            m_object->setLeft( m_rect.left()+column*m_rect.width()/m_span);
            m_object->setRight( m_rect.left()+(column+1)*m_rect.width()/m_span);
            m_object->setHeight( itemBlocks());
		}
		return m_object;
	}
}

/** Paint again one of the items.
 *  @param index [in] What to paint again.
 */
void C2DswypeDialog::invalidateIndex( int index)
{
    try
    {
        m_repaint =true;
        for ( auto obj : m_swypeObjects)
        {
            if ( obj->index ==index)
            {
                m_mainGraph->setRenderArea( obj->getTexture());
                if ( m_horizontal)
                {
                    Crect rect( 0, 0, itemBlocks(), m_rect.height());
                    onPaintUnit( index, rect);
                }
                else
                {
                    Crect rect( 0,0, m_rect.width(), itemBlocks());
                    onPaintUnit( index, rect);
                }
                m_mainGraph->setRenderArea( NULL);
                break;
            }
        }
    }
    catch(...)
    {
        Cgraphics::m_defaults.log("ERROR %s::invalidateIndex", typeid(this).name());
    }
}

/** Paint on normal background.
 *  @param index [in] Which element to paint.
 *  @param location [in] Where to paint this one.
 */
void C2DswypeDialog::onPaintBackground( int index, const Crect &location)
{
#ifdef USE_SDL2
	(void)index;
	(void)location;
	assert(0);
#else
	(void)index;
	Cgraphics *previous =m_graphics;
	Cdialog *parent =m_parent;
	m_graphics =m_mainGraph;
	m_parent =NULL;
	m_object->setGraphics(m_mainGraph);
	m_graphics =m_mainGraph;

	onPaintUnit( m_dragIndex, location);
	m_graphics =previous;
	m_parent =parent;
#endif
}

void C2DswypeDialog::setSpan( int x)
{
	m_span =x;
	resetPaintedArea();
	clean();
	calculateItemRect();
}

/** @brief make sure this is the top/left item to show.
 *  @param index [in] which one to show.
 *  @param time [in] how much time left.
 */
bool C2DswypeDialog::scrollToIndex( int index, int time)
{
	return CswypeDialog::scrollToRow( index/m_span, time);
}

/// @brief  Depending on the scroll offset and painted area, we calculate the drawing area.
void C2DswypeDialog::calculateSurfacePosition()
{
	int size;
	int row =(int)rows();
	if ( m_horizontal)
	{
		size =m_itemRect.width()*8;
		m_firstVisibleUnit =m_span*(int)(m_scrollPosition/size);
		m_lastVisibleUnit =(int)((m_scrollPosition+m_rect.width()*8+size-1)/size);
	}
	else
	{
		size =m_itemRect.height()*8;
		m_firstVisibleUnit =m_span*(int)(m_scrollPosition/size);
		m_lastVisibleUnit =m_span*(int)((m_scrollPosition+m_rect.height()*8+size-1)/size);
	}
	if ( m_lastVisibleUnit >row*m_span)
	{
		m_lastVisibleUnit =row*m_span;
	}
	m_firstOptionalUnit =m_firstVisibleUnit-m_listSize/(4*m_span);
	m_lastOptionalUnit =m_lastVisibleUnit+m_listSize/(4*m_span);
	if ( m_firstOptionalUnit<0)
	{
		m_firstOptionalUnit=0;
	}
	if ( m_lastOptionalUnit>row*m_span)
	{
		m_lastOptionalUnit =row*m_span;
	}
}

/// @brief Paint all things from textures to the real screen.
void C2DswypeDialog::onPaint()
{
	if ( !m_visible)
	{
		return;
	}
	try
	{
        if (m_horizontal)
        {
            onPaintSurfaceHorizontal( false);
        }
        else
        {
            onPaintSurfaceVertical( false);
        }
        SDL_Rect rct, scr;

        int offx =m_rect.left()*8;
        int offy =m_rect.top()*8;
        rct.x =(Sint16)(m_rect.left()*8);
        rct.y =(Sint16)(m_rect.top()*8);
        rct.w =(Uint16)(m_rect.width()*8);
        rct.h =(Uint16)(m_rect.height()*8);
        scr.x =rct.x;
        scr.y =rct.y;
        scr.w =rct.w;
        scr.h =rct.h;
        m_mainGraph->setViewport( &rct);
        m_mainGraph->setColour( m_backgroundColour);
        m_mainGraph->bar( rct.x, rct.y, rct.x+rct.w, rct.h+rct.y);
        m_horizontal ? paintHorizontal(rct, offx, offy):paintVertical(rct, offx, offy);

        // Put back old viewport
        m_mainGraph->setViewport(NULL);
        m_mainGraph->update( scr.x, scr.y, scr.w, scr.h);
	}
    catch(...)
    {
        Cgraphics::m_defaults.log("ERROR %s::onPaint", typeid(this).name());
    }
}

/*----------------------------------------------------------------------------*/
void C2DswypeDialog::paintHorizontal(SDL_Rect &rct, int offx, int offy)
{
    try
    {
        int width =itemBlocks()*8;
        int height =m_rect.height()*8;
        bool first = true;
        // Draw horizontal items
        for ( CswypeObjectPtr obj : m_swypeObjects )
        {
            if (first)
            {
                rct.w =(Uint16)(m_swypeObjects[0]->destination.width()*8);
                first = false;
            }
            rct.x =(Sint16)(offx+width*(int)(obj->index/m_span)-(int)m_scrollPosition);
            int y =obj->index%m_span;
            rct.y =(Sint16)(y*height/m_span);
            rct.h =(Uint16)((y+1)*height/m_span-rct.y);
            rct.y =(Uint16)(rct.y+offy);
            renderCopy( obj->getTexture(), &rct);
        }
    }
    catch(...)
    {
        Cgraphics::m_defaults.log("ERROR C2DswypeDialog::paintHorizontal");
    }
}

/*----------------------------------------------------------------------------*/
void C2DswypeDialog::paintVertical(SDL_Rect &rct, int offx, int offy)
{
    try
    {
        int height =itemBlocks()*8;
        int width =m_rect.width()*8;
        // Draw horizontal items
        bool first = true;

        for ( CswypeObjectPtr obj : m_swypeObjects )
        {
            if (first)
            {
                first = false;
                rct.h =(Uint16)(m_swypeObjects[0]->destination.height()*8);
            }
            rct.y =(Sint16)(offy+height*(int)(obj->index/m_span)-(int)m_scrollPosition);
            int x =obj->index%m_span;
            rct.x =(Sint16)(x*width/m_span);
            rct.w =(Uint16)((x+1)*width/m_span-rct.x);
            rct.x =(Sint16)(rct.x+offx);
            renderCopy( obj->getTexture(), &rct);
        }
    }
    catch(...)
    {
        Cgraphics::m_defaults.log("ERROR C2DswypeDialog::paintVertical");
    }
}
