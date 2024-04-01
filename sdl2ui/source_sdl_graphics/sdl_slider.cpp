/*============================================================================*/
/**  @file       sdl_slider.cpp
 **  @ingroup    zhongcan_sdl
 **  @brief		 Default dialog.
 **
 **  Create a slider for touchscreens. This is a dialog object.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cslider
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
#include "sdl_slider.h"
#include "sdl_image.h"
#include "sdl_graphics.h"

#define TOUCH_SLIDER

/*============================================================================*/
///
/// @brief Constructor.
//
/// @param rect [in] Rectangle of slider
/// @param unitSize [in] Should be smaller than max-minimum, step size of slider
/// @param code [in] What code on display
/// @param type [in] Different types of slider
///
/*============================================================================*/
Cslider::Cslider( const Crect &rect, double unitSize, keybutton code, EsliderType type)
: CdialogObject( NULL, rect, code)
, m_sliderType( type)
, m_scrollbarColour( Cgraphics::m_defaults.button_text)
, m_backgroundColour( Cgraphics::m_defaults.button_background2)
, m_noBackground(true)
, m_imageEnable(false)
, m_image_x(0)
, m_image_y(0)
, m_unitSize( unitSize)
, m_value(-1000)
, m_minimum(0)
, m_maximum(1.0)
, m_unit(6.0)
, m_offDisabled( false)
, m_itemsOnScreen( m_rect.height()/m_unitSize)
{
	m_dragEnable =true;
}

