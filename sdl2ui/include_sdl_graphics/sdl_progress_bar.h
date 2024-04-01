/*============================================================================*/
/**  @file      sdl_progress_bar.h
 **  @ingroup   user_interface
 **  @brief		Draw progress bar.
 **
 **  Create and show scrollbar.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CprogressBar
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
#include <string>
#include "sdl_slider.h"
#include "timeout.hpp"

/// @brief  Forward declaration.
class Cdialog;

/// @brief  Create and display buttons.
class CprogressBar : public Cslider
{
public:
	CprogressBar( const Crect &rect, keybutton button);
	virtual ~CprogressBar();

public:
	void	onPaint( int touch);
	void 	start();
	virtual void setValue( double y);

private:
	void 	paintBackground();
	void	paintBorder();
	void	paintImage();

private:
	Ctimeout	m_timer;
	int			m_todoTime;
	int			m_speed;
};


