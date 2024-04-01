/*============================================================================*/
/**  @file      sdl_label.cpp
 **  @ingroup   sdl2ui
 **  @brief		Draw scrollbar.
 **
 **  Create and show scrollbar.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Clabel
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
#include "sdl_label.h"
#include "sdl_text.h"
#include "sdl_background.h"

/** @brief Constructor for label, which is a hint on the object to tell which keyboard button can also be used
 *  @param parent [in] Dialog which owns me
 *  @param rect [in] Rectangle of parent object
 *  @param key [in] What key is used to press
 */
Clabel::Clabel( Cdialog *parent, const Crect &rect, keybutton key)
: CdialogObject( parent, rect, key)
, m_label( (key==KEY_NOCHANGE) ? "":Ckeybutton(key).label())
, m_font( CtextFont("label"))
{
	calculateSize( rect);
}

/** @brief Calculate the rectangle size of your label
 *  @param rect [out] Calculated rectangle for label
 */
void Clabel::calculateSize( const Crect &rect)
{
	int len =m_label.length();
	m_enable =!( len==0);
	int w,h;
	//if ( key ==KEY_NOCHANGE)
	//{
	//	m_visible =false;
	//}
	if ( !m_font.local.font)
	{
		return;
	}
	std::string txt =Ckeybutton( m_code);
	TTF_SizeUTF8( m_font.local.font, txt.c_str(), &w, &h);
	w=(w+7)/8;
	h=(h+4)/8;

	m_rect.setHeight(h);
    m_rect.setWidth( w);
	m_rect.setLeft( rect.right()-w);
    m_rect.setTop( rect.bottom()-h);
}

/** @brief Paint the label.
 *  @param touch [in] How much we touch the button
 */
void Clabel::onPaint( int touch)
{
	(void)touch;
	if ( !Cgraphics::m_defaults.show_labels || m_enable==false || m_visible==false)
	{
		return;
	}
	Cbackground( m_pParent, m_rect, m_code, Cgraphics::m_defaults.label_background,
			     4, FILL_GRADIENT, Cgraphics::m_defaults.label_background+0x202020).onPaint(0);
	Ctext txt( m_pParent, m_rect, m_code, m_font,
			   m_label, Cgraphics::m_defaults.label_text, GRAVITY_CENTER);
	txt.onPaint( 0);
}

/** @brief Destructor */
Clabel::~Clabel()
{
}

/** @brief Get the length for the label
 *  @return Length in characters
 */
int Clabel::length()
{
	return m_label.length();
}

/** @brief Change the text for a label
 *  @param txt [in] New text for the label
 */
void Clabel::setText( const std::string &txt)
{
	m_label=txt;
	m_enable =(m_label.length()) ? true:false;
	int w=0,h=0;
	if ( !m_font.local.font)
	{
		return;
	}
	const char *t =txt.c_str();
	if (t && *t)
	{
		TTF_SizeUTF8( m_font.local.font, t, &w, &h);
		w=(w+7)/8;
		h=(h+4)/8;
	}
	m_rect.setTop( m_rect.bottom()-h);
	m_rect.setHeight(h);
	m_rect.setLeft( m_rect.right()-w);
	m_rect.setWidth( w);
}

void Clabel::setRect( const Crect &rect)
{
    m_rect.setLeft( rect.right()-m_rect.width() );
    m_rect.setTop( rect.bottom()-m_rect.height() );
}
