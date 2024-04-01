/*
 * sdl_rectangle.cpp
 *
 *  Created on: 23 dec. 2011
 *      Author: mensfort
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

/*---------------------------------------------------------------------------*/
Crectangle::~Crectangle()
{
}

/*---------------------------------------------------------------------------*/
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
	(void)touch;
	if ( m_pGraphics ==NULL)
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

	m_pGraphics->lock();
	for (int a=0; a<linewidth; a++)
	{
		m_pGraphics->setColour( col1 );
		m_pGraphics->line( x1+a, y1+a, x2-a-1, y1+a);
		m_pGraphics->line( x2-a-1, y1+a+1, x2-a-1, y2-a-1);
		m_pGraphics->setColour( col2 );
		m_pGraphics->line( x2-a-1, y2-a-1, x1+a, y2-a-1);
		m_pGraphics->line( x1+a, y2-a-1, x1+a, y1+a+1);
	}
	m_pGraphics->unlock();
}

/*---------------------------------------------------------------------------*/
void Crectangle::setBackground( EfillType pattern, colour background1, colour foreground2, colour background2)
{
	m_square.setFillStyle( pattern);
	m_square.setColours( background1, background2);
	m_border1 =foreground2;
	m_border2 =background1;
}

/*---------------------------------------------------------------------------*/
void Crectangle::setKey( keybutton key)
{
	m_square.setKey( key);
}
