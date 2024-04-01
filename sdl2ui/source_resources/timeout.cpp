/*============================================================================*/
/**  @file		timeout.cpp
 *   @ingroup   Source files
 *   @brief     Timeout
 *
 *   Handle timers.
 *
 *   @author    mensfort
 *============================================================================*/
/*------------- Standard includes --------------------------------------------*/
#include <sys/time.h>
#include <assert.h>
#include <unistd.h>

#include "timeout.hpp"

/*------------- Module options / compiler switches ---------------------------*/

/*------------- Includes -----------------------------------------------------*/
#include "pthread.h"

/*------------- Local symbolic constants -------------------------------------*/

/*------------- Local macros -------------------------------------------------*/

/*------------- Local enum definitions ---------------------------------------*/
                                                             
/*------------- Local structure definitions ----------------------------------*/

/*------------- Local functions prototypes -----------------------------------*/

/*------------- Global variables ---------------------------------------------*/
pthread_mutex_t g_mutex =PTHREAD_MUTEX_INITIALIZER;

/*------------- Exported classes ---------------------------------------------*/

/*==============================================================================
**              Ctimeout
**============================================================================*/

/*------------- Exported functions ------------------------------------------*/

unsigned long Ctimeout::GetTickCount()
{
    struct timeval tv;
    if(gettimeofday(&tv, NULL) != 0)
            return 0;

    return (unsigned long)((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/*==============================================================================
**              Ctimeout::setTime
**============================================================================*/
///
///  @brief		Set timeout.
///
///  @param		timeout [in] Maximum time to elapse.
///  @param     margin [in] Maximum time to be expired to detect breakpoints.
///  @param     minTries [in] Minimum tries before really timeout.
///
///  @post      Class created.
///               
///  @remarks   DEBUGGED
///                                 
/*============================================================================*/
void Ctimeout::setTime( int timeout, int margin, int minTries)	
{
	lock();
	m_startTime  =GetTickCount();
	m_timeout    =timeout;
	m_margin     =margin;
	m_minRetries =minTries;
	m_detected   =false;
	m_tries      =0;
	m_lastElapsed =0;
	unlock();
}

/*==============================================================================
**              Ctimeout::stop
**============================================================================*/
///
///  @brief		Stop timer.
///
///  @post      Timer stopped.
///
///  @remarks   DEBUGGED
///
/*============================================================================*/
void Ctimeout::stop()
{
	lock();
	m_startTime  =0;
	m_timeout    =-10000;
	m_margin     =10;
	m_minRetries =10000;
	m_detected   =false;
	m_tries      =0;
	unlock();
}

int Ctimeout::elapsed()
{
	lock();
	m_lastElapsed = GetTickCount()-m_startTime;
	unlock();
	return m_lastElapsed;
}

void Ctimeout::setExpired()
{
	m_timeout =-1;
}

/*==============================================================================
**              Ctimeout::expired
**============================================================================*/
///
///  @brief		Check if timer still valid.
///
///  @post      Nothing changed.
///               
///  @return    TRUE when timer has expired.
///
///  @remarks   DEBUGGED
///                                 
/*============================================================================*/
bool Ctimeout::expired(void)
{
	lock();
	bool retVal =false;
	
	if ( m_detected )
	{
		retVal =true;
	}
	else if ( m_timeout >=0)
	{
		m_lastElapsed =GetTickCount()-m_startTime;

		if (m_lastElapsed >=m_timeout)
		{
			if ( ++m_tries >m_minRetries)
			{
				if (  m_margin >0
				   && m_lastElapsed >=m_timeout +m_margin
				   && m_minRetries)
				{
					m_startTime =GetTickCount();
					m_tries =0;
				}
				else
				{
					retVal =true;
					m_detected =true;
				}
			}
		}
	}
	else
	{
		retVal =true;
	}
	unlock();
	return retVal;
}


/*------------- Local functions ---------------------------------------------*/

/*==============================================================================
**              Ctimeout::~Ctimeout
**============================================================================*/
///
/// @brief Destructor for register.
///
/// @post       Class removed.
///               
/// @remarks    DEBUGGED
///                                 
/*============================================================================*/
Ctimeout::~Ctimeout(void)
{
}

/*==============================================================================
**              Ctimeout::Ctimeout
**============================================================================*/
///
/// @brief Constructor for timeout.
///
/// @post       Class created.
///               
/// @remarks    DEBUGGED
///                                 
/*============================================================================*/
Ctimeout::Ctimeout(void)
: m_timeout(1000000)
, m_detected(false)
, m_margin(0)
, m_tries(100000)
, m_minRetries(10000)
{
	m_startTime =GetTickCount();
}

/*==============================================================================
**              Ctimeout::running
**============================================================================*/
///
/// @brief		Find out how long the timer is running.
///
/// @return		Time from setTime until now.
/// 
/// @post       Time calculated.
///               
/// @remarks    DEBUGGED
///                                 
/*============================================================================*/
int Ctimeout::running(void)
{
	return GetTickCount()-m_startTime;
}


/*----------------------------------------------------------------------------*/
void delay(int millisec)
{
#if 0
	if ( millisec<0.000001)
	{
		return;
	}
	if (millisec>=1000)
	{
	    sleep((int)(millisec/1000));
	}
	else
	{
	    usleep(millisec*1000L);
	}
#else
	std::this_thread::sleep_for(std::chrono::milliseconds(millisec));
#endif
}

/*======-----==================================================================
**      end of file
**===========-----===========================================================*/







