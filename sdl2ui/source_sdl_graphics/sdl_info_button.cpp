/*============================================================================*/
/**  @file       sdl_info_button.cpp
 **  @ingroup    zhongcan_sdl
 **  @brief		 The info button has a button and an explanation text.
 **
 **  Create an information button. This is a button with text in front
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CinfoButton
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
#include "sdl_info_button.h"

/*============================================================================*/
///
/// @brief Constructor special types.
//
/// @param rect [in] Position.
/// @param type [in] What type of button.
/// @param key  [in] What key.
/// @param text [in] What text.
/// @param icon [in] Which icon.
///
/*============================================================================*/
CinfoButton::CinfoButton( Cdialog *parent,
			     const Crect &rect,
			     textId id,
			     keybutton code,
			     const std::string &defaultText,
			     int widthTitle)
: CdialogObject( parent, rect, KEY_NOCHANGE),
  m_title( parent,
		   Crect( rect.left(), rect.top(),
		          (widthTitle<=0) ? (Cgraphics::m_defaults.width/8/10):widthTitle,
		          rect.height()),
		   code,
		   (Sfont)CtextFont("info_button"),
		   id,
		   BORDER_NONE,
		   GRAVITY_RIGHT_CENTER,
		   0,
		   FILL_GRADIENT)
{
	m_title.setLabel("");

	Crect rct( m_rect.left()+m_title.width(), m_rect.top(),
			   m_rect.width()-m_title.width(), m_rect.height());
	m_data.push_back( std::make_shared<CtextButton>( parent, rct, code, defaultText));
}


/*============================================================================*/
///
/// @brief Constructor special types.
//
/// @param rect [in] Position.
/// @param type [in] What type of button.
/// @param key  [in] What key.
/// @param text [in] What text.
/// @param icon [in] Which icon.
///
/*============================================================================*/
CverticalInfoButton::CverticalInfoButton( Cdialog *parent,
			     const Crect &rect,
			     textId id,
			     keybutton code,
			     const std::string &defaultText)
: CinfoButton( parent, rect, id, code, defaultText)
{
	int halfHeight =rect.height()/2;
	m_title.setRect( Crect( rect.left(), rect.top(), rect.width(), halfHeight));
	m_data[0]->setRect( Crect( rect.left(), rect.top()+halfHeight, rect.width(), halfHeight));
}

/*----------------------------------------------------------------------------*/
void CinfoButton::push_back(int width, const std::string &text, keybutton key)
{
	int size = (int)m_data.size();
	m_data[0]->decrease(width,0);

	for (int n = 1; n < size; ++n)
	{
		m_data[n]->addLeft(-width);
	}
	int left = m_data[m_data.size()-1]->m_rect.right();

	CtextButtonPtr btn = std::make_shared<CtextButton>
	     ( m_pParent, Crect(left, m_rect.top(), width, m_rect.height()), key, text);
	m_data.push_back(btn);
}

void CinfoButton::setFillType( int line, EfillType fill)
{
	m_data[line]->setFillType(fill);
}

/*----------------------------------------------------------------------------*/
CinfoButton::~CinfoButton()
{
}

/*----------------------------------------------------------------------------*/
void CinfoButton::onPaint( int touch)
{
	if (!m_visible)
	{
		return;
	}
	m_title.onPaint( 0);
	for ( auto button : m_data)
	{
		button->onPaint( touch);
	}
}

/*----------------------------------------------------------------------------*/
void CinfoButton::setText( int line, const std::string &text)
{
	m_data[line]->setText( text);
}

/*----------------------------------------------------------------------------*/
void CinfoButton::setText( textId text)
{
	m_data[0]->setTextId( text);
}

/*----------------------------------------------------------------------------*/
void CinfoButton::setNumber( int line, int text)
{
	char s[20];
	sprintf(s, "%d", text);
	m_data[line]->setText( s);
}

/*----------------------------------------------------------------------------*/
std::string CinfoButton::getText(int line)
{
	return m_data[line]->getText();
}

/*----------------------------------------------------------------------------*/
void CinfoButton::setVisible( bool visible)
{
	m_title.setVisible( visible);
    for (auto button : m_data)
    {
    	button->setVisible( visible);
    }
}

/*----------------------------------------------------------------------------*/
void CinfoButton::setTop( int top)
{
	m_title.setTop(top);
	for (auto button: m_data)
	{
		button->setTop(top);
	}
}

/*----------------------------------------------------------------------------*/
void CinfoButton::setTitleColours( colour infoText,
			            colour background1, colour background2)
{
	(void)infoText;
	(void)background1;
	(void)background2;
}

/*----------------------------------------------------------------------------*/
void CinfoButton::setDataColours( colour infoText,
			            colour background1, colour background2)
{
	m_data[0]->setColours( infoText, background1, background2);
}

/*----------------------------------------------------------------------------*/
void CinfoButton::setTitleWidth( int width)
{
	int total_width =m_data[0]->width()+m_title.width();
	m_title.setWidth( width);
	m_data[0]->setWidth( total_width-width);
	m_data[0]->setLeft( m_title.left()+width);
}

/*----------------------------------------------------------------------------*/
keybutton CinfoButton::getKey( const Cpoint &p) const
{
	for (auto obj : m_data)
	{
		if (obj->m_rect.inside(p))
		{
			return obj->getObjectKey();
		}
	}
	return m_code;
}
