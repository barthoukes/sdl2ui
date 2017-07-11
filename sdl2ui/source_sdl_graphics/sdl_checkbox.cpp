/*============================================================================*/
/**  @file       sdl_checkbox.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Default checkbox implementation.
 **
 **  Create a checkbox button.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Ccheckbox
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
#include "sdl_checkbox.h"

/*============================================================================*/
///
/// @brief Constructor to be expired...
//
/// @param x [in] Position.
/// @param y [in] Position.
/// @param width [in] Position.
/// @param height [in] Position.
///
/*============================================================================*/
Ccheckbox::Ccheckbox( Cdialog *parent,
		          const Crect &rect,
		          keybutton code,
		          textId id,
		          EborderType border,
		          EfillType fill,
		          const std::string &icon)
: Cbutton( parent, rect, code, id, border, fill, icon)
, m_checked(false)
{
}

/*============================================================================*/
///
/// @brief Constructor to be expired...
//
/// @param x [in] Position.
/// @param y [in] Position.
/// @param width [in] Position.
/// @param height [in] Position.
///
/*============================================================================*/
Ccheckbox::Ccheckbox( Cdialog *parent,
		          const Crect &rect,
		          keybutton code,
		          const std::string &text,
		          EborderType border,
		          EfillType fill,
		          const std::string &icon)
: Cbutton( parent, rect,code, text, border, fill, icon)
, m_checked(false)
{
}

/*============================================================================*/
///
/// @brief Constructor to be expired...
//
/// @param x [in] Position.
/// @param y [in] Position.
/// @param width [in] Position.
/// @param height [in] Position.
///
/*============================================================================*/
Ccheckbox::Ccheckbox( Cdialog *parent,
		          const Crect &rect,
		          keybutton code,
		          const Sfont &font,
		          textId id,
		          EborderType border,
		          Egravity gravity,
		          int radius,
		          EfillType fill)
: Cbutton( parent, rect, code, font, id, border, gravity, radius, fill)
, m_checked(false)
{
}

Ccheckbox::Ccheckbox( Cdialog *parent,
		          const Crect &rect,
		          keybutton code,
		          const Sfont &font,
		          const std::string &text,
		          EborderType border,
		          Egravity gravity,
		          int radius,
				  EfillType fill)
: Cbutton(parent,rect,code,font,text,border,gravity,radius,fill)
, m_checked(false)
{
}

/*============================================================================*/
///
/// @brief Destructor.
///
/*============================================================================*/
Ccheckbox::~Ccheckbox()
{
	m_checked=false;
}

/*============================================================================*/
///
///  @brief 	Paint the dialog once. Use invalidate() to re-paint soon.
///
///	 @pre		Called when dialog is invalidated.
///
///  @post		Dialog updated. Buttons, drag and background are done automatic.
///
/*============================================================================*/
void Ccheckbox::onPaint( int touch)
{
	if ( !m_visible)
	{
		return;
	}
	Cbutton::onPaint( touch);
	std::string i=m_checked ? "check*.png":"uncheck*.png";
	int w=Cgraphics::m_defaults.button_height;
	Cimage state( m_parent, m_rect.offset( m_rect.width()-w, 0, w,w), KEY_NOCHANGE, i);
	state.onPaint(touch);
}
