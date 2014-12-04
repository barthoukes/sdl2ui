/*============================================================================*/
/**  @file      sdl_after_glow.cpp
 **  @ingroup   sdl
 **  @brief		Keep a list of buttons that were pressed
 **
 **  Class to define which buttons have an after-glow of buttons pressed.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CtouchList
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 29 nov 2014
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

#include <sys/time.h>
#include "sdl_after_glow.h"

/// Release time for a button
#define MAXIMUM_RELEASE_TIME 200

/** @brief Constructor */
CafterGlowList::CafterGlowList()
: m_time_now(0)
{
}

/** @brief Destructor */
CafterGlowList::~CafterGlowList()
{
	m_time_now=0;
	m_info.clear();
}

/** @brief Calculate how much time is elapsed between now and last-time
 *  @return Elapsed time in msec.
 */
unsigned int CafterGlowList::elapsed()
{
	unsigned int elapsed =0;
	struct timeval tv;
    if( gettimeofday(&tv, NULL) != 0)
            return 0;

    m_time_now =(unsigned int)((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
    elapsed =m_time_now-m_last_time;
    if (elapsed>1000)
    {
    	elapsed =0;
    }
    m_last_time =m_time_now;
    return elapsed;
}
/** @brief Regular update of the touch list, remove keys that were pressed long ago.
 *  @param pressed [in] Is anything pressed
 *  @param mouse [in] Last position pressed
 *  @return true when we need to repaint the screen
 */
bool CafterGlowList::update( bool pressed, const Cpoint &mouse)
{
	bool retVal =false;
	Cpoint q=mouse/8;
	(void)elapsed();
	lock();
	int n=0;
	for ( n=0; n<(int)m_info.size();)
	{
		StouchInfo *ti =&m_info[n];
		if ( ti->pressed)
		{
			retVal =!ti->last_time;
			ti->last_time =m_time_now;
			// Check if the button is released
			if ( !pressed || !ti->object->m_rect.inside( q))
			{
				ti->pressed =false;
			    ti->startTime =m_time_now;
			    retVal =true;
			}
			else
			{
				// Do nothing
			}
			n++;
		}
		else
		{
			// See if we need to remove this from the list
			if ( m_time_now-ti->startTime>=MAXIMUM_RELEASE_TIME)
			{
				m_last_time =m_time_now;
				m_info.erase( m_info.begin()+n);
			    retVal =true;
			}
			else
			{
				if ( m_time_now-ti->last_time>33) // 33 Hz update
				{
					m_last_time =m_time_now;
					retVal =true;
				}
				n++;
			}
		}
	}
	unlock();
	return retVal;
}

/** @brief We want to know the touch-factor for this button
 *  @param object [in] Which object we examine
 *  @param factor [out] Touch value for this one. 0=nothing, 1..99 =pressing, 100=pressed, 101..200 =releasing
 *  @return true when the button was found.
 */
bool CafterGlowList::getFactor( CdialogObject *object, int *factor)
{
	*factor=0;
    bool retVal =false;
	lock();
	for ( int n=0; n<(int)m_info.size(); n++)
	{
		StouchInfo *ti=&m_info[n];
		if ( ti->object ==object)
		{
			retVal =true;
			if ( ti->pressed)
			{
				*factor =100;
				ti->last_time =m_last_time;
			}
			else
			{
				*factor =100-((m_time_now-ti->startTime)*100)/MAXIMUM_RELEASE_TIME;
				if ( *factor>100)
				{
					*factor =100;
				}
				if ( *factor<0)
				{
					*factor =0;
					m_info.erase( m_info.begin()+n);
				}
			}
			break;
		}
	}
	unlock();
	return retVal;
}

/** @brief Forget all the buttons that are pressed or were pressed */
void CafterGlowList::clear()
{
	m_info.clear();
}

/** @brief Tell how many buttons are pressed now
 *  @return quantity of buttons
 */
int CafterGlowList::size()
{
	return (int)m_info.size();
}

/** @brief A new button is pressed, let is glow some time after we release it.
 *  @param object [in] The new button pressed
 */
void CafterGlowList::addObject( CdialogObject *object)
{
	lock();
	for ( int n=0; n<(int)m_info.size(); n++)
	{
		if (m_info[n].object ==object)
		{
			return;
		}
	}
	StouchInfo ti;
	ti.object =object;
	ti.pressed =true;
	ti.last_time =0; //m_time_now; // 100%
	ti.startTime =m_last_time;
	m_info.push_back( ti);
	unlock();
}
