/*============================================================================*/
/**  @file       lingual.h
 **  @ingroup    demo
 **  @brief		 all language strings
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

#include <string>
#include "lingual.h"

// strings, languages

#define MAX_LANGUAGES 2
std::string Lingual[MAX_STRINGS][2] =
{
		// _CANCEL,
		{	"CANCEL", "取消" },

		// _EMPTY
		{   "", "" },

		// _ENTER,
		{	"ENTER", "输入" },

		// _HEADER_TITLE,
		{   "SDL2UI DEMO", "SDL2UI 演示" },

		// _OK,
		{	"OK", "好 !" },

		// _PREVIOUS
		{   "PREVIOUS", "以前" }
};

int current_country =0;

std::string find_some_translation(int id, language country)
{
	return Lingual[id][country];
}
