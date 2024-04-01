/*============================================================================*/
/** @file       singleton.hpp
 *   @ingroup   source_files
 *  @brief      Basic singleton, nothing fancy.
 *
 *  Creation Date: Sat - 1 january 2011
 *
 *  @author Bart Houkes
 */
/*------------------------------------------------------------------------------
 *  Copyright (c) Bart Houkes, 2011
 *
 *  Copyright notice:
 *  This software is property of Bart Houkes.
 *  Unauthorized duplication and disclosure to third parties is forbidden.
 *============================================================================*/

#pragma once

#include <stddef.h>

/// Singleton class without reference counting.
template<class PARENT_CLASS> class Tsingleton
{
//	friend PARENT_CLASS;

	//============================================================================
	// Constructor / Destructor
protected:
	/// Constructor
	Tsingleton() = default;

	/// Destructor
	virtual ~Tsingleton() = default;

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
    Tsingleton(const Tsingleton&) = delete;
    Tsingleton(Tsingleton&&) noexcept = delete;

private:
	static PARENT_CLASS*  s_pInstance; ///< Pointer to instance.
};

///////////////////////////////////////////////////////////////////////////////
//! static class member initialisation.
template <class PARENT_CLASS>

PARENT_CLASS* Tsingleton<PARENT_CLASS>::s_pInstance = NULL;



