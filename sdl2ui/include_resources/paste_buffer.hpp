/*
 * paste_buffer.hpp
 *
 *  Created on: 5 jan 2022
 *      Author: mensfort
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2022
 ** Houkes Horeca Applications
 **===========================================================================*/

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <string>
#include <limits.h>
#include <X11/Xlib.h>

// @brief Class to get the copy paste buffer from Linux
class CpasteBuffer
{
public:
	CpasteBuffer();
	virtual ~CpasteBuffer();
	std::string getPaste();

private:
	std::string getString(Display *display, Window window, const char *bufname, const char *fmtname);

};

/* End PASSWORD_H */
