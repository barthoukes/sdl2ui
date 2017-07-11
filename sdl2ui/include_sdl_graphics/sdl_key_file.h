/*============================================================================*/
/**  @file      sdl_key_file.h
 **  @ingroup   user_interface
 **  @brief		Record key buttons.
 **
 **  Record key button info.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CkeyFile
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
#include <sdl_keybutton.h>
#include <stdio.h>
#ifdef USE_SDL2
#include "SDL_events.h"
#else
#include "SDL_events.h"
#endif

class Cevent;

/// @brief  Create and display buttons.
class CkeyFile
{
	FILE *m_file;

public:
	CkeyFile();
	~CkeyFile();

	void init();
	void close();
	void addEvent( const std::string &event, int value1, int value2, const std::string &comment);
	void onEvent(const SDL_Event *event);
	void onEvent(const Cevent &event);

private:
	bool m_mouseDown; ///< If no mouse pressed, then forget mouse move events.
};

