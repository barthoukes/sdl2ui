/*
 * sdl_rectangle.cpp
 *
 *  Created on: 23 dec. 2011
 *      Author: mensfort
 */

#include <assert.h>
#include "sdl_rectangle.h"

/*============================================================================*/
///
/// @brief Constructor. A rectangle is a square which is filled.
///
/// @param name [in] Name of rectangle.
/// @param rect [in] Rectangle position and size.
/// @param code [in] Code for button.
/// @param border [in] What border.
/// @param back [in] Background colour.
///
/*============================================================================*/
Crectangle::Crectangle( Cdialog *parent,
		                const Crect &rect,
		                keybutton code,
		                EborderType border,
		                colour back,
		                int corner,
		                EfillType fill,
		                colour col2)
: CdialogObject( parent, rect, code)
, m_square(	parent, rect, code, back, corner, fill)
, m_border( border)
, m_border1( back)
, m_border2( col2)
, m_fill( FILL_UNICOLOURED)
{
}

Crectangle::~Crectangle()
{
}

/// @brief Change style rectangle.
void Crectangle::setFillStyle( EfillType pattern)
{
	m_square.setFillStyle( pattern);
}

/*============================================================================*/
///
/// @brief Paint border of button.
///
/*============================================================================*/
void Crectangle::onPaint( int touch)
{
	if ( m_graphics ==NULL)
	{
		return;
	}
	int linewidth =2;
	colour col1=0,col2=0;

	switch (m_border)
	{
	case BORDER_NONE:
		return;
	case BORDER_THIN:
		col1 =m_border1;
		col2 =m_border1;
		break;
	case BORDER_THIN_SHADOW:
	case BORDER_THIN_BUTTON_SHADOW:
		col1 =m_border1;
		col2 =m_border2;
		break;
	case BORDER_THIN_SELECTED:
		col1 =m_border2;
		col2 =m_border1;
		break;
	case BORDER_THICK:
		linewidth =4;
		col1 =m_border1;
		col2 =m_border1;
		break;
	case BORDER_THICK_SHADOW:
		linewidth =4;
		col1 =m_border1;
		col2 =m_border2;
		break;
	case BORDER_ITEM_THICK:
		linewidth =4;
		col1 =m_border2;
		col2 =m_border2;
		break;
	case BORDER_THICK_SELECTED:
		linewidth =4;
		col1 =m_border2;
		col2 =m_border1;
		break;
	}
	int x1 = ( m_rect.left() << 3 ), y1 = (m_rect.top() << 3);
	int x2 = ( m_rect.right() << 3), y2 = (m_rect.bottom() << 3);

	m_graphics->lock();
	for (int a=0; a<linewidth; a++)
	{
		m_graphics->setColour( col1 );
		m_graphics->line( x1+a, y1+a, x2-a-1, y1+a);
		m_graphics->line( x2-a-1, y1+a+1, x2-a-1, y2-a-1);
		m_graphics->setColour( col2 );
		m_graphics->line( x2-a-1, y2-a-1, x1+a, y2-a-1);
		m_graphics->line( x1+a, y2-a-1, x1+a, y1+a+1);
	}
	m_graphics->unlock();
}

void Crectangle::setBackground( EfillType pattern, colour background1, colour foreground2, colour background2)
{
	m_square.setFillStyle( pattern);
	m_square.setColours( background1, background2);
	m_border1 =foreground2;
	m_border2 =background1;
}

void Crectangle::setCode( keybutton key)
{
	m_square.setCode( key);
}
