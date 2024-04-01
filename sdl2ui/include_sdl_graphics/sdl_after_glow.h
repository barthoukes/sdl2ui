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
#include <vector>
#include "sdl_dialog_object.h"
#include "my_thread.hpp"

typedef struct
{
	CdialogObjectPtr object;	 // What object to use
	bool		  	 pressed;	 // true=press, false=release
	int				 last_time;	 // time of invalidate
	unsigned int	 startTime;  // Press or release start
} StouchInfo;

class CafterGlowList : public CmyLock
{
public:
	CafterGlowList();
	virtual ~CafterGlowList();

public:
	unsigned int elapsed();

	bool update( bool pressed, const Cpoint &mouse);
	bool getFactor( CdialogObjectPtr object, int *factor);
	void clear();
	int size();
	void addObject( CdialogObjectPtr object);

private:
	std::vector<StouchInfo> m_info;
	unsigned int m_time_now; ///< Current time
	unsigned int m_last_time; ///< Previous call
};
