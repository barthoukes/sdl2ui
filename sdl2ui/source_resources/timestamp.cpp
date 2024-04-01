#include <stdio.h>
#include <sys/time.h>
#include "timestamp.hpp"
#include <string.h>
#include <string>
#include <sstream>
#include "timestamp.hpp"
#include "text_splitter.hpp"

CmyLock Ctimestamp::m_lock;
struct tm Ctimestamp::m_simulation;
bool Ctimestamp::m_use_simulation(false);

/*----------------------------------------------------------------------------*/
int operator <(const Ctimestamp &a, const Ctimestamp &b)
// Check 1 date earlier than the other !
{
	if (a.getYear() != b.getYear() )
		return (a.getYear() < b.getYear() );
	if (a.getMonth() != b.getMonth() )
		return (a.getMonth() < b.getMonth() );
	return (a.getDay() < b.getDay() );
}

/*----------------------------------------------------------------------------*/
int operator >=(const Ctimestamp &a, const Ctimestamp &b)
{
	if (a.getYear() != b.getYear() )
		return (a.getYear() < b.getYear() );
	if (a.getMonth() != b.getMonth() )
		return (a.getMonth() < b.getMonth() );
	return (a.getDay() >= b.getDay() );
}

/*----------------------------------------------------------------------------*/
int operator >(const Ctimestamp &a, const Ctimestamp &b)
{
	if (a.getYear() != b.getYear() )
		return (a.getYear() > b.getYear() );
	if (a.getMonth() != b.getMonth() )
		return (a.getMonth() > b.getMonth() );
	return (a.getDay() > b.getDay() );
}

/*----------------------------------------------------------------------------*/
int operator ==(const Ctimestamp &a, const Ctimestamp &b)
{
	return (  a.getYear()   == b.getYear()
		   && a.getMonth()  == b.getMonth()
		   && a.getHours()  == b.getHours()
		   && a.getDay()    == b.getDay()
		   && a.getMinutes()== b.getMinutes()
		   && a.getSeconds()== b.getSeconds()) ? true:false;
}

/*----------------------------------------------------------------------------*/
void Ctimestamp::startSimulation()
{
	m_lock.lock();
	m_use_simulation =true;
	m_simulation.tm_hour =16;
	m_simulation.tm_mday =10;
	m_simulation.tm_min =20;
	m_simulation.tm_mon =8;
	m_simulation.tm_sec =0;
	m_simulation.tm_wday =2;
	m_simulation.tm_yday =0;
	m_simulation.tm_year =2014;
	m_lock.unlock();
}

/*----------------------------------------------------------------------------*/
int Ctimestamp::compareSeconds(Ctimestamp &b)
{
    long int time2 = b.time2seconds();
    long int time1 = time2seconds();
    return (time1-time2);
    // Return > 0 :
}

long int Ctimestamp::time2seconds()
{
    return mktime(&m_time);
}

/*----------------------------------------------------------------------------*/
bool Ctimestamp::isAsap(bool config)
{
    Ctimestamp now;
    bool asap = ( compareDate(now) <= 120 && config);
    return asap;
}

/*----------------------------------------------------------------------------*/
/// Return 1 if I am after a, 0 for equal and -1 for smaller
int Ctimestamp::compareDate(const Ctimestamp &a) const
{
	if (a.getYear() >getYear())
	{
		return 1;
	}
	if (a.getYear() <getYear())
	{
		return -1;
	}
	if (a.getMonth() >getMonth())
	{
		return 1;
	}
	if (a.getMonth() <getMonth())
	{
		return -1;
	}
	if (a.getDay() >getDay())
	{
		return 1;
	}
	if (a.getDay() <getDay())
	{
		return -1;
	}
	return 0;
}

/*----------------------------------------------------------------------------*/
/** Round time to nearest 5, 10, 20 minutes etc. */
void Ctimestamp::roundUp(int minutes)
{
	if (minutes<=1)
	{
		return;
	}
	m_time.tm_sec =0;
	m_time.tm_min += (minutes-1);
	if ( m_time.tm_min >=60)
	{
		m_time.tm_min =0;
		addHours(1);
	}
	else
	{
		m_time.tm_min -=(m_time.tm_min%minutes);
	}
}

