/*============================================================================*/
/**  @file      sdl_after_glow.h
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

#pragma once

#include <vector>
#include "sdl_dialog_object.h"
#include "my_thread.h"

typedef struct
{
	CdialogObject 	*object;	// What object to use
	bool		  	pressed;	// true=press, false=release
	int				last_time;	// time of invalidate
	unsigned int	startTime;  // Press or release start
} StouchInfo;

class CafterGlowList : public CmyLock
{
public:
	CafterGlowList();
	virtual ~CafterGlowList();

public:
	unsigned int elapsed();

	bool update( bool pressed, const Cpoint &mouse);
	bool getFactor( CdialogObject *object, int *factor);
	void clear();
	int size();
	void addObject( CdialogObject *object);

private:
	std::vector<StouchInfo> m_info;
	unsigned int m_time_now; ///< Current time
	unsigned int m_last_time; ///< Previous call
};
