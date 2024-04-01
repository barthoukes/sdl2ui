/*============================================================================*/
/** @file       timeout.hpp
 *  @ingroup    drivers
 *  @brief		time elapsed routines.
 *
 *  Creation Date: Sat - 1 january 2011
 * 
 *  @author Bart Houkes
 */
/*------------------------------------------------------------------------------
 *  Copyright (c) Bart Houkes, 2011
 *
 *  Copyright notice:
 *  This software is property of Bart Houkes software (Eindhoven).
 *  Unauthorized duplication and disclosure to third parties is forbidden.
 *============================================================================*/

#pragma once

#include "my_thread.hpp"

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

extern void delay( int millisec);

/*=============================================================================
**      end of file
**===========================================================================*/
