/*============================================================================*/
/**  @file       sdl_bar_graph.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Default dialog.
 **
 **  Create a bar graph. This is a dialog object, which contains one bar only.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CbarGraph
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
#include "sdl_button.h"
#include "sdl_bar_graph.h"

/// Colour help lines
colour CbarGraph::m_defaultColour[8]={ 0x111111, 0x222222, 0x333333, 0x444444,
                                       0x555555, 0x666666, 0x777777, 0x888888 };

/*============================================================================*/
///
/// @brief Constructor.
//
/// @param x [in] Position.
/// @param y [in] Position.
/// @param width [in] Position.
/// @param height [in] Position.
///
/*============================================================================*/
CbarGraph::CbarGraph( Cdialog *parent, double value1, double value2, bool stacked,
		              const std::string &text, int spacing, int helplines,
		              colour back)
: CdialogObject( parent, Crect(0,0,1,1), KEY_F1)
, m_minimum( 0)
, m_maximum( 0)
, m_stacked( stacked)
, m_spacing( spacing)
, m_background( back)
, m_text( text)
, m_textHeight( 12)
, m_helpLines( helplines)
, m_left(0)
, m_right(0)
, m_showValue(true)
{
	m_value.push_back( value1);
	m_value.push_back( value2);
	m_colour.push_back( m_defaultColour[0]);
	m_colour.push_back( m_defaultColour[1]);
	m_rotate =false;
}

/*============================================================================*/
///
/// @brief Constructor.
//
/// @param x [in] Position.
/// @param y [in] Position.
/// @param width [in] Position.
/// @param height [in] Position.
///
/*============================================================================*/
CbarGraph::CbarGraph( Cdialog *parent, double value1, bool stacked,
		              const std::string &text, int spacing, colour helplines,
		              colour back)
: CdialogObject( parent, Crect(0,0,1,1), KEY_F1)
, m_minimum( 0)
, m_maximum( 0)
, m_stacked( stacked)
, m_spacing( spacing)
, m_background( back)
, m_text( text)
, m_textHeight( 12)
, m_helpLines( helplines)
, m_left(0)
, m_right(0)
, m_showValue(true)
{
	m_value.push_back( value1);
	m_colour.push_back( m_defaultColour[0]);
	m_rotate =false;
}

/*============================================================================*/
///
/// @brief Destructor.
///
/*============================================================================*/
CbarGraph::~CbarGraph()
{
	// TODO Auto-generated destructor stub
}

void CbarGraph::paintHelpLines()
{
	int top =m_rect.top()*8;
	int bottom =(m_rect.bottom()-m_textHeight)*8;

	m_pGraphics->setColour( m_background);
	m_pGraphics->bar( m_left, top, m_right, static_cast<int>(bottom), 0);

	int c=m_helpLines;
	m_pGraphics->setColour(c);
	m_helpLines = 1;
	for ( int n=1; n<m_helpLines; n++)
	{
		int y=(int)((bottom-top)*n/m_helpLines);
		for ( int x=m_left; x<m_right; x+=8)
		{
			m_pGraphics->line( x,y, x+3, y);
		}
	}
}

void CbarGraph::addSpacing()
{
	// Add spacing
	int spacingLeft =(int)((m_spacing+1)/2);
	int spacingRight =(int)(m_spacing/2);
	if ( spacingLeft)
	{
		m_left +=spacingLeft;
	}
	if ( spacingRight)
	{
		m_right -=spacingRight;
	}
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
void CbarGraph::onPaint( const Crect &rect, int touch)
{
	m_rect =rect;
	m_left =m_rect.left()*8;
	m_right =m_rect.right()*8;

	if ( !m_visible)
	{
		return;
	}

	/// Display background + helpLines
	addSpacing();
	paintHelpLines();

	int top =m_rect.top()*8;
	double bottom =(m_rect.bottom()-m_textHeight)*8;
	double scale =(bottom-top)/(m_maximum-m_minimum);

	// Add text
	CgraphButton v( m_pParent, Crect( rect.left(), rect.bottom()-m_textHeight, rect.width(), m_textHeight), m_text, m_rotate);
	if (m_font.length())
	{
		v.setFont(CtextFont(m_font.c_str()));
	}
	v.setBackgroundColour( m_background);
	v.onPaint( touch);

	double yTop = top;
	double yBottom = static_cast<int>(bottom);; // bottom to paint; // top to paint

	int x1 =m_left;
	int x2 =m_right;

	for (int i=0; i<(int)m_value.size(); i++)
	{
		if (!m_stacked)
		{
			x1= m_left+(i)*(m_right-m_left)/m_value.size();
			x2 =m_left+(i+1)*(m_right-m_left)/m_value.size();
		}
		double heightNow =(m_value[i]-m_minimum)*scale;

		heightNow =gLimit(heightNow, 0, yBottom-yTop);
		colour c = m_colour[i];
		if (heightNow>2)
		{
			m_pGraphics->setColour( c);
			m_pGraphics->bar( x1+1, yBottom-heightNow+1, x2-1, (int)yBottom-1, 0);
		}
		if (heightNow>=1)
		{
			int d=m_pGraphics->brighter( c, -15);
			m_pGraphics->setColour( d);
			m_pGraphics->rectangle( x1, yBottom-heightNow, x2, (int)yBottom, 2,0);
			if (m_stacked)
			{
				yBottom-=heightNow;
			}
		}
	}
	char s[32];
	sprintf(s, "%d", (int)m_value[0]);
	CgraphButton w( m_pParent, Crect( rect.left(), rect.top(), rect.width(), rect.height()-m_textHeight), s, false);
	if (m_font.length())
	{
		w.setFont(CtextFont(m_font.c_str()));
	}
	w.m_noBackground =true;
	w.onPaint( touch);
 }

void CbarGraph::rotate()
{
	m_rotate =true;
}

void CbarGraph::setRange( double minimum, double maximum)
{
	m_minimum =minimum;
	m_maximum =maximum;
}

void CbarGraph::setColourHelpLines( colour n)
{
	m_helpLines =n;
}

void CbarGraph::setTextHeight(int n)
{
	m_textHeight =n;
}

void CbarGraph::setFont(const std::string &font)
{
	m_font =font;
}

void CbarGraph::setColours( int n, colour background)
{
	while ( (int)m_colour.size()<n)
	{
		m_colour.push_back(0);
	}
	m_colour[n] =background;
}
