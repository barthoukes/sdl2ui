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
 **  Copyright (c) Bart Houkes, 26 dec 2012
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <stdio.h>
#include "keybutton.h"

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
	void onEvent(SDL_Event* event);

private:
	bool m_mouseDown; ///< If no mouse pressed, then forget mouse move events.
};

