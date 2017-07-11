/*============================================================================*/
/**  @file       language.h
 **  @ingroup    demo
 **  @brief		 all language enumerators
 **
 **  Demo for sdl2ui
 **
 **  @author     mensfort
 **
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2011, 2014, 2015
 ** Houkes Horeca Applications
 **
 ** This file is part of the SDL2UI Demo.  This library is free
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

typedef int language;

// Language ID's, let's keep these in alphabetical order
typedef enum
{
	_CANCEL, _EMPTY, _ENTER, _HEADER_TITLE, _OK, _PREVIOUS,

	// Should be last element!
	MAX_STRINGS
} ElanguageID;

// This is for the framework THE interface to get all strings in all languages
// Strings should not be in the sdl-library, but in your specific application.
// @param id [in] What text we prefer, can be any enumerator
// @param country [in] What language we want it to be.
// The code here is just an example, not part of the library
extern std::string find_some_translation(int id, language country);