/*----------------------------------------------------------------------------*/
void Ctimestamp::incrementSimulationTime()
{
	m_lock.lock();
	if ( !m_use_simulation)
	{
		startSimulation();
	}
	m_simulation.tm_sec+=5;
	if ( m_simulation.tm_sec >= 60)
	{
        m_simulation.tm_sec =0;
        m_simulation.tm_min +=1;
        if ( m_simulation.tm_min >= 60)
        {
            m_simulation.tm_min =0;
            if (++m_simulation.tm_hour >= 24)
            {
                m_simulation.tm_hour =0;
                if (++m_simulation.tm_mday >= 28)
                {
                    m_simulation.tm_mday =1;
                }
            }
        }
	}
	m_lock.unlock();
}

/*--Bart Houkes--20/02/97------------------------------------Status:NEW OK--*/
std::string Ctimestamp::Tijd(int hour, int minut, int seconds)
{
	static char u[15];
	if (hour < 0)
	{
		Ctimestamp d;
		hour = d.getHours();
		minut = d.getMinutes();
		seconds = d.getSeconds();
	}
	sprintf(u, "%d:%02d:%02d", hour, minut, seconds);
	return u;
}

/*----------------------------------------------------------------------------*/
/** Get maximum day for a year and month.
 *  @param year [in] Where we live now or -1900.
 *  @param month [in] 1..12
 */
int Ctimestamp::getMaxDay( int year, int month)
{
	switch ( month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12: return 31;
	case 2: if ( (year%100)==0) return 30;
	        else if ((year&3)==0) return 29;
	        else return 28;
	default: return 30;
	}
}

/*----------------------------------------------------------------------------*/
/** Get year day.
 *  @param year [in] What year we have.
 *  @param month [in] Month 1..12.
 *  @param day [in] Day of the month 1..31.
 */
int Ctimestamp::getYearDay( int year, int month, int day)
{
	int yd=0;
	for ( int m=1; m<month; m++)
	{
		yd +=getMaxDay( year, m);
	}
	yd+=day-1;

#if 0
	tm t;
	memset( &t,0,sizeof(t));
	t.tm_hour=1;
	t.tm_mon =month-1;
	t.tm_year =(year>=1900) ? year-1900:year;
	t.tm_mday =day;
	mktime( &t);
	assert( t.tm_yday ==yd);
#endif
	return yd;
}

/*----------------------------------------------------------------------------*/
std::string Ctimestamp::getSysTime()
{
    char line[64];
    getRealTime();
    (void) sprintf(line, "%04d/%02d/%02d %02d:%02d:%02d.%03d: ",
            getYear(), getMonth(), getDay(), getHours(),
            getMinutes(), getSeconds(), getMilliseconds());
    return std::string(line);
}

/*----------------------------------------------------------------------------*/
int Ctimestamp::getWeek()
{
	int weekId =getWeek( m_time.tm_year, m_time.tm_mon, m_time.tm_mday);
	return weekId;
}

/*----------------------------------------------------------------------------*/
/** Calculate days since sunday.
 *  @param year [in] What year (maybe -1900)
 *  @param month [in] Month 1..12.
 *  @param day [in] Day 1..31.
 *  @return 0=monday.
 */
int Ctimestamp::getWeekDay( int year, int month, int day)
{
	tm t;
	memset( &t,0,sizeof(t));
	t.tm_hour=1;
	t.tm_mon =month-1;
	t.tm_year =(year >=1900) ? year-1900:year;
	t.tm_mday =day;
	mktime( &t);
	return (t.tm_wday+6)%7;
}

/*----------------------------------------------------------------------------*/
int Ctimestamp::getWeek( int year, int month, int day)
{
	//int fwd =firstWeekday( year, 1);
	int yd =getYearDay( year, month, day);
	int wd =getWeekDay( year, 1, 1); // (fwd+yd)%7;
	int week =(yd+wd)/7;
	//int week =(yd + 7 - (wd ? (wd - 1) : 6)) / 7;
	// yd    wd    week
	// 0      0     0
	// 6      6     0
	// 1      0


	return week;
}

/*----------------------------------------------------------------------------*/
int Ctimestamp::getWeekday()
{
    return getWeekDay(getYear(), getMonth(), getDay());
}

