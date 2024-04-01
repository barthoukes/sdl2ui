/*============================================================================*/
/**  @file       sdl_hand_clock.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Ask hours and minutes by handwriting.
 **
 **  Create a timestamp.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              ChandClock
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
#include "sdl_hand_clock.h"
#include "sdl_surface.h"
#include "sdl_dialog_event.h"
#include "sdl_background.h"
#include "timeout.hpp"
#include <math.h>

#define TWO_THIRD

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
ChandClock::ChandClock( Cdialog *parent, const Crect &rect, colour hourColour,
		colour minutesColour, colour fingerColour, const Sfont &font, colour textColour)
: CdialogObject( parent, rect, KEY_NONE)
, m_hours(12)
, m_minutes(15)
, m_state(CLOCK_STATE_NONE)
, m_startHour(10)
, m_startMinute(15)
, m_stroke(0)
, m_distance(1000)
, m_started(false)
, m_lastPoint(0,0)
, m_textColour(textColour)
, m_textButton( parent, rect, KEY_NOCHANGE, font, "",
		 	    textColour, GRAVITY_CENTER)
, m_hourPoint(0,0)
, m_minutesPoint(0,0)
, h_center(0)
, v_center(0)
, m_resolution(1)
, m_minutesArc( m_rect.width()*8, m_rect.height()*8, minutesColour)
, m_hoursArc( m_rect.width()*8*2/3, m_rect.height()*8*2/3, hourColour)
, m_fingerArc( m_rect.width()*8*1/6, m_rect.height()*8*1/6, fingerColour)
, m_amPmArc( m_rect.width()*8*1/3, m_rect.height()*8*1/3, minutesColour)
, m_isAm(true)
{
	m_hourColour[0] =hourColour;
	m_hourColour[1] =m_pGraphics->brighter(hourColour, -40);
	m_hourColour[2] =m_pGraphics->brighter(hourColour, 40);
	m_minutesColour[0] =minutesColour;
	m_minutesColour[1] =m_pGraphics->brighter(minutesColour, -40);
	m_minutesColour[2] =m_pGraphics->brighter(minutesColour, 40);
	m_radius0 = m_rect.width()*8*1/6;
	m_radius1 = m_rect.width()*8*2/6;
	m_radius2 = m_rect.width()*8*3/6;

    enablePainting();
    setVisible(true);
}

/** @brief  Clear the handwriter background. */
void ChandClock::clearImage()
{
}

bool ChandClock::isChanged()
{
	return (m_minutes!=m_startMinute || m_hours!=m_startHour);
}

/*============================================================================*/
///
/// @brief Destructor.
///
/*============================================================================*/
ChandClock::~ChandClock()
{
}

/*----------------------------------------------------------------------------*/
void ChandClock::setResolution(int res)
{
    m_resolution=res;
}

/*----------------------------------------------------------------------------*/
int ChandClock::getDayMinutes() const
{
    return m_minutes+m_hours*60;
}

/*----------------------------------------------------------------------------*/
int ChandClock::getHours() const
{
    return m_hours;
}

/*----------------------------------------------------------------------------*/
int ChandClock::getMinutes() const
{
    return m_minutes;
}

/** @brief We drag our finger on the screen to another location.
 *  @param p [in] New location of our finger.
 *  @return true on success.
 */
bool ChandClock::onPaintingMove( const Cpoint &point)
{
	int x=point.x-h_center;
	int y=point.y-v_center;
	int angle;
	int temp;
	switch ( m_state)
	{
	case CLOCK_STATE_HOURS:
		angle =(int)( atan2(y, x)*360/(2*M_PI) +0.5)+90+360;
		temp = (((angle+15)/30)%12)+( m_isAm ? 0:12);
		if ( temp !=m_hours)
		{
			m_hours =temp;
			onPaint(0);
		}
		break;

	case CLOCK_STATE_MINUTES:
		angle =(int)( atan2(y, x)*360/(2*M_PI)+0.5)+90+360;
		temp = ((angle+m_resolution*3)/6)%60;
		temp -=(temp%m_resolution);
		if ( temp !=m_minutes)
		{
			m_minutes =temp;
			onPaint(0);
		}
		break;

	case CLOCK_STATE_AMPM:
		onPaint(0);
		break;

	default:
		onPaintingStart(point);
		break;
	}
	return true;
}

