/*============================================================================*/
/**  @file       sdl_combobox.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Default combobox implementation.
 **
 **  Create a combobox button.
 **
 **  @date       21 may 2016
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Ccombobox
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
#include "sdl_combobox.h"

/*============================================================================*/
///
/// @brief Constructor combobox
//
/// @param parent [in] Parent directory
/// @param rect [in] Position and size
/// @param code [in] Code on display
/// @param id [in] What text to use
/// @param border [in] What border
/// @param fill [in] Pattern to fill
/// @param icon [in] What icon to use
///
/*============================================================================*/
Ccombobox::Ccombobox( Cdialog *parent,
		          const Crect &rect,
		          keybutton code,
		          textId id,
	 			  const comboNames &names,
		          EborderType border,
		          EfillType fill,
		          const std::string &icon)
: Cbutton( parent, rect, code, id, border, fill, icon)
, m_index(0)
{
	Sfont font( CtextFont("small_button"));
	m_value =new Ctext( m_pParent, m_rect, KEY_NOCHANGE, font, "",
			                 Cgraphics::m_defaults.button_text,
			                 GRAVITY_RIGHT_CENTER);

	for ( const std::string &n : names)
	{
		m_values.push_back(n);
	}
	setChoice( m_index);
	m_value->setMargin( 16,0);
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
Ccombobox::Ccombobox( Cdialog *parent,
		          const Crect &rect,
		          keybutton code,
		          const std::string &text,
				  const comboNames &names,
		          EborderType border,
		          EfillType fill,
		          const std::string &icon)
: Cbutton( parent, rect,code, text, border, fill, icon)
, m_index(0)
{
	Sfont font( CtextFont("small_button"));

	m_value =new Ctext( m_pParent, m_rect, KEY_NOCHANGE, font, "",
							 Cgraphics::m_defaults.button_text,
			                 GRAVITY_RIGHT_CENTER);

	for ( const std::string &n : names)
	{
		m_values.push_back(n);
	}
	setChoice( m_index);
	m_value->setMargin( 16,0);
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
Ccombobox::Ccombobox( Cdialog *parent,
		          const Crect &rect,
		          keybutton code,
		          const Sfont &font,
		          textId id,
				  const comboNames &names,
		          EborderType border,
		          Egravity gravity,
		          int radius,
		          EfillType fill)
: Cbutton( parent, rect, code, font, id, border, gravity, radius, fill)
, m_index(0)
{
	m_textButton =std::make_shared<Ctext>( m_pParent, m_rect, KEY_NOCHANGE,
			                 font, "",
							 Cgraphics::m_defaults.button_text,
			                 GRAVITY_RIGHT_CENTER);
	for ( const std::string &n : names)
	{
		m_values.push_back(n);
	}
	setChoice( m_index);
	m_value->setMargin( 16,0);
}

Ccombobox::Ccombobox( Cdialog *parent,
		          const Crect &rect,
		          keybutton code,
		          const Sfont &font,
		          const std::string &text,
 				  const comboNames &names,
		          EborderType border,
		          Egravity gravity,
		          int radius,
				  EfillType fill)
: Cbutton(parent,rect,code,font,text,border,gravity,radius,fill)
, m_index(0)
{
	m_textButton =std::make_shared<Ctext>( m_pParent, m_rect, KEY_NOCHANGE,
			                 font, "",
							 Cgraphics::m_defaults.button_text,
			                 GRAVITY_RIGHT_CENTER);
	for ( const std::string &n : names)
	{
		m_values.push_back(n);
	}
	setChoice( m_index);
	m_value->setMargin( 16,0);
}

/*============================================================================*/
///
/// @brief Destructor.
///
/*============================================================================*/
Ccombobox::~Ccombobox()
{
	m_values.clear();
	m_index=0;
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
void Ccombobox::onPaint( int touch)
{
	if ( !m_visible)
	{
		return;
	}
	Cbutton::onPaint( touch);
	if ( m_index>=0 && m_index<(int)m_values.size())
	{
		std::string i=m_values[m_index];
		m_value->onPaintText( i, touch);
	}
}

std::string Ccombobox::getValue()
{
	if ( m_index>=0 && m_index<(int)m_values.size())
	{
		return m_values[m_index];
	}
	return "";
}

void Ccombobox::setValue( const std::string &value)
{
	for ( int n=0; n<(int)m_values.size(); n++)
	{
		if (value==m_values[n])
		{
			m_index=n;
		}
	}
	m_values.push_back(value);
	m_index=(int)m_values.size();
}

int Ccombobox::getChoice()
{
	return m_index;
}

void Ccombobox::setChoice( int x)
{
	m_index =x;
}

void Ccombobox::next()
{
	m_index++;
	if (m_index<0)
	{
		m_index=0;
	}
	if (m_index>=(int)m_values.size())
	{
		m_index=0;
	}
}