/*============================================================================*/
///
/// @brief Destructor.
///
/*============================================================================*/
Cslider::~Cslider()
{
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
void Cslider::onPaint( int touch)
{
	(void)touch;
	if ( !m_visible)
	{
		return;
	}
	switch (m_sliderType)
	{
	case SLIDER_HORIZONTAL:
		paintHorizontal();
		break;

	case SLIDER_HORIZONTAL_BAR:
		paintHorizontalBar();
		break;

	case SLIDER_VERTICAL:
		paintVertical();
		break;

	case SLIDER_VERTICAL_UPDOWN:
		paintVertical();
		break;
	default:
		break;
	}
	enableDrag();
	// height_arrow=MAX(MIN((height*height)/(unit*(maximum-minimum)); height/2);unit)
	// offset_arrow=(height-2*height_arrow)*(y/(maximum-minimum))
}

void Cslider::onPaint( const Cpoint &p, int touch)
{
	(void)p; onPaint(touch);
}

#if 0
void Cslider::paintVerticalUpDown()
{
	int    maxHeight   =static_cast<int>(m_rect.height()*8/2);
	double heightArrow =(double)( m_rect.height()*m_rect.height())/(2*m_unit*(m_maximum-m_minimum))+0.5;
	if ( heightArrow>maxHeight)
	{
		heightArrow =maxHeight;
	}
	if ( heightArrow <m_unit)
	{
		heightArrow =m_unit;
	}
	int hh =static_cast<int>(heightArrow);
	double offset_arrow =(m_rect.height()-2*hh)*(m_value/(m_maximum-m_minimum-(m_rect.height()/m_unit)));
	int y1 =static_cast<int>(offset_arrow+0.5f+m_rect.top());
	if (y1<m_rect.top()) y1=m_rect.top();
	if (y1+hh+hh>m_rect.bottom()) y1=m_rect.bottom()-hh-hh;
	int y2 =y1+hh;
}
#endif

/// @brief Paint a vertical slider.
void Cslider::paintHorizontal()
{
	int  widthArrow =0;
	double  maxWidth =(m_rect.width()-widthArrow); // Image height removed.
	double offset_arrow =0;
	int slidingRange =(int)(m_maximum-m_minimum-m_itemsOnScreen);
	if ( slidingRange >0)
	{
		offset_arrow =maxWidth*8*(m_value-m_minimum) /(m_maximum-m_minimum-m_itemsOnScreen);
	}
	m_image_x=(int)offset_arrow+m_rect.left()*8;
	m_pGraphics->setColour( Cgraphics::m_defaults.slider_background);
	m_pGraphics->rectangle( m_rect.left()*8, m_rect.top()*8, m_rect.right()*8, m_rect.bottom()*8, 4,3);
	m_pGraphics->setColour( Cgraphics::m_defaults.slider_lines);
	int y=(m_rect.top()+m_rect.bottom())*8/2;
	int x1=m_rect.left()*8+24;
	int x2=m_rect.right()*8-24;
	m_pGraphics->line( x1,y, x2,y);
	m_pGraphics->line( x1,y-10, x1,y+10);
	m_pGraphics->line( (x1+x2)/2,y-5, (x1+x2)/2,y+5);
	m_pGraphics->line( x2,y-10, x2,y+10);

	int left =m_rect.left()*8+(int)m_rect.width()*8/2-24;
	m_pGraphics->image( Cgraphics::m_defaults.icon_slider48, left, m_image_y, left+47, m_image_y+47);
}

/// @brief Paint a vertical slider.
void Cslider::paintHorizontalBar()
{
	int  widthArrow =0;
	double  maxWidth =(m_rect.width()-widthArrow); // Image height removed.
	double offset_arrow =0;
	int slidingRange =(int)(m_maximum-m_minimum);
	if ( slidingRange >0)
	{
		offset_arrow =maxWidth*8*(m_value-m_minimum) /(m_maximum-m_minimum);
	}
	m_image_x=(int)offset_arrow+m_rect.left()*8;
	m_pGraphics->setColour( Cgraphics::m_defaults.slider_background);
	m_pGraphics->rectangle( m_rect.left()*8, m_rect.top()*8, m_rect.right()*8, m_rect.bottom()*8, 4,3);
	int y=(m_rect.top()+m_rect.bottom())*8/2;
	int x1=m_rect.left()*8+4;
	int x2=m_rect.right()*8-4;
	m_pGraphics->setColour( Cgraphics::m_defaults.line_bright);
	m_pGraphics->bar( x1,y-5,m_image_x,y+4,0);
	m_pGraphics->setColour( Cgraphics::m_defaults.slider_lines);
	m_pGraphics->line( x1,y, x2,y);
	m_pGraphics->line( x1,y-10, x1,y+10);
	m_pGraphics->line( (x1+x2)/2,y-5, (x1+x2)/2,y+5);
	m_pGraphics->line( x2,y-10, x2,y+10);
}

/// @brief Paint a vertical slider.
void Cslider::paintVertical()
{
	int  heightArrow =6;
	double  maxHeight =(m_rect.height()-heightArrow); // Image height removed.
	double offset_arrow =0;
	int slidingRange =(int)(m_maximum-m_minimum-m_itemsOnScreen);
	if ( slidingRange >0)
	{
		offset_arrow =maxHeight*8*(m_value-m_minimum) /(m_maximum-m_minimum-m_itemsOnScreen);
	}
	m_image_y=(int)offset_arrow+m_rect.top()*8;
	m_pGraphics->setColour( Cgraphics::m_defaults.slider_background);
	m_pGraphics->rectangle( m_rect.left()*8, m_rect.top()*8, m_rect.right()*8, m_rect.bottom()*8, m_rect.width()*4,m_rect.width()*3);
	m_pGraphics->setColour( Cgraphics::m_defaults.slider_lines);
	int x=(m_rect.left()+m_rect.right())*8/2;
	int y1=m_rect.top()*8+24;
	int y2=m_rect.bottom()*8-24;
	m_pGraphics->line( x,y1, x,y2);
	m_pGraphics->line( x-10,y1, x+10,y1);
	m_pGraphics->line( x-5,(y1+y2)/2, x+5, (y1+y2)/2);
	m_pGraphics->line( x-10,y2, x+10,y2);

	int left =m_rect.left()*8+(int)m_rect.width()*8/2-24;
	m_pGraphics->image( Cgraphics::m_defaults.icon_slider48, left, m_image_y, left+47, m_image_y+47);
}


/*============================================================================*/
///
/// @brief Paint background to button.
///
/*============================================================================*/
void Cslider::paintBackground()
{
}

/*============================================================================*/
///
/// @brief Paint border of button.
///
/*============================================================================*/
void Cslider::paintBorder()
{
}

/*============================================================================*/
///
/// @brief Paint image next to text.
///
/*============================================================================*/
void Cslider::paintImage()
{
}

/*============================================================================*/
///
/// @brief Set new text.
///
/// @param text [in] Text to display.
///
/*============================================================================*/
void Cslider::setImage( const std::string &image)
{
	m_image =image;
	m_imageEnable =(m_image.size() >0);
}

/*============================================================================*/
///
/// @brief Set new text.
///
/// @param text [in] Text to display.
///
/*============================================================================*/
void Cslider::setColours( colour scrollbar, colour background)
{
	(void)scrollbar;
	m_backgroundColour =background;
}

bool Cslider::wheelUp()
{
	m_value =m_value-(m_itemsOnScreen-1.01);
	if ( m_value<m_minimum)
	{
		m_value =m_minimum;
	}
	return true;
}

bool Cslider::wheelDown()
{
	m_value =m_value+(m_itemsOnScreen-1.01);
	if ( m_value>m_maximum-m_itemsOnScreen)
	{
		m_value =m_maximum-m_itemsOnScreen;
	}
	if ( m_value <m_minimum)
	{
		m_value =m_minimum;
	}
	if ( m_maximum-m_minimum+1<m_itemsOnScreen)
	{
		m_value =m_minimum;
	}
	return true;
}

/*============================================================================*/
///
/// @brief Start to drag
///
/// @param p [in] Position to start.
///
/*============================================================================*/
bool Cslider::onDragStart( Cpoint p)
{
#ifndef TOUCH_SLIDER
	int n=m_image_y;
	if ( p.y<8*(n-1))
	{
		m_offDisabled =true;
		wheelUp();
		return true;
	}
	if ( p.y>8*(n+7))
	{
		m_offDisabled =true;
		wheelDown();
		return true;
	}
#endif
	return onDrag(p);
}

/*============================================================================*/
///
/// @brief Drag finger to object.
///
/// @param p [in] Position finger.
///
/*============================================================================*/
bool Cslider::onDragEnd( Cpoint p)
{
	bool retVal =onDrag( p);
	m_offDisabled =false;
	return retVal;
}

/*============================================================================*/
///
/// @brief Release the finger.
///
/// @param p [in] Last position finger.
///
/*============================================================================*/
bool Cslider::onDrag( Cpoint p)
{
	if ( m_offDisabled==true)
	{
		return false;
	}
	if ( m_sliderType ==SLIDER_VERTICAL)
	{
		int y=p.y-m_rect.top()*8-24;
		double value =m_minimum+(m_maximum-m_minimum-m_itemsOnScreen)*y/(m_rect.height()*8-48.0);
		if ( value>m_maximum-m_itemsOnScreen)
		{
			value =m_maximum-m_itemsOnScreen;
		}
		if ( m_maximum-m_minimum+1<m_itemsOnScreen)
		{
			value=m_minimum;
		}
		if ( value<m_minimum)
		{
			value=m_minimum;
		}
		if ( value !=m_value)
		{
			//Cgraphics::m_defaults.log( "Slider =%f", value);
			m_value =value;
			return true;
		}
	}
	return false;
}

void Cslider::setValue( double y)
{
	m_value =gLimit( y, m_minimum, m_maximum-m_itemsOnScreen);
}

double Cslider::getValue()
{
	return m_value;
}

int Cslider::getInt()
{
	return (int)(m_value+0.5);
}

bool Cslider::setRange( double minimum, double maximum)
{
	bool retVal =false;
	m_minimum =minimum;
	m_maximum =maximum;
	if ( maximum<minimum)
	{
		maximum = minimum;
	}
	if ( m_value<m_minimum)
	{
		m_value =m_minimum;
		retVal =true;
	}
	if ( m_value>m_maximum-m_itemsOnScreen)
	{
		m_value =m_maximum-m_itemsOnScreen;
		retVal =true;
	}
	if ( m_value<m_minimum)
	{
		m_value =m_minimum;
		retVal =true;
	}
	return retVal;
}
