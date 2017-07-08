#include <stdio.h>
#include <string>
#include <sstream>
#include <sys/time.h>
#include "timestamp.h"
#include "text_splitter.h"

struct tm Ctimestamp::m_simulation;
bool Ctimestamp::m_use_simulation =false;

int operator <(const Ctimestamp &a, const Ctimestamp &b)
// Check 1 date earlier than the other !
{
	if (a.getYear() != b.getYear() )
		return (a.getYear() < b.getYear() );
	if (a.getMonth() != b.getMonth() )
		return (a.getMonth() < b.getMonth() );
	return (a.getDay() < b.getDay() );
}

int operator >=(const Ctimestamp &a, const Ctimestamp &b)
{
	if (a.getYear() != b.getYear() )
		return (a.getYear() < b.getYear() );
	if (a.getMonth() != b.getMonth() )
		return (a.getMonth() < b.getMonth() );
	return (a.getDay() >= b.getDay() );
}

int operator ==(const Ctimestamp &a, const Ctimestamp &b)
{
	return (  a.getYear()   == b.getYear()
		   && a.getMonth()  == b.getMonth()
		   && a.getHours()  == b.getHours()
		   && a.getDay()    == b.getDay()
		   && a.getMinutes()== b.getMinutes()
		   && a.getSeconds()== b.getSeconds()) ? true:false;
}

void Ctimestamp::startSimulation()
{
	m_use_simulation =true;
	m_simulation.tm_hour =16;
	m_simulation.tm_mday =10;
	m_simulation.tm_min =20;
	m_simulation.tm_mon =8;
	m_simulation.tm_sec =0;
	m_simulation.tm_wday =2;
	m_simulation.tm_yday =0;
	m_simulation.tm_year =2014;
}

void Ctimestamp::incrementSimulationTime()
{
	if ( !m_use_simulation)
	{
		startSimulation();
	}
	m_simulation.tm_sec+=5;
	if ( m_simulation.tm_sec<60)
	{
		return;
	}
	m_simulation.tm_sec =0;
	m_simulation.tm_min +=1;
	if ( m_simulation.tm_min<60)
	{
		return;
	}
	m_simulation.tm_min =0;
	if (++m_simulation.tm_hour<24)
	{
		return;
	}
	m_simulation.tm_hour =0;
	if (++m_simulation.tm_mday<=28)
	{
		return;
	}
	m_simulation.tm_mday =1;
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

/** Get maximum day for a year and month.
 *  @param year [in] Where we live now or -1900.
 *  @param month [in] 1..12
 */
int Ctimestamp::maxDay( int year, int month)
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
		yd +=maxDay( year, m);
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

int Ctimestamp::getWeek()
{
	int weekId =getWeek( m_time.tm_year, m_time.tm_mon, m_time.tm_mday);
	return weekId;
}

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

bool Ctimestamp::addMonth( int nr)
{
	bool lastDay =(maxDay( m_time.tm_yday, m_time.tm_mon)==m_time.tm_mday);
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
	int mday =maxDay( m_time.tm_year, m_time.tm_mon);
	if ( m_time.tm_mday >mday || lastDay==true)
	{
		m_time.tm_mday =mday;
	}
	return retVal;
}

bool Ctimestamp::addDay( int nr)
{
	int n=m_time.tm_mday+nr;
	for (;;)
	{
		int mx =maxDay( getYear(), getMonth());
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
		n+=maxDay( getYear(), getMonth());
	}
	m_time.tm_mday =n;
	return true;
}

bool Ctimestamp::addYear( int nr)
{
	if ( getYear()+nr<MINIMUM_YEAR)
	{
		return false;
	}
	m_time.tm_year +=nr;
	int mday =maxDay( m_time.tm_year, m_time.tm_mon);
	if ( m_time.tm_mday >mday)
	{
		m_time.tm_mday =mday;
	}
	return true;
}

/** Calculate which weekday is at a certain year and month. */
int Ctimestamp::firstWeekday( int year, int month)
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
			day +=maxDay( year, m);
		}
	}
	day =day+13+777777*7; ///???compensate???
    day =day%7;
    return day; /// 0=monday.
}

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

/*--Bart Houkes--20/02/97------------------------------------Status:NEW OK--*/
/*--Bart Houkes--05/01/98------------------------------------Status:orders--*/
std::string Ctimestamp::TimeString(int month, int day, int year, int hour, int minut, int seconds)
// Make correct string for time !!
{
  static char t[100];
//  sprintf(t, "%s[If:%d]\n[Height:1|Endif]  ", Datum(month, day, year).c_str(), SP342);
  strcat(t, Tijd(hour, minut, seconds).c_str());

  return t;
}

Ctimestamp::operator std::string(void) const
{
	char s[32];
	sprintf( s, "%02d:%02d:%02d", getHours(), getMinutes(), getSeconds());
	return s;
}

//void Ctimestamp::add( CGLUE_tinyXML &x, TiXmlElement *root, const char *name) const
//{
//	TiXmlElement *el =x.add( root, name);
//	x.add( el, "seconds", m_time.tm_sec);
//	x.add( el, "minutes", m_time.tm_min);
//	x.add( el, "hours", m_time.tm_hour);
//	x.add( el, "days", m_time.tm_mday);
//	x.add( el, "months", m_time.tm_mon);
//	x.add( el, "years", m_time.tm_year);
//	x.add( el, "week_day", m_time.tm_wday);
//	x.add( el, "year_day", m_time.tm_yday);
//}

