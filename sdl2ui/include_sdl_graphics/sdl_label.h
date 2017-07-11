/*============================================================================*/
/**  @file      sdl_label.h
 **  @ingroup   user_interface
 **  @brief		Draw scrollbar.
 **
 **  Create and show scrollbar.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CsdlLabel
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
#include "sdl_dialog_object.h"
#include "sdl_font.h"

/*------------- Standard includes --------------------------------------------*/

/// @brief  Create and display buttons.
class Clabel : public CdialogObject
{
public:
	Clabel( Cdialog *parent, const Crect &rect, keybutton key);
	virtual ~Clabel();

public:
	void	onPaint( int touch);
	int		length();
	void	setText( const std::string &txt);
	void    calculateSize( const Crect &rect);

private:
	std::string   m_label; 	///< Text on the label
	bool 		  m_enable;	///< Is the label enabled?
	Sfont		  m_font;	///<
};