/** @brief Start with next stroke */
bool ChandClock::onPaintingStart( const Cpoint &p)
{
	int dist = p.distance(Cpoint(h_center, v_center));
	if ( dist<(m_radius0-4)*(m_radius0-4))
	{
		m_state =CLOCK_STATE_AMPM;
		m_isAm =!m_isAm;
		m_hours =(m_hours%12)+ (m_isAm ? 0:12);
		onPaintingMove(p);
	}
	else if (dist <(m_radius0+4)*(m_radius0+4))
	{
		// in between somewhere
	}
	else if ( dist<(m_radius1-4)*(m_radius1-4))
	{
		m_state = CLOCK_STATE_HOURS;
		onPaintingMove(p);
	}
	else if ( dist>(m_radius1+4)*(m_radius1+4))
	{
		m_state = CLOCK_STATE_MINUTES;
		onPaintingMove(p);
	}
	return true;
}

/** @brief Drag finger on screen, now release your finger.
 *  @param p [in] Point where we release the finger.
 *  @return true when correct.
 */
bool ChandClock::onPaintingStop( const Cpoint &point)
{
	(void)point;
	m_state = CLOCK_STATE_NONE;
	return true;
}

void ChandClock::setTime(int hours, int minutes)
{
	m_startHour =hours;
	m_startMinute =minutes;
	m_hours=hours;
	m_minutes=minutes;
	m_isAm =(hours<12);
}


void ChandClock::setCenter( const Cpoint &center)
{
	CdialogObject::setCenter( center);
	m_textButton.setCenter( center);
}

/** @brief Add a point to the symbol line.
 *  @param x [in] X-coordinate of mouse.
 *  @param y [in] Y-coordinate of mouse.
 */
void ChandClock::addPoint( int x, int y)
{
	(void)x; (void)y;
}

/** @brief Paint 4 circles with arc
 */
void ChandClock::onPaint( int touch)
{
	if ( !m_visible)
	{
		return;
	}
	(void)touch;
	h_center = (m_rect.left()+m_rect.right())*8/2;
	v_center = (m_rect.top()+m_rect.bottom())*8/2;
	m_minutesPoint.x = m_rect.width()*cos(2*M_PI*(m_minutes-15)/60.0)*4*5/6 + h_center;
	m_minutesPoint.y = m_rect.height()*sin(2*M_PI*(m_minutes-15)/60.0)*4*5/6 + v_center;
	m_hourPoint.x = m_rect.width()*cos(2*M_PI*(m_hours-3)/12.0)*2 + h_center;
	m_hourPoint.y = m_rect.height()*sin(2*M_PI*(m_hours-3)/12.0)*2 + v_center;

	m_minutesArc.onPaint(m_pGraphics, 0, h_center, v_center, m_minutes*6);
	m_hoursArc.onPaint(m_pGraphics, 0, h_center, v_center, (m_hours%12)*30);
	m_fingerArc.onPaint(m_pGraphics, 0, m_hourPoint.x, m_hourPoint.y, 0);
	m_fingerArc.onPaint(m_pGraphics, 0, m_minutesPoint.x, m_minutesPoint.y, 0);
	m_amPmArc.onPaint( m_pGraphics, 0, h_center, v_center, 0);
	char s[16];
	sprintf(s, "%02d:%02d", m_hours, m_minutes);
	m_textButton.onPaintText(s, 0);
	m_pGraphics->update(m_rect.left()*8, m_rect.top()*8, m_rect.width()*8, m_rect.height()*8);
	// Set mouse touch.
	m_pGraphics->setKey( m_rect, m_code);
}

/** @brief After recognizing 14 symbols, we want to get number N in the list.
 *  @param offset [in] Which one 0..13
 *  @return String with that specific character.
 */
std::string ChandClock::get( size_t offset)
{
	(void)offset;
	return "";
}
