/*
 * timestamp.h
 *
 *  Created on: 12 jan 2011
 *      Author: mensfort
 */

#pragma once

#include <atomic>
#include <string>
#include "my_thread.hpp"
#include "itimestamp.hpp"

#define MINIMUM_YEAR 2010

/// @brief Timestamp to add in classes.
class Ctimestamp : public Itimestamp
{
public:
	Ctimestamp();
	Ctimestamp( const std::string &sqlValue);
	~Ctimestamp();

protected:
	struct tm   m_time;

private:
	int			   m_msec;
	time_t 	       m_rawtime;
	static struct  tm m_simulation;
	static bool    m_use_simulation;
	static CmyLock m_lock;

public:
	static void startSimulation();
	static void incrementSimulationTime();
	int compareDate(const Ctimestamp &a) const;
    int compareSeconds(Ctimestamp &b);
    bool isAsap(bool config);
    void getRealTime();

	int getHours() const { return m_time.tm_hour; }
	int getMinutes() const { return m_time.tm_min; }
	int getSeconds() const { return m_time.tm_sec; }
	int getDay() const { return m_time.tm_mday; }
	int getMonth() const { return m_time.tm_mon; }
	int getQuarter() const { return static_cast<int>((2+getMonth())/3); }
	int getMilliseconds() const { return m_msec; }
	int getWeek();
	std::string getSysTime();
	int getDayMinutes() { return m_time.tm_hour*60+m_time.tm_min; }
	static int getWeekDay( int year, int month, int day);
	std::string getDate() const;
	void decreaseHours( int x);
	std::string getDateTime() const;
	static int getMaxDay( int year, int month);
	static int getFirstWeekday( int year, int month);
	static int getYearDay( int year, int month, int day);
	static int getWeek( int year, int month, int day);
	int getWeekday();
	long int time2seconds();
	bool addMonth( int nr);
	bool addYear( int nr);
	bool addDay( int nr);
	bool addHours( int nr);
	bool addMinutes( int nr);
	void roundUp(int minutes);
	bool isToday() const;
	std::string getSimpleTime() const;
	std::string getDisplayDate() const;
	long long getTicks() { return  getSeconds()+
			                       getMinutes()*60+
			                       getHours()*3600+
			                       getDay()*24*60*60L+
			                       getMonth()*24L*60L*60L*31L+
			                       (getYear()-1900)*24L*60L*60L*31L*13L;
	}
	bool timeInRange(const Ctimestamp &startTime, const Ctimestamp &endTime);
    std::string Datum(int month=-1, int day=-1, int year=-1) const;
	int getYear() const
	{
		if ( m_time.tm_year<2000)
		{
			return 0;
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
	//void add( CGLUE_tinyXML &x, TiXmlElement *root, const char *name) const;
	//void load( CGLUE_tinyXML &x, TiXmlElement *root, const char *name);

	/* Function Prototypes */
	operator std::string(void) const;
	std::string getTime() const;
	std::string getShortTime() const;

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

