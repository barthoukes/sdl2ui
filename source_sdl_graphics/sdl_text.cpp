/*
 * sdl_text.cpp
 *
 *  Created on: 23 dec. 2011
 *      Author: mensfort
 */

#include "sdl_text.h"
#include "sdl_dialog.h"
#include "sdl_surface.h"
#include "graphics_widgets.h"

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
, m_textId( _NO_TEXT)
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
{
}

Ctext::Ctext( Cdialog *parent,
		      const Crect &rect,
		      keybutton code,
		      const Sfont &font,
		      const EtextId value,
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
{
}

Ctext::~Ctext()
{
	// TODO Auto-generated destructor stub
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
		text =Lin[ m_textId][ Cdialog::language()];
		onPaint( text, touch);
	}
	else
	{
		onPaint( m_value, touch);
	}
}

/*============================================================================*/
///
/// @brief Paint alternate text.
///
/// @brief text [in] Alternative text.
///
/*============================================================================*/
void Ctext::onPaint( const std::string &text, int touch)
{
	utf8string zs(text); // Conversion old format.
	std::string t(zs);
	TTF_Font *f=m_font.local.font;
	for ( int a=0; a<(int)zs.size(); a++)
	{
		if ( zs[a]>255)
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
	if ( m_shadow)
	{
		Crect rectp=rect;
		rectp.addLeft(1); rectp.addTop(1);
		CtextSurface surface1( m_graphics, t, rectp, m_cursor, m_gravity, m_shadow, m_cursorColour, f);
	}
	CtextSurface surface2( m_graphics, t, rect, m_cursor, m_gravity, m_colour, m_cursorColour, f);
}

/*============================================================================*/
///
/// @brief Set Text ID for text to display.
///
/// @param textId [in] Which Text ID to display.
///
/*============================================================================*/
void Ctext::setTextId( EtextId textId)
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
	m_textId =_NO_TEXT;
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
	if ( m_useTextId) { return (m_textId ==_NO_TEXT); }
	return m_value.empty();
}
