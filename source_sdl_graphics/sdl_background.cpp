/*
 * sdl_square.cpp
 *
 *  Created on: 23 dec. 2011
 *      Author: mensfort
 */

#include <math.h>
#include "sdl_background.h"
#include "graphics_widgets.h"
#include "sdl_dialog_object.h"

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
, m_borderWidth(0)
{
	setColours( col1, col2);
}

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
	}
	int r=(int)(fraction*(double)m_R1+(1.0-fraction)*(double)m_R2);
	int g=(int)(fraction*(double)m_G1+(1.0-fraction)*(double)m_G2);
	int b=(int)(fraction*(double)m_B1+(1.0-fraction)*(double)m_B2);
	return (r<<16) + (g<<8) + b;
}

void Cbackground::onPaint( int touch)
{
	int xx =(m_rect.left() << 3) +m_borderWidth;
	int yy =(m_rect.top() << 3)+m_borderWidth;
	int mx =(m_rect.right() << 3) -1-m_borderWidth;
	int my =(m_rect.bottom() << 3) -1-m_borderWidth;
	double height =(my-yy)+1;

	colour col1 =touch ? m_graphics->brighter(m_col1, -touch/2):m_col1;
	colour col2 =touch ? m_graphics->brighter(m_col2, -touch/2):m_col2;
	switch (m_pattern)
	{
	case FILL_NONE:
		return;
	case FILL_UNICOLOURED:
		m_graphics->setColour( col1);
		m_graphics->bar(xx, yy, mx+1, my+1, m_radius);
		return;
	case FILL_IMAGE:
		onPaintImageButton();
		return;
	default:
		m_B1 =(col1 & 0x0000FF);
	    m_G1 =(col1 & 0x00FF00) >> 8;
		m_R1 =(col1 & 0xFF0000) >> 16;
		m_B2 =(col2 & 0x0000FF);
		m_G2 =(col2 & 0x00FF00) >> 8;
		m_R2 =(col2 & 0xFF0000) >> 16;
		break;
	}
	for ( int x=0; x<=m_radius; x++)
	{
		int v=(int)( sqrt( (double)(m_radius*m_radius-(m_radius-x)*(m_radius-x))+0.5) );
		v =m_radius-v;
		m_graphics->setColour( calcColour( col1, col2, x/height));
		m_graphics->line( xx+v,yy+x, mx-v, yy+x);
		m_graphics->setColour( calcColour( col1, col2, (height-1.0-x)/height));
		m_graphics->line( xx+v,my-x, mx-v, my-x);
	}
	for ( int y=m_radius+yy; y<my-m_radius; y++)
	{
		m_graphics->setColour( calcColour( col1,col2, (y-yy)/height));
		m_graphics->line( xx,y, mx,y);
	}
	if ( m_key!=KEY_UNDEFINED)
	{
		int w =m_mainGraph->width();
		int h =m_mainGraph->height();
		m_graphics->setCode( Crect( 0, 0, w,h), m_key);
	}
}

void Cbackground::setColours( colour col1, colour col2)
{
	m_col1=col1;
	m_col2=col2;
	if ( col2<0 && col2>=-1000)
	{
		double f =-col2*0.01;
		int B = (col1 & 0x0000FF)*f;
		int G = ((col1 & 0x00FF00) >> 8)*f;
		int R = ((col1 & 0xFF0000) >> 16)*f;
		if ( B>0xff) B=0xff;
		if ( G>0xff) G=0xff;
		if ( R>0xff) R=0xff;
		m_col2 =(R<<16) +(G<<8) + B;
	}
}

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
			m_graphics->imageLine( m_image, xx+v,yy+x, mx-v, yy+x+1);
			m_graphics->imageLine( m_image, xx+v,my-1-x, mx-v, my-x);
		}
	}
	m_graphics->imageLine( m_image, xx, m_radius+yy, mx,my-m_radius);
	if ( m_key!=KEY_UNDEFINED)
	{
		int w =m_mainGraph->width();
		int h =m_mainGraph->height();
		m_graphics->setCode( Crect(0, 0, w,h), m_key);
	}
}

Cbackground::~Cbackground()
{
	// TODO Auto-generated destructor stub
}
