/*
 * timestamp.h
 *
 *  Created on: 12 jan 2011
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

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <string>
//#include <sys/time.h>
#include "glue_tinyxml.h"

#define MINIMUM_YEAR 2010

/// @brief Timestamp to add in classes.
class Ctimestamp
{
public:
	Ctimestamp();
	Ctimestamp( const std::string &sqlValue);
	~Ctimestamp() {}

private:
	struct tm   m_time;
	int			m_msec;
	time_t 		m_rawtime;
	static struct tm m_simulation;
	static bool m_use_simulation;

public:
	static void startSimulation();
	static void incrementSimulationTime();
	int compareDate(const Ctimestamp &a) const;

	int getHours() const { return m_time.tm_hour; }
	int getMinutes() const { return m_time.tm_min; }
	int getSeconds() const { return m_time.tm_sec; }
	int getDay() const { return m_time.tm_mday; }
	int getMonth() const { return m_time.tm_mon; }
	int getQuarter() const { return static_cast<int>((2+getMonth())/3); }
	int getMilliseconds() const { return m_msec; }
	int getWeek();
	static int getWeekDay( int year, int month, int day);
	std::string getDate() const;
	void decreaseHours( int x);
	std::string getDateTime() const;
	static int maxDay( int year, int month);
	static int firstWeekday( int year, int month);
	static int getYearDay( int year, int month, int day);
	static int getWeek( int year, int month, int day);
	bool addMonth( int nr);
	bool addYear( int nr);
	bool addDay( int nr);
	bool addHours( int nr);
	bool addMinutes( int nr);
	std::string getDisplayDate() const;
	long long getTicks() { return  getSeconds()+
			                       getMinutes()*60+
			                       getHours()*3600+
			                       getDay()*24*60*60L+
			                       getMonth()*24L*60L*60L*31L+
			                       (getYear()-1900)*24L*60L*60L*31L*13L;
	}
	int getYear() const
	{
		if ( m_time.tm_year<200 && m_time.tm_year>=MINIMUM_YEAR-1900 )
		{
			return m_time.tm_year+1900;
		}
		return m_time.tm_year;
	}
	void setDay(int day) { m_time.tm_mday =day; }
	void setMonth(int month) { m_time.tm_mon =month; }
	void setYear(int year) { m_time.tm_year =year; }
	void setHour(int hour) { m_time.tm_hour =hour; }
	void setMinute( int minute) { m_time.tm_min =minute; }
	void setSecond( int second) { m_time.tm_sec =second; }
	static std::string Tijd(int hour=-1, int minut=-1, int seconds=0);
	void add( CGLUE_tinyXML &x, TiXmlElement *root, const char *name) const;
	void load( CGLUE_tinyXML &x, TiXmlElement *root, const char *name);
	/* Function Prototypes */
	static std::string TimeString( int     month=-1,
	                         int           day=-1,
	                         int           year=-1,
	                         int           hour=-1,
	                         int           minut=-1,
	                         int           seconds=-1);
	operator std::string(void) const;
	std::string getTime() const;
	std::string getShortTime() const;
	static std::string Datum(int month=-1, int day=-1, int year=-1);

	  //int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
	  //int tm_min;			/* Minutes.	[0-59] */
	  //int tm_hour;			/* Hours.	[0-23] */
	  //int tm_mday;			/* Day.		[1-31] */
	  //int tm_mon;			/* Month.	[0-11] */
	  //int tm_year;			/* Year	- 1900.  */
	  //int tm_wday;			/* Day of week.	[0-6] */
	  //int tm_yday;			/* Days in year.[0-365]	*/
	  //int tm_isdst;
};

extern int operator <(const Ctimestamp &a, const Ctimestamp &b);
extern int operator >(const Ctimestamp &a, const Ctimestamp &b);
extern int operator >=(const Ctimestamp &a, const Ctimestamp &b);
extern int operator ==(const Ctimestamp &a, const Ctimestamp &b);

