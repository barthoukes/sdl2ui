/*============================================================================*/
/**  @file      sdl_background.cpp
 **  @ingroup   sdl2ui
 **  @brief		Any background for buttons, images etc.
 **
 **  Class to define a background
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cbackground
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
#include <math.h>
#include "sdl_background.h"
#include "sdl_dialog_object.h"

/*----------------------------------------------------------------------------*/
Cbackground::Cbackground( Cdialog *parent, const Crect &rect,
		          keybutton code, colour col1, int radius, EfillType fill, colour col2)
: CdialogObject( parent, rect, code)
, m_col1( col1)
, m_col2( col2)
, m_pattern( fill)
, m_key( KEY_UNDEFINED)
, m_radius( radius)
, m_image("")
, m_R1(0)
, m_G1(0)
, m_B1(0)
, m_R2(0)
, m_G2(0)
, m_B2(0)
, m_pieCorner(45)
, m_borderWidth(0)
{
	setColours( col1, col2);
}

/*----------------------------------------------------------------------------*/
// n=0..height
colour Cbackground::calcColour( colour col1, colour col2, double fraction)
{
	switch (m_pattern)
	{
	case FILL_2COLOURS:
		return ( fraction>=0.5) ? col2:col1;
	case FILL_GRADIENT:
		break;
	case FILL_PYRAMID:
		if ( fraction>=0.5)
		{
			fraction=(1.0-fraction)*2;
		}
		else
		{
			fraction*=2.0;
		}
		break;
	default:
	case FILL_UNICOLOURED:
	case FILL_NONE:
		return col1;
	case FILL_CIRCULAR:
		break;
	}
	int r=(int)(fraction*(double)m_R1+(1.0-fraction)*(double)m_R2);
	int g=(int)(fraction*(double)m_G1+(1.0-fraction)*(double)m_G2);
	int b=(int)(fraction*(double)m_B1+(1.0-fraction)*(double)m_B2);
	return (r<<16) + (g<<8) + b;
}

/*----------------------------------------------------------------------------*/
bool Cbackground::pieColour( int dx, int dy)
{
	int corner =(int)(atan2( dx,-dy)*360.0/6.2928);
	if ( corner<0) corner+=360;
	if ( corner>=360) corner-=360;
	return ( corner >= m_pieCorner );
}

/*----------------------------------------------------------------------------*/
/** @brief Draw the background with a touch alue
 *  @param touch 0 = not pressed, 100 = pressed, 99..1 = releasing
 */
