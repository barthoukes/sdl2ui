/*============================================================================*/
/**  @file       sdl_dialog_object.h
 **  @ingroup    zhongcan_sdl
 **  @brief		 Object for dialogs.
 **
 **  We have several types of dialog objects.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CdialogObject
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
#include <string>
#include <vector>
#include <memory>
#ifdef USE_SDL2
#else
#include "sdl_keybutton.h"
#endif
#include "sdl_rect.h"
#include "sdl_graphics.h"

// Touch speeds, may be used for showing pressed buttons
#define NO_TOUCH		  0
#define TOUCH_MAXIMUM	  100
#define TOUCH_MINIMUM	  -100

/// @brief  Alignment of buttons.
typedef enum
{
	GRAVITY_LEFT,			///< Object to the left.
	GRAVITY_LEFT_CENTER,	///< Object left center.
	GRAVITY_LEFT_BOTTOM,    ///< Object left bottom.
	GRAVITY_RIGHT_CENTER,	///< Object right center.
	GRAVITY_RIGHT_BOTTOM,	///< Object right bottom.
	GRAVITY_CENTER,			///< Object in middle.
	GRAVITY_RIGHT,			///< Object in right center.
	GRAVITY_TOP_CENTER,		///< Object on top center.
	GRAVITY_BOTTOM_CENTER,	///< Object on bottom center.
	GRAVITY_RESIZE,			///< Object resize to fit.
	GRAVITY_BEHIND_TEXT,	///< Object behind text
	GRAVITY_NO_CHANGE		///< Keep object on it's place.
} Egravity;

class Cdialog;

/// @brief	Root for all painted objects on screen.
class CdialogObject
{
public:
	CdialogObject( Cdialog *parent, const Crect &rect, keybutton code);
	virtual ~CdialogObject();

public:
	/// Paint the object.
	virtual void onPaint( int touch) =0;
	virtual void onPaint( const Cpoint &p, int touch);
	void enableDrag() { m_dragEnable=true; }
	void enablePainting() { m_painting=true; }
	void setRect( const Crect &rect) { m_rect=rect; }
	void setLeft( int left) { m_rect.setLeft(left); }
	int height() { return m_rect.height(); }
	int width() { return m_rect.width(); }
	int left() { return m_rect.left(); }
	int right() { return m_rect.right(); }
	int top() { return m_rect.top(); }
	int bottom() { return m_rect.bottom(); }
	void 	setVisible( bool visible) { m_visible=visible; }
	bool	isVisible() { return m_visible; }

	keybutton getKey() { return m_code; }
	void setKey( keybutton code) { m_code=code; }
	virtual bool onDragStart( Cpoint p) { (void)p.x; return false; } // Position, not grid!
	virtual bool onDrag( Cpoint p) { (void)p.x; return false; } // Position, not grid!
	virtual bool onDragEnd( Cpoint p) { (void)p.x; return false; } // Position, not grid!
	virtual bool onPaintingStart( const Cpoint &p) { (void)p.x; return false; }
	virtual bool onPaintingMove( const Cpoint &p) { (void)p.x; return false; }
	virtual bool onPaintingStop( const Cpoint &p) { (void)p.x; return false; }
	virtual bool wheelUp() { return false; }
	virtual bool wheelDown() { return false; }
	virtual void onUpdate();
public:
	Crect		m_rect; ///< Rectangle.
	keybutton 	m_code;		///< Code for keys.
	bool		m_dragEnable; ///< Can move the button around.
	bool		m_painting; ///< Is this object for painting?
	bool		m_visible; ///< are we there?
	std::shared_ptr<Cgraphics> m_graphics; ///< where to paint.
	Cdialog		*m_parent; ///< dialog on top.
};

typedef std::vector<CdialogObject*> dialogObjectList;
typedef dialogObjectList::iterator 	dialogObjectIterator;
typedef dialogObjectList::reverse_iterator 	dialogObjectReverseIterator;

/* DIALOG_OBJECT_H_ */
