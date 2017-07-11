/*============================================================================*/
/**  @file       sdl_progress_bar.cpp
 **  @ingroup    zhongcan_sdl
 **  @brief		 Default dialog.
 **
 **  Create a progress bar for touchscreens. This is a dialog object.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CprogressBar
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
#include <stdio.h>
#include "sdl_button.h"
#include "sdl_progress_bar.h"
#include "sdl_image.h"

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
CprogressBar::CprogressBar( const Crect &rect, keybutton code)
: Cslider(rect,1,code,SLIDER_HORIZONTAL_BAR)
, m_todoTime(0)
, m_speed(1)
{
	start();
}

/*============================================================================*/
///
/// @brief Destructor.
///
/*============================================================================*/
CprogressBar::~CprogressBar()
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
void CprogressBar::onPaint( int touch)
{
	if ( !m_visible)
	{
		return;
	}
	Cslider::onPaint(touch);
	Cslider::paintHorizontalBar();
	//m_speed +=!m_speed;
	//m_todoTime =((m_maximum-m_minimum)-m_value)/m_speed;
	char t[32];
	sprintf(t, "%d", m_todoTime);
	Crect r( (m_rect.left()+m_rect.right())/2-4, m_rect.bottom(), 8,4);
	Cbutton btn( NULL, r, KEY_NOCHANGE, CtextFont(""),
			     t, BORDER_NONE, GRAVITY_BOTTOM_CENTER, 4, FILL_UNICOLOURED);
	btn.setColours(0,0xcccccc,0xcccccc);
	btn.onPaint(0);
}

void CprogressBar::setValue(double y)
{
	int n =m_timer.elapsed();
	Cslider::setValue(y);
	m_todoTime =(int)(m_maximum+1);
	if (n!=0)
	{
		m_speed =(int)(y*1000/n);
		if ( m_speed!=0)
		{
			m_todoTime =(int)(((m_maximum-m_minimum)-y)/m_speed);
		}
	}
}

void CprogressBar::start()
{
	m_timer.setTime(100000000,0,1);
}