//void Ctimestamp::load( CGLUE_tinyXML &x, TiXmlElement *root, const char *name)
//{
//	TiXmlElement *el =x.find( root,name).ToElement();
//	m_time.tm_sec =0;
//	m_time.tm_min =0;
//	m_time.tm_hour =0;
//	m_time.tm_mday =0;
//	m_time.tm_mon =0;
//	m_time.tm_year =0;
//	m_time.tm_wday =0;
//	m_time.tm_yday =0;
/////	x.find( el, "seconds", &m_time.tm_sec);
////	x.find( el, "minutes", &m_time.tm_min);
//	//x.find( el, "hours", &m_time.tm_hour);
////	x.find( el, "days", &m_time.tm_mday);
////	x.find( el, "months", &m_time.tm_mon);
//	x.find( el, "years", &m_time.tm_year);
//	if ( m_time.tm_year<200)
//	{
//		m_time.tm_year+=1900;
//	}
//	x.find( el, "week_day", &m_time.tm_wday);
//	x.find( el, "year_day", &m_time.tm_yday);
//}


/*--Bart Houkes--20/02/97------------------------------------Status:NEW OK--*/
/*--Bart Houkes--05/01/98------------------------------------Status:orders--*/
std::string Ctimestamp::Datum(int month, int day, int year)
{
	static char t[128];

	if (month < 0)
	{
		Ctimestamp d;

		year = d.getYear();
		month = d.getMonth();
		day = d.getDay();
	}
//	if (PrintLang == LANG_SIMPLIFIED || PrintLang ==LANG_TRADITIONAL)
//		sprintf(t, "%d-%d-%d", day, month, year);
//	else
//		sprintf(t, "[Width:1|If:%d|Font:0|Endif|Height:1]%d %s %d", SP342, day,
//				getTranslation( _JANUAR + month - 1, PrintLang).c_str(), year);
	return t;
}

/// @brief Get the date and time string for sql.
std::string Ctimestamp::getDateTime() const
{
	char t[64];
	sprintf( t, "%04d-%02d-%02d  %02d:%02d:%02d",
			 getYear(), getMonth(), getDay(), getHours(), getMinutes(), getSeconds());
	std::string tt(t);
	return tt;
}

/// @brief Get the time string (like sql).
std::string Ctimestamp::getTime() const
{
	char t[32];
	sprintf( t, "%02d:%02d:%02d",
			 getHours(), getMinutes(), getSeconds());
	std::string tt(t);
	return tt;
}

/// @brief Get the date for display.
std::string Ctimestamp::getDate() const
{
	char t[256];
	sprintf( t, "%02d-%02d-%04d",
			 getDay(), getMonth(), getYear());
	std::string tt(t);
	return tt;
}

Ctimestamp::Ctimestamp( const std::string &sqlValue)
{
	CtextSplitter cs( sqlValue, " ", " ");
	if ( cs.size()<2)
	{
		*this =Ctimestamp();
		return;
	}

	std::string _date=cs[0];
	std::string _time=cs[1];
	CtextSplitter cdate( _date, "-", ":");
	CtextSplitter ctime( _time, "-", ":");
	if ( cdate.size()<3 || ctime.size()<3)
	{
		*this =Ctimestamp();
		return;
	}

	m_time.tm_year =atoi( cdate[0].c_str());
	m_time.tm_mon  =atoi( cdate[1].c_str());
	m_time.tm_mday =atoi( cdate[2].c_str());
	m_time.tm_hour =atoi( ctime[0].c_str());
	m_time.tm_min  =atoi( ctime[1].c_str());
	m_time.tm_sec  =atoi( ctime[2].c_str());
}

Ctimestamp::Ctimestamp()
: m_rawtime(0)
{
	struct   timeval  tv;
	struct   timezone tz;
	gettimeofday(&tv, &tz);
	struct tm * timeinfo;
	if ( m_use_simulation)
	{
		m_msec =0;
		m_time =m_simulation;
	}
	else
	{
		timeinfo =localtime(&tv.tv_sec);
		m_msec =tv.tv_usec/1000;
		m_time =*timeinfo;
		m_time.tm_mon++; // Not nice to work with 0..11, so we add one.
	}

#if 0
	assert( Ctimestamp::getWeekDay( 2013, 1,1) ==1);
	assert( Ctimestamp::getWeekDay( 2013, 1,2) ==2);
	assert( Ctimestamp::getWeekDay( 2013, 1,6) ==6);
	assert( Ctimestamp::getWeekDay( 2013, 1,7) ==0);
	assert( Ctimestamp::getWeek( 2013, 1,1) ==0);
	assert( Ctimestamp::getWeek( 2013, 1,2) ==0);
	assert( Ctimestamp::getWeek( 2013, 1,6) ==0);
	assert( Ctimestamp::getWeek( 2013, 1,7) ==1);
	assert( Ctimestamp::getWeekDay( 2014, 1,1) ==2);
	assert( Ctimestamp::getWeekDay( 2014, 1,2) ==3);
	assert( Ctimestamp::getWeekDay( 2014, 1,5) ==6);
	assert( Ctimestamp::getWeekDay( 2014, 1,6) ==0);
	assert( Ctimestamp::getWeek( 2014, 1,1) ==0);
	assert( Ctimestamp::getWeek( 2014, 1,2) ==0);
	assert( Ctimestamp::getWeek( 2014, 1,5) ==0);
	assert( Ctimestamp::getWeek( 2014, 1,6) ==1);
#endif
}
