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
#include <assert.h>
#include "sdl_dialog_object.h"
#include "sdl_dialog.h"

/** @brief Constructor for dialog object.
 *  @param parent [in] Who is painting me.
 *  @param rect [in] Where am i?
 *  @param code [in] What to react when you press me.
 */
CdialogObject::CdialogObject( Cdialog *parent, const Crect &rect, keybutton code)
: m_rect( rect)
, m_code( code)
, m_dragEnable( false)
, m_painting( false)
, m_visible( true)
, m_graphics( NULL)
, m_parent( parent)
{
	if ( parent !=NULL && parent->m_graphics !=NULL)
	{
		 m_graphics = parent->m_graphics;
	}
	else if ( Cdialog::g_defaultWorld !=NULL )
	{
		m_graphics = Cdialog::g_defaultWorld->graphics();
	}
	else
	{
		assert(0);
	}
}

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
void CdialogObject::onUpdate()
{
	m_graphics->update(m_rect*8);
}

/** @brief Destructor. */
CdialogObject::~CdialogObject()
{
}
