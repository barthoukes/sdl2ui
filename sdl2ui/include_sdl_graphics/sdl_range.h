/*============================================================================*/
/** @file       sdl_range.h
 *  @ingroup    source_files
 *  @brief      Range of 2 numbers
 *
 *  Creation Date: Sun - 27 - aug -2016
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
#include <vector>

template<typename T>
class Crange
{
public:
	T	m_min; ///< Minimum value.
	T	m_max; ///< Maximum value.

public:
	/// Constructor
	Crange( T range_min=0, T range_max=0)
	: m_min(range_min)
	, m_max(range_max)
	{
	}
	/// Destructor
	virtual ~Crange()
	{
	}
	bool inside(T value)
	{
		return (value>=m_min && value<m_max);
	}
	//============================================================================
	// Tsingleton interface
public:
	T low() { return m_min; }
	T high() { return m_max; }
	T range() { return m_max-m_min; }

private:

};

///////////////////////////////////////////////////////////////////////////////



