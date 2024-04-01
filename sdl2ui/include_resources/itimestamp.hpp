/*
 * itimestamp.hpp
 *
 *  Created on: 30 aug 2020
 *      Author: mensfort
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

#include <memory>
#include <string>

/// @brief Timestamp to add in classes.
class Itimestamp
{
public:
	Itimestamp() {}
	virtual ~Itimestamp() {}

	virtual int getYear() const =0;
	virtual int getHours() const =0;
	virtual int getMinutes() const =0;
	virtual int getSeconds() const =0;
	virtual int getDay() const =0;
	virtual int getMonth() const =0;
	virtual int getQuarter() const =0;
	virtual int getMilliseconds() const =0;
	virtual int getWeek() =0;
};

typedef std::shared_ptr<Itimestamp> ItimestampPtr;
