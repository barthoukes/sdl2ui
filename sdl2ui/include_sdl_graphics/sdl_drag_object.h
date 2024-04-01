/*============================================================================*/
/**  @file       sdl_dialog_event.h
 **  @ingroup    zhongcan_user_interface
 **  @brief		 Default dialog.
 **
 **  Create an event for the dialogs.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cevent
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

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <memory>

class CdialogObject;
typedef std::shared_ptr<CdialogObject> CdialogObjectPtr;

#include "sdl_rect.h"

/// @brief Object to use for dragging around the screen
class CdragObject
{
public:
	CdragObject();
	virtual ~CdragObject();
	void 	clean();
	CdragObject & operator =(CdialogObjectPtr object);
	bool isEmpty() { return !m_dragObject; }
	void setObject( CdialogObjectPtr object, Cpoint offset, Cpoint start);
	void onPaint();
	Cpoint dragTo( const Cpoint &finger);
	bool moveTo( const Cpoint &finger);
	Cpoint getDragPoint() { return m_dragPoint; }
	bool start( const Cpoint &finger, CdialogObjectPtr object);
	bool stop( const Cpoint &finger);
	Cpoint getTopLeft( const Cpoint mouse);

public:
	CdialogObjectPtr	m_dragObject;		///< object to drag.
private:
	Cpoint 				m_dragStart;		///< Where to start dragging
	Cpoint				m_dragOffset;		///< Drag offset in 800x600 res.
	Cpoint              m_dragPoint;		///< Where to drag now
};

typedef std::shared_ptr<CdragObject> CdragObjectPtr;

#if 0
#ifdef USE_SDL2

#include "sdl_rect.h"
#include "SDL_rect.h"
#include "SDL_render.h"

class CdialogObject;

/// @brief Object to use for dragging around the screen
class CdragObject
{
public:
	//CdragObject( SDL_Texture *reference, const Cpoint &origin);
	CdragObject();
	virtual ~CdragObject();

	void dragStart( CdialogObjectPtr object, const Cpoint &origin);
	void clear();
	void dragMove( const Cpoint &position);
	void dragEnd( const Cpoint &position);
	void setVisible( bool visible);
	void onPaint();
	bool isVisible() { return m_visible; }
	Cpoint dragPoint( const Csize &graph, const Cpoint &mouse);

private:
	CdialogObjectPtr m_relatedObject;	///< Relation to an object
	//SDL_Texture		*m_relatedTexture;  ///< Texture relation
	bool			m_visible;			///< Is the item visible
	Cpoint			m_origin; 			///< Position to start from
	SDL_Rect		m_position;			///< Position current
	SDL_Texture		*m_texture;			///< Object itself to paint
	SDL_Renderer    *m_renderer;		///< Where to render
	Cpoint			m_dragOffset;		///< Point within area we touched
	Cpoint			m_dragStart;		///< start of drag object.
};
#endif

#endif
/* END SDL_DRAG_OBJECT_H_ */
