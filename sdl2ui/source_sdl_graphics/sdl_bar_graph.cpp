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
	double y1;
	double y2;
	m_rect =rect;
	if ( !m_visible)
	{
		return;
	}

	double bottom =(m_rect.bottom()-m_textHeight)*8;
	int left =m_rect.left()*8;
	int right =m_rect.right()*8;
	int top =m_rect.top()*8;

	/// Display background + helpLines
	m_graphics->setColour( m_background);
	m_graphics->bar( left, top, right, static_cast<int>(bottom), 0);
	int c=m_helpLines;
	m_graphics->setColour(c);
	for ( int n=1; n<m_helpLines; n++)
	{
		int y=(int)((bottom-top)*n/m_helpLines);
		for ( int x=left; x<right; x+=8)
		{
			m_graphics->line( x,y, x+3, y);
		}
	}
	double height =(bottom-top)/(m_maximum-m_minimum);

	// Add text
	CgraphButton v( m_parent, Crect( rect.left(), rect.bottom()-m_textHeight, rect.width(), m_textHeight), m_text, m_rotate);
	v.setBackgroundColour( m_background);
	v.onPaint( touch);

	// Add spacing
	int spacingLeft =(int)((m_spacing+1)/2);
	int spacingRight =(int)(m_spacing/2);
	if ( spacingLeft)
	{
		//m_graphics->setColour( m_background);
		//m_graphics->bar( left, top, left+spacingLeft, (int)bottom, 0);
		left +=spacingLeft;
		if ( spacingRight)
		{
			right -=spacingRight;
		}
	}
	y1 =static_cast<int>(bottom);
	y2 =y1;

	if ( m_stacked)
	{
		// Bars on top of each other.
		for ( int i=0; i<(int)m_value.size(); i++)
		{
			double v=m_value[i];
			colour c=m_colour[i];
			if (v<0.01)
			{
				continue;
			}
			double n=v*height;
			bottom-=n;
			if ( bottom<0)
			{
				bottom=0;
			}
			y1=y2;
			y2=static_cast<int>(bottom);
			if ( (int)y2<=top)
			{
				m_graphics->setColour( c);
				m_graphics->bar( left+1, top+1, right-1, (int)y2-1, 0);
				int d=m_graphics->brighter( c, -15);
				m_graphics->setColour( d);
				m_graphics->rectangle( left, top, right, (int)y2, 2,0);
			}
			else
			{
				m_graphics->setColour( c);
				m_graphics->bar( left+1, (int)y2+1, right-1, (int)y1-1, 0);
				int d=m_graphics->brighter( c, -15);
				m_graphics->setColour( d);
				m_graphics->rectangle( left, (int)y2, right, (int)y1, 2,0);
			}
		}
	}
	else
	{
		double zero =bottom+m_minimum*height+0.5;
		// Bars next to each other.
		int x1=0,x2=left;
		for ( int i=0; i<(int)m_value.size(); i++)
		{
			x1=x2;
			x2 =left+(i+1)*(right-left)/m_value.size();
			double v=m_value[i];
			if (v==0)
			{
				continue;
			}
			v-=m_minimum;
			colour c=m_colour[i];
			double n=v*height;
			y1 =zero-n;
			if ( y1<0) y1=0;
			if ( y1>bottom) y1=bottom;

			if ( v!=0)
			{
				m_graphics->setColour( c);
				m_graphics->bar( x1, (int)y1, x2, (int)zero, 0);
				int d=m_graphics->brighter( c, -15);
				m_graphics->setColour( d);
				m_graphics->rectangle( x1, (int)y1, x2, (int)zero, 0);
			}
		}
	}
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

void CbarGraph::setColours( int n, colour background)
{
	while ( (int)m_colour.size()<n)
	{
		m_colour.push_back(0);
	}
	m_colour[n] =background;
}
