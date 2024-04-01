/*============================================================================*/
/**  @file       sdl_dialog_object.cpp
 **  @ingroup    sdl2ui
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

/*------------- Standard includes --------------------------------------------*/
#include "sdl_dialog_object.h"
#include "sdl_dialog.h"

/*----------------------------------------------------------------------------*/
/** @brief Constructor for dialog object.
 *  @param parent [in] Who is painting me.
 *  @param rect [in] Where am i?
 *  @param code [in] What to react when you press me.
 */
CdialogObject::CdialogObject( Cdialog *pParent, const Crect &rect, keybutton code)
: m_rect( rect)
, m_code( code)
, m_dragEnable( false)
, m_paintDuringDrag( false)
, m_visible( true)
, m_pGraphics( m_mainGraph)
, m_pParent( pParent)
{
	if ( pParent !=NULL && pParent->m_graphics !=NULL)
	{
		 m_pGraphics =pParent->m_graphics;
	}
}


/*----------------------------------------------------------------------------*/
/** @brief Paint me.
 *  @param p [in] Different location.
 *  @post I'm painted.
 */
void CdialogObject::onPaint( const Cpoint &p, int touch)
{
	Crect r =m_rect;
	m_rect.setLeft( gMin( p.x, (Cgraphics::m_defaults.width/8)-m_rect.width()));
	m_rect.setTop( gMin( p.y, (Cgraphics::m_defaults.height/8)-m_rect.height()));
	onPaint( touch);
	m_rect =r;
}

// Copy part to the final graphics output
/*----------------------------------------------------------------------------*/
void CdialogObject::onUpdate()
{
	if ( m_pGraphics)
	{
		m_pGraphics->update( m_rect.left()*8, m_rect.top()*8, m_rect.width()*8, m_rect.height()*8);
	}
	else
	{
		m_mainGraph->update( m_rect.left()*8, m_rect.top()*8, m_rect.width()*8, m_rect.height()*8);
	}
}

/*----------------------------------------------------------------------------*/
void CdialogObject::setGraphics(Cgraphics *pGraphics)
{
	m_pGraphics = pGraphics;
}

/*----------------------------------------------------------------------------*/
bool CdialogObject::isVisible() const
{
	return m_visible;
}

/*----------------------------------------------------------------------------*/
void CdialogObject::setVisible(bool visible)
{
    m_visible = visible;
}

/*----------------------------------------------------------------------------*/
void CdialogObject::unlockKeycodes()
{
	if (m_pGraphics)
	{
		m_pGraphics->unlock_keycodes();
	}
}

void CdialogObject::onPaintLocked(const Cpoint &point)
{
	m_pGraphics->lock_keycodes();
	onPaint( point.div8(),0);
	m_pGraphics->unlock_keycodes();
}

/*----------------------------------------------------------------------------*/
bool CdialogObject::hasGraphics() const
{
	return (m_pGraphics != NULL);
}

/*----------------------------------------------------------------------------*/
void CdialogObject::setCenter( const Cpoint &center)
{
	 m_rect.setLeft( center.left()-m_rect.width()/2);
	 m_rect.setTop( center.top()-m_rect.height()/2);
}

/*----------------------------------------------------------------------------*/
void CdialogObject::setHeight( int hh)
{
    m_rect.setHeight(hh);
}

/*----------------------------------------------------------------------------*/
void CdialogObject::setLeft( int left)
{
    m_rect.setLeft(left);
}

/*----------------------------------------------------------------------------*/
void CdialogObject::enablePainting()
{
    m_paintDuringDrag=true;
}

/*----------------------------------------------------------------------------*/
void CdialogObject::setRight( int right)
{
    m_rect.setRight(right);
}

/*----------------------------------------------------------------------------*/
void CdialogObject::setTop( int top)
{
    m_rect.setTop(top);
}

/*----------------------------------------------------------------------------*/
int CdialogObject::height() const
{
    return m_rect.height();
}

/*----------------------------------------------------------------------------*/
int CdialogObject::left() const
{
    return m_rect.left();
}

/*----------------------------------------------------------------------------*/
int CdialogObject::width() const
{
    return m_rect.width();
}

/*----------------------------------------------------------------------------*/
void CdialogObject::setWidth( int width)
{
    m_rect.setWidth(width);
}

bool CdialogObject::inside( const Cpoint &point) const
{
    return m_rect.inside(point);
}

/*----------------------------------------------------------------------------*/
/** @brief Destructor. */
CdialogObject::~CdialogObject()
{
	/* I am not owner of the graphics, so not delete */
}

/*----------------------------------------------------------------------------*/
keybutton CdialogObject::getKey( const Cpoint &p) const
{
	std::ignore = p;
    return m_code;
}

/*----------------------------------------------------------------------------*/
void CdialogObject::setKey( keybutton code)
{
    m_code=code;
}

/*----------------------------------------------------------------------------*/
bool CdialogObject::onDragStart( Cpoint p)
{
    (void)p.x;
    return false;
}

/*----------------------------------------------------------------------------*/
bool CdialogObject::onDrag( Cpoint p)
{
    (void)p.x;
    return false;
}

/*----------------------------------------------------------------------------*/
bool CdialogObject::onDragEnd( Cpoint p)
{
    (void)p.x;
    return false;
}

/*----------------------------------------------------------------------------*/
bool CdialogObject::onPaintingStart( const Cpoint &p)
{
    (void)p.x;
    return false;
}

/*----------------------------------------------------------------------------*/
bool CdialogObject::onPaintingMove( const Cpoint &p)
{
    (void)p.x;
    return false;
}

/*----------------------------------------------------------------------------*/
bool CdialogObject::onPaintingStop( const Cpoint &p)
{
    (void)p.x;
    return false;
}

/*----------------------------------------------------------------------------*/
bool CdialogObject::wheelUp()
{
    return false;
}

/*----------------------------------------------------------------------------*/
bool CdialogObject::wheelDown()
{
    return false;
}

/*----------------------------------------------------------------------------*/
bool CdialogObject::isPaintingDuringDrag() const
{
    return m_paintDuringDrag;
}

/*----------------------------------------------------------------------------*/
bool CdialogObject::isDragEnabled() const
{
    return m_dragEnable;
}

/*----------------------------------------------------------------------------*/
Cdialog *CdialogObject::getParent()
{
    return m_pParent;
}

/*----------------------------------------------------------------------------*/
int CdialogObject::right() const
{
    return m_rect.right();
}

/*----------------------------------------------------------------------------*/
int CdialogObject::top() const
{
    return m_rect.top();
}

/*----------------------------------------------------------------------------*/
int CdialogObject::bottom() const
{
    return m_rect.bottom();
}

/*----------------------------------------------------------------------------*/
Csize CdialogObject::size() const
{
    return m_rect.size();
}

/*----------------------------------------------------------------------------*/
Cpoint CdialogObject::origin() const
{
    return m_rect.origin();
}

/*----------------------------------------------------------------------------*/
keybutton CdialogObject::getObjectKey() const
{
	return m_code;
}

/*----------------------------------------------------------------------------*/
void CdialogObject::enableDrag()
{
    m_dragEnable=true;
}

/*----------------------------------------------------------------------------*/
void CdialogObject::disableDrag()
{
    m_dragEnable=false;
}

/*----------------------------------------------------------------------------*/
void CdialogObject::setRect(const Crect &rect)
{
    m_rect =rect;
}

/*----------------------------------------------------------------------------*/
Crect CdialogObject::getRect() const
{
	return m_rect;
}
