/*============================================================================*/
/**  @file       sdl_text.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Text to display
 **
 **  Create a text object, with font, background, multi-language etc.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Ctext
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
#include "lingual.h"
#include "sdl_text.h"
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
Ctext::Ctext( Cdialog *parent,
		      const Crect &rect,
		      keybutton code,
		      const Sfont &font,
		      const std::string &value,
		      colour text,
			  Egravity align,
		      int cursor,
		      int background)
: CdialogObject( parent, rect, code)
, m_value( value)
, m_textId( INVALID_TEXT_ID)
, m_useTextId( false)
, m_gravity( align)
, m_colour( text)
, m_font( font)
, m_style( TTF_STYLE_NORMAL)
, m_horizontal_margin(0)
, m_vertical_margin(0)
, m_shadow(0)
, m_cursor(cursor)
, m_cursorColour(background)
, m_rotate(false)
, m_doesNotFit(false)
, m_isChineseText(false)
{
	m_canDecreaseFont = Cgraphics::m_defaults.resize_font;
}

/** @brief Constructor
 *  @param parent [in] Parent dialog
 *  @param rect [in] Position text
 *  @param code [in] What key code
 *  @param font [in] What font and size
 *  @param value [in] String to display
 *  @param text [in] What colour to display
 *  @param align [in] Alignment on screen
 */
Ctext::Ctext( Cdialog *parent,
		      const Crect &rect,
		      keybutton code,
		      const Sfont &font,
		      const textId value,
		      colour text,
		      Egravity align)
: CdialogObject( parent, rect, code)
, m_value( "")
, m_textId( value)
, m_useTextId( true)
, m_gravity( align)
, m_colour( text)
, m_font( font)
, m_style( TTF_STYLE_NORMAL)
, m_horizontal_margin(0)
, m_vertical_margin(0)
, m_shadow(0)
, m_cursor(-1)
, m_cursorColour(0)
, m_rotate( false)
, m_doesNotFit(false)
, m_isChineseText(false)
{
	m_canDecreaseFont = Cgraphics::m_defaults.resize_font;
}

/** @brief Destructor */
Ctext::~Ctext()
{
}

/*============================================================================*/
///
/// @brief Paint alternate text.
///
/// @brief text [in] Alternative text.
///
/*============================================================================*/
void Ctext::onPaint( int touch)
{
	std::string text;
	if ( m_useTextId)
	{
		text = getTranslation( m_textId, Cgraphics::m_defaults.country);
		onPaintText( text, touch);
	}
	else
	{
		onPaintText( m_value, touch);
	}
}

/*============================================================================*/
///
/// @brief Paint alternate text.
///
/// @brief text [in] Alternative text.
///
/*============================================================================*/
void Ctext::onPaintText( const std::string &text, int touch)
{
	(void)touch;
	utf8string zs(text); // Conversion old format.
	std::string t(zs);
    m_doesNotFit = false;
    m_isChineseText = false;

	// Check for chinese characters in the text.
	for ( int a=0; a<(int)zs.size(); a++)
	{
		if ( zs[a]>255) // && zs[a]!=0x20ac)
		{
			m_isChineseText = true;
			break;
		}
	}

	while(1)
	{
		TTF_Font *font = m_isChineseText ? m_font.chinese.font:m_font.local.font;
		if (!font)
		{
			return; // No font found.
		}

		TTF_SetFontStyle( font, m_style);
		Crect rect( m_rect*8);
		rect = Crect( rect.left()+m_horizontal_margin, rect.top()+m_vertical_margin,
					  rect.width()-2*m_horizontal_margin, rect.height()-2*m_vertical_margin);
		if ( m_shadow)
		{
			Crect rectp=rect;
			rectp.addLeft(1); rectp.addTop(1);
			CtextSurface surface1( m_pGraphics, t, rectp, m_cursor, m_gravity,
								   m_shadow, m_cursorColour, font, m_canDecreaseFont);
			m_doesNotFit = surface1.doesNotFit();
			if (surface1.doesNotFit())
			{
				return;
			}
		}
		CtextSurface surface2( m_pGraphics, t, rect, m_cursor, m_gravity, m_colour,
							   m_cursorColour, font, m_canDecreaseFont);
		m_doesNotFit = surface2.doesNotFit();

		if (!m_doesNotFit)
		{
			break;
		}
		decreaseFont();
	}
}

void Ctext::decreaseFont()
{
	if (m_isChineseText)
	{
		if ( m_font.chinese.fontSize >12)
		{
			m_font.chinese = CtextFont::findFont(m_font.chinese.fontName,
					                             m_font.chinese.fontSize-2);
		}
		else
		{
			m_canDecreaseFont = RESIZE_OFF;
		}
	}
	else
	{
		if ( m_font.local.fontSize >8)
		{
			m_font.local = CtextFont::findFont(m_font.local.fontName,
					                           m_font.local.fontSize-1);
		}
		else
		{
			m_canDecreaseFont = RESIZE_OFF;
		}
	}
}

/*============================================================================*/
///
/// @brief Set Text ID for text to display.
///
/// @param textId [in] Which Text ID to display.
///
/*============================================================================*/
void Ctext::setTextId( textId textId)
{
	m_textId =textId;
	m_value ="";
	m_useTextId =true;
}

/*============================================================================*/
///
/// @brief Set Text ID for text to display.
///
/// @param textId [in] Which Text ID to display.
///
/*============================================================================*/
void Ctext::setColour( colour text)
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
void Ctext::setText( const std::string &textId)
{
	m_textId =INVALID_TEXT_ID;
	m_value =textId;
	m_useTextId =false;
}

/*============================================================================*/
///
/// @brief Check whether the text is an empty string.
///
/// return true when the text is empty.
///
/*============================================================================*/
bool Ctext::empty()
{
	if ( m_useTextId) { return (m_textId ==INVALID_TEXT_ID); }
	return m_value.empty();
}

/*----------------------------------------------------------------------------*/
void Ctext::move( int x, int y)
{
    m_rect.move(x,y);
}

/*----------------------------------------------------------------------------*/
void Ctext::decrease( int w, int h)
{
    m_rect.decrease(w,h);
}