void Cbackground::onPaint( int touch)
{
	int xx =(m_rect.left() << 3) +m_borderWidth;
	int yy =(m_rect.top() << 3)+m_borderWidth;
	int mx =(m_rect.right() << 3) -1-m_borderWidth;
	int my =(m_rect.bottom() << 3) -1-m_borderWidth;
	double height =(my-yy)+1;
	double width =(mx-xx)+1;
	double middle_x=(mx+xx)/2;
	double middle_y=(my+yy)/2;
	int x=0;
	int y=0;
	double maxf;

	colour col1 =touch ? m_pGraphics->brighter(m_col1, -touch/4):m_col1;
	colour col2 =touch ? m_pGraphics->brighter(m_col2, -touch/4):m_col2;
	switch (m_pattern)
	{
	case FILL_NONE:
		return;
	case FILL_UNICOLOURED:
		m_pGraphics->setColour( col1);
		m_pGraphics->bar(xx, yy, mx+1, my+1, m_radius);
		return;
	case FILL_IMAGE:
		onPaintImageButton();
		return;
	case FILL_2COLOURS:
	case FILL_GRADIENT:
	case FILL_PYRAMID:
		m_B1 =(col1 & 0x0000FF);
	    m_G1 =(col1 & 0x00FF00) >> 8;
		m_R1 =(col1 & 0xFF0000) >> 16;
		m_B2 =(col2 & 0x0000FF);
		m_G2 =(col2 & 0x00FF00) >> 8;
		m_R2 =(col2 & 0xFF0000) >> 16;
		for ( y=0; y<=m_radius; y++)
		{
			int v=(int)( sqrt( (double)(m_radius*m_radius-(m_radius-y)*(m_radius-y))+0.5) );
			v =m_radius-v;
			m_pGraphics->setColour( calcColour( col1, col2, y/height));
			m_pGraphics->line( xx+v,yy+y, mx-v, yy+y);
			m_pGraphics->setColour( calcColour( col1, col2, (height-1.0-y)/height));
			m_pGraphics->line( xx+v,my-y, mx-v, my-y);
		}
		for ( y=m_radius+yy; y<my-m_radius; y++)
		{
			m_pGraphics->setColour( calcColour( col1,col2, (y-yy)/height));
			m_pGraphics->line( xx,y, mx,y);
		}
		if ( m_key!=KEY_UNDEFINED)
		{
			m_pGraphics->setKey( Crect( 0, 0, m_mainGraph->width(),m_mainGraph->height()), m_key);
		}
		break;
	case FILL_CIRCULAR:
		{
			//maxf =sqrt(((width-m_radius)*(width-m_radius)+(height-m_radius)*(height-m_radius))/4);
			maxf =width/2.2;
			double dist;
			double len;
			m_pGraphics->setColour( col1);
			m_B1 =(col1 & 0x0000FF);
			m_G1 =(col1 & 0x00FF00) >> 8;
			m_R1 =(col1 & 0xFF0000) >> 16;
			m_B2 =(col2 & 0x0000FF);
			m_G2 =(col2 & 0x00FF00) >> 8;
			m_R2 =(col2 & 0xFF0000) >> 16;
			for ( y=0; y<=m_radius; y++)
			{
				int v=(int)( sqrt( (double)(m_radius*m_radius-(m_radius-y)*(m_radius-y))+0.5) );
				v =m_radius-v;
				for ( x=xx+v; x<=mx-v; x++)
				{
					len =(x-middle_x)*(x-middle_x)+(middle_y-(yy+y))*(middle_y-(yy+y));
					dist =sqrt( len)/maxf;
					m_pGraphics->transparantPixel( x, yy+y, dist);
					m_pGraphics->transparantPixel( x, my-y, dist);

				}
			}
			for ( y=m_radius+yy; y<my-m_radius; y++)
			for ( x=xx; x<=mx; x++)
			{
				len =(x-middle_x)*(x-middle_x)+(y-middle_y)*(y-middle_y);
				dist =sqrt( len)/maxf;
				m_pGraphics->transparantPixel( x, y, dist);
			}
			if ( m_key!=KEY_UNDEFINED)
			{
				m_pGraphics->setKey( Crect( 0, 0, m_mainGraph->width(),m_mainGraph->height()), m_key);
			}
			break;
		}
	case FILL_PIE:
		{
			maxf =width/2.2;
			m_pGraphics->setColour( col1);
			m_B1 =(col1 & 0x0000FF);
			m_G1 =(col1 & 0x00FF00) >> 8;
			m_R1 =(col1 & 0xFF0000) >> 16;
			m_B2 =(col2 & 0x0000FF);
			m_G2 =(col2 & 0x00FF00) >> 8;
			m_R2 =(col2 & 0xFF0000) >> 16;
			for ( y=0; y<=m_radius; y++)
			{
				int v=(int)( sqrt( (double)(m_radius*m_radius-(m_radius-y)*(m_radius-y))+0.5) );
				v =m_radius-v;
				for ( x=xx+v; x<=mx-v; x++)
				{
					// Define corner
					colour col =pieColour( (int)(x-middle_x), (int)((yy+y)-middle_y) ) ? col1:col2;
					m_pGraphics->setColour( col );
					m_pGraphics->setPixel( x, yy+y );
					col =pieColour( (int)(x-middle_x), (int)((my-y)-middle_y) ) ? col1:col2;
					m_pGraphics->setColour( col );
					m_pGraphics->setPixel( x, my-y );
				}
			}
			for ( y=m_radius+yy; y<my-m_radius; y++)
			for ( x=xx; x<=mx; x++)
			{
				colour col =pieColour( (int)(x-middle_x), (int)(y-middle_y) ) ? col1:col2;
				m_pGraphics->setColour( col );
				m_pGraphics->setPixel( x, y );
			}
			if ( m_key!=KEY_UNDEFINED)
			{
				m_pGraphics->setKey( Crect( 0, 0, m_mainGraph->width(),m_mainGraph->height()), m_key);
			}
			break;
		}

	}
}

/*----------------------------------------------------------------------------*/
/** @brief Set colours for the background.
 *  @param col1 [in] First colour
 *  @param col2 [in] Second colour
 */
void Cbackground::setColours( colour col1, colour col2)
{
    if (col1 != -1)
    {
        m_col1 = col1;
    }
    if (col2 != -1)
    {
        m_col2=col2;
    }
	if ( col2<0 && col2>=-1000)
	{
		double f =-col2*0.01;
		int B = (int)((col1 & 0x0000FF)*f);
		int G = (int)(((col1 & 0x00FF00) >> 8)*f);
		int R = (int)(((col1 & 0xFF0000) >> 16)*f);
		if ( B>0xff) B=0xff;
		if ( G>0xff) G=0xff;
		if ( R>0xff) R=0xff;
		m_col2 =(R<<16) +(G<<8) + B;
	}
}

/*----------------------------------------------------------------------------*/
void Cbackground::onPaintImageButton()
{
	int xx =(m_rect.left() << 3)+m_borderWidth;
	int yy =(m_rect.top() << 3)+m_borderWidth;
	int mx =(m_rect.right() << 3) -m_borderWidth;
	int my =(m_rect.bottom() << 3) -m_borderWidth;

	if ( m_radius>0)
	{
		for ( int x=0; x<=m_radius; x++)
		{
			int v=(int)( sqrt( (double)(m_radius*m_radius-(m_radius-x)*(m_radius-x))+0.5) );
			v =m_radius-v;
			m_pGraphics->imageLine( m_image, xx+v,yy+x, mx-v, yy+x+1);
			m_pGraphics->imageLine( m_image, xx+v,my-1-x, mx-v, my-x);
		}
	}
	m_pGraphics->imageLine( m_image, xx, m_radius+yy, mx,my-m_radius);
	if ( m_key!=KEY_UNDEFINED)
	{
		int w =m_mainGraph->width();
		int h =m_mainGraph->height();
		m_pGraphics->setKey( Crect(0, 0, w,h), m_key);
	}
}

/*----------------------------------------------------------------------------*/
Cbackground::~Cbackground()
{
}

/*----------------------------------------------------------------------------*/
Crect Cbackground::getRect() const
{
    return m_rect;
}
