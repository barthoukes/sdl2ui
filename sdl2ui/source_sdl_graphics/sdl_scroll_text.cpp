/*============================================================================*/
/**  @file       sdl_scroll_text.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Text to display scrolling
 **
 **  Create a text object, with font, background
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CscrollText
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
#include "sdl_scroll_text.h"
#include "sdl_dialog.h"
#include "sdl_surface.h"

/** @brief Constructor
 *  @param parent [in] Parent dialog
 *  @param rect [in] Position text
 *  @param code [in] What key code
 *  @param font [in] What font and size
 *  @param value [in] String to display
 *  @param text [in] What colour to display
 *  @param align [in] Alignment on screen
 *  @param cursor [in] To display any cursor (or -1)
 *  @param background [in] Background colour for cursor
 */
CscrollText::CscrollText( Cdialog *parent,
		      const Crect &rect,
		      keybutton code,
		      const Sfont &font,
		      const std::string &value,
		      colour text,
		      int background)
: CdialogObject( parent, rect, code)
, m_value( value)
, m_colour( text)
, m_font( font)
, m_style( TTF_STYLE_NORMAL)
, m_horizontal_margin(0)
, m_vertical_margin(0)
, m_shadow(0)
, m_rotate(false)
, m_speed(0)
, m_scroll(0)
, m_surface( value, m_font.chinese.font, Csize(10000, rect.height()*8), GRAVITY_LEFT_CENTER)
{
    (void)background;
}

/** @brief Destructor */
CscrollText::~CscrollText()
{
}

/*============================================================================*/
///
/// @brief Paint alternate text.
///
/// @brief text [in] Alternative text.
///
/*============================================================================*/
void CscrollText::onPaint( int touch)
{
	(void)touch;
	onPaint( m_value, touch);
}

/*============================================================================*/
///
/// @brief Paint alternate text.
///
/// @brief text [in] Alternative text.
///
/*============================================================================*/
void CscrollText::onPaint( const std::string &text, int touch)
{
    (void)touch;
	utf8string zs(text); // Conversion old format.
	std::string t(zs);
	TTF_Font *f=m_font.local.font;
	for ( int a=0; a<(int)zs.size(); a++)
	{
		if ( zs[a]>4096)
		{
			f=m_font.chinese.font;
			break;
		}
	}
	if (!f)
	{
		return;
	}
	TTF_SetFontStyle( f, m_style);
	Crect rect( m_rect*8);

	rect =Crect( rect.left()+m_horizontal_margin, rect.top()+m_vertical_margin,
			     rect.width()-2*m_horizontal_margin, rect.height()-2*m_vertical_margin);
//	SDL_Rect view;
//	view.x =m_rect.left()*8+m_horizontal_margin;
//	view.y =m_rect.top()*8+m_vertical_margin;
//	view.w =m_rect.width()*8-2*m_horizontal_margin;
//	view.h =m_rect.height()*8-2*m_vertical_margin;

	int w =m_rect.width()*8-2*m_horizontal_margin;
	int x =m_rect.left()*8+m_horizontal_margin;
	int y =m_rect.top()*8+m_vertical_margin;
	int h =m_rect.height()*8-2*m_vertical_margin;
	Crect src( 0,0, (int)m_scroll,h );
	if ( (int)m_scroll<w)
	{
		Crect dst( x+w+1-(int)m_scroll,y+1, (int)m_scroll,h );
		m_pGraphics->copySurface( m_surface.first(), dst, src);
	}
	Crect dst2( x+w-(int)m_scroll,y, (int)m_scroll,h );
	m_pGraphics->copySurface( m_surface.first(), dst2, src);
}

/*============================================================================*/
///
/// @brief Set Text ID for text to display.
///
/// @param textId [in] Which Text ID to display.
///
/*============================================================================*/
void CscrollText::setColour( colour text)
{
	m_colour =text;
}

/*============================================================================*/
///
/// @brief Set Text ID for text to display.
///
/// @param textId [in] Which Text ID to display.
///
/*============================================================================*/
void CscrollText::setText( const std::string &text)
{
	m_value =text;
}

/*============================================================================*/
///
/// @brief Check whether the text is an empty string.
///
/// return true when the text is empty.
///
/*============================================================================*/
bool CscrollText::empty()
{
	return m_value.empty();
}