/*----------------------------------------------------------------------------*/
bool Ctimestamp::addMonth( int nr)
{
	bool lastDay =(getMaxDay( m_time.tm_yday, m_time.tm_mon)==m_time.tm_mday);
	bool retVal =true;
	m_time.tm_mon +=nr;
	while (m_time.tm_mon>12)
	{
		m_time.tm_mon -=12;
		m_time.tm_year++;
	}
	while (m_time.tm_mon<=0)
	{
		if ( m_time.tm_year ==MINIMUM_YEAR || m_time.tm_year ==MINIMUM_YEAR-1900)
		{
			retVal =false;
			break;
		}
		m_time.tm_mon+=12;
		m_time.tm_year--;
	}
	int mday =getMaxDay( m_time.tm_year, m_time.tm_mon);
	if ( m_time.tm_mday >mday || lastDay==true)
	{
		m_time.tm_mday =mday;
	}
	return retVal;
}

/*----------------------------------------------------------------------------*/
bool Ctimestamp::addDay( int nr)
{
	int n=m_time.tm_mday+nr;
	for (;;)
	{
		int mx =getMaxDay( getYear(), getMonth());
		if ( n >mx)
		{
			addMonth(1);
			n -=mx;
		}
		else
		{
			break;
		}
	}
	while ( n<1)
	{
		if ( addMonth(-1)==false)
		{
			return false;
		}
		n +=getMaxDay( getYear(), getMonth());
	}
	m_time.tm_mday =n;
	return true;
}

/*----------------------------------------------------------------------------*/
bool Ctimestamp::addMinutes( int nr)
{
	int n=m_time.tm_min+=nr;
	while (n>=60)
	{
		n-=60;
		addHours(1);
	}
	while (n<0)
	{
	    n+=60;
	    addHours(-1);
	}
	m_time.tm_min=n;
	return true;
}

/*----------------------------------------------------------------------------*/
bool Ctimestamp::addHours( int nr)
{
	int n=m_time.tm_hour+=nr;
	while (n>=24)
	{
		n-=24;
		addDay(1);
	}
	while (n<0)
	{
	    n+=24;
	    addDay(-1);
	}
	m_time.tm_hour=n;
	return true;
}

/*----------------------------------------------------------------------------*/
bool Ctimestamp::addYear( int nr)
{
	if ( getYear()+nr<MINIMUM_YEAR)
	{
		return false;
	}
	m_time.tm_year +=nr;
	int mday =getMaxDay( m_time.tm_year, m_time.tm_mon);
	if ( m_time.tm_mday >mday)
	{
		m_time.tm_mday =mday;
	}
	return true;
}

/*----------------------------------------------------------------------------*/
/** Calculate which weekday is at a certain year and month. */
int Ctimestamp::getFirstWeekday( int year, int month)
{
	if ( year>1900) year-=1900;
	int day=year*365+(int)((year-1)/4)+(int)((year-1)/100);
	for ( int m=1; m<12; m++)
	{
		if ( m==month)
		{
			break;
		}
		else
		{
			day +=getMaxDay( year, m);
		}
	}
	day =day+13+777777*7; ///???compensate???
    day =day%7;
    return day; /// 0=monday.
}

/*----------------------------------------------------------------------------*/
// @brief Decrease hours.
void Ctimestamp::decreaseHours( int x)
{
	if ( m_time.tm_hour>=x)
	{
		m_time.tm_hour-=x;
		return;
	}
	m_time.tm_hour =m_time.tm_hour+24-x;
	if ( m_time.tm_mday>1)
	{
		m_time.tm_mday--;
		return;
	}
	m_time.tm_mon--;
	switch ( m_time.tm_mon)
	{
		case 0: m_time.tm_mday =31; m_time.tm_mon=12; m_time.tm_year--; break;
		case 4:
		case 9:
		case 11:
		case 6: m_time.tm_mday =30; break;
		case 3:
		case 10:
		case 5:
		case 7:
		case 8:
		case 1: m_time.tm_mday =31; break;
		case 2: m_time.tm_mday =( (m_time.tm_year&3)==0) ? 29:28; break;
		default:
			break;
	}
	return;
}

/*----------------------------------------------------------------------------*/
Ctimestamp::operator std::string(void) const
{
	char s[32];
	sprintf( s, "%02d:%02d:%02d", getHours(), getMinutes(), getSeconds());
	return s;
}

/*----------------------------------------------------------------------------*/
/// @brief Get the date and time string for sql.
std::string Ctimestamp::getDateTime() const
{
	char t[64];
	int year =getYear();
	if ( year<1980)
	{
		year =1980;
	}
	sprintf( t, "%04d-%02d-%02d  %02d:%02d:%02d",
			 year, getMonth(), getDay(), getHours(), getMinutes(), getSeconds());
	std::string tt(t);
	return tt;
}

