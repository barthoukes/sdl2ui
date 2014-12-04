/*
 * sdl_drag_object.cpp
 *
 *  Created on: 21 jul. 2014
 *      Author: mensfort
 */

#ifdef USE_SDL2

#include <stdio.h>
#include "sdl_drag_object.h"
#include "sdl_rect.h"
#include "sdl_dialog_object.h"

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
void CdragObject::dragStart( CdialogObject *object, const Cpoint &origin)
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

/*
void CdragObject::onUpdateBackground()
{
	if ( m_background ==NULL)
	{
		m_background =SDL_CreateTexture( m_mainGraph->getRenderer(),
				SDL_PIXELFORMAT_ARGB8888,
				SDL_TEXTUREACCESS_TARGET, m_position.w, m_position.h);
		if ( !m_background)
		{
			return;
		}
	}
	SDL_RenderCopy( m_renderer, m_background, &m_position, NULL);
}
*/

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
