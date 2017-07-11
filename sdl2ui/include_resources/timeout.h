/*============================================================================*/
/** @file       timeout.h
 *  @ingroup    drivers
 *  @brief		time elapsed routines.
 *
 *  Creation Date: Sat - 1 january 2011
 * 
 *  @author Bart Houkes
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
#include "my_thread.h"

/*------------- Necessary include files -------------------------------------*/

/*------------- External symbolic constants ---------------------------------*/

/*------------- External macros ---------------------------------------------*/

/*------------- Enum definitions --------------------------------------------*/

/*------------- Structure definitions ---------------------------------------*/

/*------------- External class declarations ---------------------------------*/

/*=============================================================================
  *              Ctimeout
  *==========================================================================*/
/**  Ctimeout
  *  Easy way to detect a timeout.
  */
/*===========================================================================*/
class Ctimeout : public CmyLock
{
public :
	Ctimeout(void);
	~Ctimeout(void);

public:
	void setTime( int timeout, int margin, int minTries);
	int running(void);
	bool expired(void);
	void setExpired(void);
	static unsigned long GetTickCount();
	void stop();
	int elapsed(void);

private:
	int m_startTime; ///< Start time value.
	int m_timeout; ///< Time value.
	bool m_detected; ///< Timeout detected.
	int m_margin; ///< Margin.
	int m_tries; ///< Number of checks. 
	int m_minRetries; ///< Minimum tries before timeout.
	int m_lastElapsed; ///< For debugging only.
};

extern void delay( double milisec);

/*=============================================================================
**      end of file
**===========================================================================*/