/*----------------------------------------------------------------------------*/
/// @brief Get the time string (like sql).
std::string Ctimestamp::getTime() const
{
	char t[32];
	sprintf( t, "%02d:%02d:%02d",
			 getHours(), getMinutes(), getSeconds());
	std::string tt(t);
	return tt;
}

/*----------------------------------------------------------------------------*/
bool Ctimestamp::isToday() const
{
	Ctimestamp today;
	return (today.getYear() ==getYear() && today.getMonth() ==getMonth() && today.getDay() ==getDay());
}

/*----------------------------------------------------------------------------*/
/// @brief Get the time string (like sql).
std::string Ctimestamp::getSimpleTime() const
{
	char t[32];
	sprintf( t, "%02d:%02d",
			 getHours(), getMinutes());
	std::string tt(t);
	return tt;
}

/*----------------------------------------------------------------------------*/
bool Ctimestamp::timeInRange(const Ctimestamp &startTime, const Ctimestamp &endTime)
{
    int h=getHours(), m=getMinutes();
    int sh=startTime.getHours(), sm=startTime.getMinutes();
    int eh=endTime.getHours(), em=endTime.getMinutes();
    if (h<sh || (h==sh && m<sm) || h>eh || (h==eh && m>=em))
    {
        return false;
    }
    return true;
}

/*----------------------------------------------------------------------------*/
/// @brief Get the time string (like sql).
std::string Ctimestamp::getShortTime() const
{
	char t[32];
	sprintf( t, "%02d:%02d", getHours(), getMinutes());
	std::string tt(t);
	return tt;
}

/*----------------------------------------------------------------------------*/
/// @brief Get the date for display.
std::string Ctimestamp::getDate() const
{
	char t[256];
	sprintf( t, "%02d-%02d-%04d",
			 getDay(), getMonth(), getYear());
	std::string tt(t);
	return tt;
}

/*----------------------------------------------------------------------------*/
Ctimestamp::Ctimestamp( const std::string &sqlValue)
{
	CtextSplitter cs( sqlValue, " ", " ", false);
	int csize = (int)cs.size();
	if ( csize<2)
	{
		*this =Ctimestamp();
		CtextSplitter xtime( sqlValue, ":", ";", false);
		int size = xtime.size();
		if (size ==2 || size ==3)
		{
			m_time.tm_hour =atoi( xtime[0].c_str());
			m_time.tm_min  =atoi( xtime[1].c_str());
			m_time.tm_sec  =0;
		}
		return;
	}

	std::string _date=cs[0];
	std::string _time=cs[csize-1];
	CtextSplitter cdate( _date, "-", ":", false);
	CtextSplitter ctime( _time, "-", ":", false);
	if ( cdate.size()<3 || ctime.size()<3)
	{
		*this =Ctimestamp();
		//Log.error( "Timestamp issue date=%s, time=%s (%d,%d) !!", _date.c_str(), _time.c_str(), cdate.size(), ctime.size());
		return;
	}

	m_time.tm_year =atoi( cdate[0].c_str());
	m_time.tm_mon  =atoi( cdate[1].c_str());
	m_time.tm_mday =atoi( cdate[2].c_str());
	m_time.tm_hour =atoi( ctime[0].c_str());
	m_time.tm_min  =atoi( ctime[1].c_str());
	m_time.tm_sec  =atoi( ctime[2].c_str());
}

/*----------------------------------------------------------------------------*/
Ctimestamp::Ctimestamp()
: m_rawtime(0)
{
	m_lock.lock();
	if ( m_use_simulation)
	{
		m_msec =0;
		m_time =m_simulation;
	}
	else
	{
		getRealTime();
	}
	m_lock.unlock();
}

/*----------------------------------------------------------------------------*/
void Ctimestamp::getRealTime()
{
	struct   timeval  tv;
	struct   timezone tz;
	gettimeofday(&tv, &tz);

	struct tm * timeinfo;
	timeinfo =localtime(&tv.tv_sec);
	m_msec =tv.tv_usec/1000;
	m_time =*timeinfo;
	if ( m_time.tm_year<200)
	{
		m_time.tm_year +=1900;
	}
	m_time.tm_mon++; // Not nice to work with 0..11, so we add one.
}

/*----------------------------------------------------------------------------*/
Ctimestamp::~Ctimestamp()
{}
