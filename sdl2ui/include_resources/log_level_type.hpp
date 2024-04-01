/*============================================================================*/
/** @file       log_level_type.hpp
**  @ingroup    include_files
**  @brief		Log types
** 
**  Log file interface.
**  Enum types to create a log file.
**
**  @author Bart Houkes
*/
/*------------------------------------------------------------------------------
**  Copyright (c) Bart Houkes, 2022
** 
**  Copyright notice:
**  This software is property of Bart Houkes
**  Unauthorized duplication and disclosure to third parties is forbidden.
**============================================================================*/

#pragma once

/*------------- Necessary include files --------------------------------*/

/*------------- Enum definitions ---------------------------------------*/

typedef enum
{
	LOG_NOTHING =0,
	LOG_ALL =1,
	LOG_WARNINGS =2
} ElogLevel;

/*========================================================================
**      end of file
**======================================================================*/
