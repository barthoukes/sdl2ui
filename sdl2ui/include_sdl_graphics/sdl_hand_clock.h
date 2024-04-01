/*============================================================================*/
/**  @file      sdl_hand_clock.h
 **  @ingroup   user_interface
 **  @brief		hand-clock library.
 **
 **  Create timestamp.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              ChandClock
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2011, 2014, 2018
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

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <vector>
#include <memory>
#include "sdl_fast_arc.h"
#include "sdl_types.h"
#include "sdl_text.h"

typedef enum
{
	CLOCK_STATE_NONE,
	CLOCK_STATE_AMPM,
	CLOCK_STATE_HOURS,
	CLOCK_STATE_MINUTES
} EclockState;

/// @brief  Create and display buttons.
class ChandClock : public CdialogObject
{
public:
	ChandClock( Cdialog *parent, const Crect &rect,
			colour hourColour, colour minutesColour, colour fingerColour,
			const Sfont &font, colour textColour);
	virtual ~ChandClock();

public:
	void    clearImage();
	virtual void onPaint( int touch);
	std::string get( size_t n);
	void addPoint( int x, int y);
	virtual bool onPaintingStart( const Cpoint &point);
	virtual bool onPaintingMove( const Cpoint &point);
	virtual bool onPaintingStop( const Cpoint &point);
	void setTime(int hours, int minutes);
	bool isChanged();
	void setResolution(int res);
	void setCenter( const Cpoint &center);
	int getDayMinutes() const;
    int getHours() const;
    int getMinutes() const;

private:
    int     m_hours;
    int     m_minutes;
	EclockState m_state; ///< What r we doing?
	int     m_startHour;
	int     m_startMinute;
	colour 	m_hourColour[3];
	colour  m_minutesColour[3];
	int 	m_stroke; ///< Index for our stroke.
	int 	m_distance; ///< Distance for current stroke.
	bool 	m_started; ///< Is the mouse pressed.
	Cpoint  m_lastPoint; ///< Last point.
	char 	m_value[4];
	colour	m_textColour; ///< Colour text.
	Ctext	m_textButton; ///< Text button to use.
	Cpoint	m_hourPoint;
	Cpoint	m_minutesPoint;
	int		h_center;
	int		v_center;
	int		m_resolution;
	int   	m_radius0;
	int   	m_radius1;
	int   	m_radius2;
	CfastArc	m_minutesArc;
	CfastArc	m_hoursArc;
	CfastArc	m_fingerArc;
	CfastArc	m_amPmArc;
	bool 	m_isAm;
};

typedef std::shared_ptr<ChandClock> ChandClockPtr;
