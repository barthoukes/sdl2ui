/*============================================================================*/
/** @file       singleton.h
 *   @ingroup   source_files
 *  @brief      Basic singleton, nothing fancy.
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
#include <stddef.h>

/// Singleton class without reference counting.
template<class PARENT_CLASS> class Tsingleton
{
//	friend PARENT_CLASS;

	//============================================================================
	// Constructor / Destructor
protected:
	/// Constructor
	Tsingleton()
	{
	}
	/// Destructor
	virtual ~Tsingleton()
	{
	}

	//============================================================================
	// Tsingleton interface
public:
	/// Get instance 
	static PARENT_CLASS* Instance()
	{
		if( s_pInstance == NULL )
		{
			s_pInstance = new PARENT_CLASS();
		}
		return s_pInstance;
	}

	/** Kill instance */
	static void KillInstance()
	{
		if ( s_pInstance != NULL)
		{
			delete s_pInstance;
			s_pInstance = NULL;
		}
	}

private:
	/// Copy constructor
    Tsingleton(const Tsingleton& /*source*/)
    {
    };

private:
	static PARENT_CLASS*  s_pInstance; ///< Pointer to instance.
};

///////////////////////////////////////////////////////////////////////////////
//! static class member initialisation.
template <class PARENT_CLASS>

PARENT_CLASS* Tsingleton<PARENT_CLASS>::s_pInstance = NULL;



