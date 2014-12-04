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
 **  Copyright (c) Bart Houkes, 16 aug 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

/*------------- Standard includes --------------------------------------------*/
#pragma once

#include <string>
#include <vector>
#include "keybutton.h"
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
	void setRect( const Crect &rect) { m_rect=rect; }
	int height() { return m_rect.height(); }
	int width() { return m_rect.width(); }
	int left() { return m_rect.left(); }
	int right() { return m_rect.right(); }
	int top() { return m_rect.top(); }
	int bottom() { return m_rect.bottom(); }
	void 	setVisible( bool visible) { m_visible=visible; }

	keybutton getKey() { return m_code; }
	virtual bool onDragStart( Cpoint p) { (void)p.x; return false; } // Position, not grid!
	virtual bool onDrag( Cpoint p) { (void)p.x; return false; } // Position, not grid!
	virtual bool onDragEnd( Cpoint p) { (void)p.x; return false; } // Position, not grid!
	virtual bool wheelUp() { return false; }
	virtual bool wheelDown() { return false; }

public:
	Crect		m_rect; ///< Rectangle.
	keybutton 	m_code;		///< Code for keys.
	bool		m_dragEnable; ///< Can move the button around.
	bool		m_visible; ///< are we there?
	Cgraphics	*m_graphics; ///< where to paint.
	Cdialog		*m_parent; ///< dialog on top.
};

typedef std::vector<CdialogObject*> dialogObjectList;
typedef dialogObjectList::iterator 	dialogObjectIterator;
typedef dialogObjectList::reverse_iterator 	dialogObjectReverseIterator;

/* DIALOG_OBJECT_H_ */
