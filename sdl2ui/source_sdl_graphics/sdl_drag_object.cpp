/*============================================================================*/
/**  @file      sdl_drag_object.cpp
 **  @ingroup   sdl2ui
 **  @brief		Keep a drag object
 **
 **  Class to define the drag object(s).
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CdragObject
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
#include <stdio.h>
#include "sdl_drag_object.h"
#include "sdl_rect.h"
#include "sdl_dialog_object.h"
#include "sdl_dialog.h"

/**
 * Constructor drag object
 * @param object [in] Related object
 * @param origin [in] Location to paint for start
 */
CdragObject::CdragObject()
: m_dragObject(NULL)
, m_dragStart(0,0)
, m_dragOffset(0,0)
, m_dragPoint(0,0)
{
}

CdragObject::~CdragObject()
{
}

/** @remove the Drag object from screen */
void CdragObject::clean()
{
	m_dragObject =NULL;
}

/** @brief Drag start finger
 *  @param finger [in] New finger location
 *  @param object [in] What object to move
 *  @return true when done correct
 */
bool CdragObject::start( const Cpoint &finger, CdialogObjectPtr object)
{
	try
	{
		m_dragStart =finger;
		m_dragObject =object;
		if ( m_dragObject)
		{
			if ( !m_dragObject->isDragEnabled())
			{
				clean();
			}
			else
			{
				// Cdialog::onEvent  drag start
				m_dragOffset =m_dragStart-m_dragObject->origin()*8;
			}
		}
	}
	catch(...)
	{
	}
	return true;
}

/** @brief Drag the object to a new location
 *  @param finger [in] New location for the finger
 *  @return new location finger
 */
Cpoint CdragObject::dragTo( const Cpoint &leftTop)
{
	/** Drag an object to another location. */
	Cpoint m=leftTop;
	if ( !isEmpty())
	{
		//m =m-m_dragOffset;
		Cpoint p( Cgraphics::m_defaults.width, Cgraphics::m_defaults.height);
		try
		{
			p-=m_dragObject->size()*8;
		}
		catch (...)
		{
			p =m_dragOffset;
		}
		m.limit( Cpoint( 0,0), p);
		m_dragPoint =m;
	}
	return m;
}

/** @brief Move to location
 *  @param point [in] New location to drag to
 *  @return true on success
 */
bool CdragObject::moveTo( const Cpoint &point)
{
	Cpoint m=point;
	if ( !isEmpty())
	{
		m -=m_dragOffset;
		if ( m_dragPoint !=m)
		{
			//m_dragPoint =point;
			Cdialog::invalidate();
			dragTo( m);
			return true;
		}
	}
	return false;
}

/** @brief Release the finger from dragging the object
 *  @param finger [in] Location to stop
 *  @return true if the dialog object has some code to tell true.
 */
bool CdragObject::stop( const Cpoint &finger)
{
	if ( m_dragObject.get())
	{
		return m_dragObject->onDragEnd( finger);
	}
	return false;
}

Cpoint CdragObject::getTopLeft( const Cpoint mouse)
{
	Cpoint p =mouse-m_dragOffset;
	return p;
}

/** Paint the drag object */
void CdragObject::onPaint()
{
	if ( m_dragObject)
	{
		m_dragObject->onPaintLocked( m_dragPoint);
	}
}

CdragObject & CdragObject::operator =(CdialogObjectPtr object)
{
	m_dragObject =object;
	return *this;
}

/** @brief Start dragging from a certain point
 *  @param object [in] What to drag
 *  @param offset [in] Where in the small image we start
 *  @param start  [in] What location on screen we start
 */
void CdragObject::setObject( CdialogObjectPtr object, Cpoint offset, Cpoint start)
{
	m_dragObject =object;
	m_dragOffset =offset;
	m_dragPoint  =start;
}

#if 0
/**
 * Constructor drag object
 * @param object [in] Related object
 * @param origin [in] Location to paint for start
 */
CdragObject::CdragObject()
: m_relatedObject( NULL)
, m_visible( false)
, m_origin( 0,0)
, m_texture( NULL)
, m_renderer( m_mainGraph->getRenderer())
, m_dragOffset( 0,0)
, m_dragStart( 0,0)
{
	m_position.x =m_origin.left();
	m_position.y =m_origin.top();
	m_position.w =m_relatedObject->width();
	m_position.h =m_relatedObject->height();
}

/**
 * Clear dragging object memory
 */
void CdragObject::clear()
{
	m_relatedObject =NULL;
	if ( m_texture)
	{
		SDL_DestroyTexture( m_texture);
		m_texture =NULL;
	}
}

/**
 * Set new object to drag around
 * @param object
 * @param origin
 */
void CdragObject::dragStart( CdialogObjectPtr object, const Cpoint &origin)
{
	int width =object->m_rect.width()*8;
	int height =object->m_rect.height()*8;

	m_relatedObject =object;
	if ( m_relatedObject && (width !=m_position.w || width !=m_position.h))
	{
		clear();
	}
	m_relatedObject =object;
	m_position.w =width;
	m_position.h =height;
	m_position.x =origin.left()-(width/2);
	m_position.y =origin.top()-(height/2);

	m_texture =SDL_CreateTexture( m_renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_TARGET, m_position.w, m_position.h);
	if ( !m_texture)
	{
		return;
	}
	if ( m_relatedObject !=NULL)
	{
		Cpoint p( m_position.x, m_position.y);
		SDL_SetRenderTarget( m_renderer, NULL);
		m_relatedObject->onPaint( p);
		m_mainGraph->setRenderArea( NULL);
		m_relatedObject->onDragStart( origin);
	}
}

/** @brief Destructor */
CdragObject::~CdragObject()
{
	clear();
}

/**
 * Move the dragging object to a new positio
 * @param position
 */
void CdragObject::dragMove( const Cpoint &position)
{
	m_position.x =position.left()-(m_position.w/2);
	m_position.y =position.top()-(m_position.h/2);
	m_relatedObject->onDrag( position);
}

/**
 * End of dragging the object
 * @param position [in] Where to stop
 */
void CdragObject::dragEnd( const Cpoint &position)
{
	m_relatedObject->onDragEnd( position);
}

/** @brief  Change how we show the object
 *  @param  visible [in]  true if we want to see the object
 */
void CdragObject::setVisible( bool visible)
{
	m_visible =visible;
}

/** @brief  Paint the object completely, even make a texture if needed.
 */
void CdragObject::onPaint()
{
	if ( m_visible && m_texture)
	{
		SDL_RenderCopy( m_renderer, m_texture, NULL, &m_position);
	}
}

/** Drag an object to another location. */
Cpoint CdragObject::dragPoint( const Csize &graph, const Cpoint &mouse)
{
	Cpoint m=mouse;
	m =m-m_dragOffset;
	int x, y;
	try
	{
		x=graph.width()-(m_relatedObject->m_rect.width()*8);
		y=graph.height()-(m_relatedObject->m_rect.height()*8);
	}
	catch (...)
	{
		x =m_dragOffset.x;
		y =m_dragOffset.y;
	}
	m.limit( Cpoint( 0,0), Cpoint( x,y));
	return m;
}

#endif
