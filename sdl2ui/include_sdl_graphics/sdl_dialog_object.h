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

#include "sdl_rect.h"
#include "sdl_keybutton.h"

class Cgraphics;
class Cdialog;

// Touch speeds, may be used for showing pressed buttons
#define NO_TOUCH		  0
#define TOUCH_MAXIMUM	  100
#define TOUCH_MINIMUM	  -100

/// @brief  Alignment of buttons.
typedef enum
{
	GRAVITY_LEFT,			///< Object to the left.
    GRAVITY_LEFT_TOP,       ///< Object on top left.
	GRAVITY_LEFT_CENTER,	///< Object left center.
	GRAVITY_LEFT_BOTTOM,    ///< Object left bottom.
    GRAVITY_RIGHT_TOP,      ///< Object on top right.
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

/// @brief	Root for all painteonPaintd objects on screen.
class CdialogObject
{
public:
	CdialogObject( Cdialog *pParent, const Crect &rect, keybutton code);
	CdialogObject( const CdialogObject & ) = delete;

	virtual ~CdialogObject();

public:
	/// Paint the object.
	virtual void onPaint( int touch) =0;
	void onPaint( const Cpoint &p, int touch);
	virtual void enableDrag();
	virtual void disableDrag();
	virtual void enablePainting();
	virtual Crect getRect() const;
	virtual void setRect( const Crect &rect);
	virtual void setLeft( int left);
    virtual void setTop( int top);
	virtual void setRight( int right);
    virtual void setHeight( int hh);
    virtual void setWidth( int width);
	virtual void setCenter( const Cpoint &center);
	virtual int height() const;
	virtual int width() const;
	virtual int left() const;
	virtual int right() const;
	virtual int top() const;
	virtual int bottom() const;
	virtual void setVisible( bool visible);
	virtual void unlockKeycodes();
	virtual void onPaintLocked(const Cpoint &point);

	virtual keybutton getKey(const Cpoint &p) const;
	virtual void setKey( keybutton code);
	virtual bool onDragStart( Cpoint p); // Position, not grid!
	virtual bool onDrag( Cpoint p); // Position, not grid!
	virtual bool onDragEnd( Cpoint p); // Position, not grid!
	virtual bool onPaintingStart( const Cpoint &p);
	virtual bool onPaintingMove( const Cpoint &p);
	virtual bool onPaintingStop( const Cpoint &p);
	virtual bool wheelUp();
	virtual bool wheelDown();
	virtual void onUpdate();
	virtual void setGraphics(Cgraphics *pGraphics);
	virtual bool isVisible() const;
	virtual bool hasGraphics() const;
	virtual bool isPaintingDuringDrag() const;
	virtual bool isDragEnabled() const;
	virtual Cdialog *getParent();
    virtual bool inside( const Cpoint &point) const;
    virtual Csize size() const;
    virtual Cpoint origin() const;
	keybutton getObjectKey() const;

public:
    Crect       m_rect; ///< Rectangle.
protected:
	keybutton 	m_code;		///< Code for keys.
	bool		m_dragEnable; ///< Can move the button around.
	bool		m_paintDuringDrag; ///< Is this object for painting?
	bool		m_visible; ///< are we there?
	Cgraphics	*m_pGraphics; ///< where to paint.
	Cdialog		*m_pParent; ///< dialog on top.
};

typedef std::shared_ptr<CdialogObject> CdialogObjectPtr;
typedef std::vector<CdialogObjectPtr> dialogObjectList;
typedef dialogObjectList::iterator 	dialogObjectIterator;
typedef dialogObjectList::reverse_iterator 	dialogObjectReverseIterator;

/* DIALOG_OBJECT_H_ */
