/*============================================================================*/
/**  @file      sdl_after_glow.cpp
 **  @ingroup   sdl2ui
 **  @brief		Keep a list of buttons that were pressed
 **
 **  Class to define which buttons have an after-glow of buttons pressed.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CafterGlowList
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
#include <sys/time.h>
#include "sdl_after_glow.h"

/// Release time for a button
#define MAXIMUM_RELEASE_TIME 200

/** @brief Constructor */
CafterGlowList::CafterGlowList()
: m_time_now(0)
, m_last_time(0)
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
	lock();
	try
	{
		(void)elapsed();
		int n=0;
		for ( n=0; n<(int)m_info.size();)
		{
			StouchInfo *ti =&m_info[n];
			if ( ti->pressed)
			{
				retVal =!ti->last_time;
				ti->last_time =m_time_now;
				// Check if the button is released
				if ( !pressed || !ti->object->inside( q))
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
	}
	catch(...)
	{
		retVal =false;
	}
	unlock();
	return retVal;
}

/** @brief We want to know the touch-factor for this button
 *  @param object [in] Which object we examine
 *  @param factor [out] Touch value for this one. 0=nothing, 1..99 =pressing, 100=pressed, 101..200 =releasing
 *  @return true when the button was found.
 */
bool CafterGlowList::getFactor( CdialogObjectPtr object, int *factor)
{
	*factor=0;
	bool retVal =false;
	lock();
	try
	{
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
	}
	catch(...)
	{
		retVal =false;
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
void CafterGlowList::addObject( CdialogObjectPtr object)
{
	lock();
	for ( int n=0; n<(int)m_info.size(); n++)
	{
		if (m_info[n].object ==object)
		{
		    unlock();
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
