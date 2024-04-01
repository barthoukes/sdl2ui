/*============================================================================*/
/**  @file       sdl_swype_interface.h
 **  @ingroup    zhongcan_user_interface
 **  @brief		 Common swype interface
 **
 **  Create a default swype inteface
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              IswypeInterface
 */
/*------------------------------------------------------------------------------
 ** Copyright (C) 2011, 2014, 2020
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
#include <memory>
#include "sdl_rect.h"

/// @brief Scrolling dialog with list of items.
class IswypeInterface
{
public:
	virtual ~IswypeInterface() {}
	virtual void onPaint() =0;
	//	virtual void setSpeed(double speed) =0;
//    virtual size_t rows() =0;
//    virtual void clean() =0;
//	virtual void onCleanup();
	virtual void onPaintUnit( int unit, const Crect &location) =0;
	//virtual void invalidate() =0;
//
//	virtual Estatus onButton( SDLMod mod, keybutton sym) =0;
	//virtual bool onLoop() =0;
};

/// Shared swype dialog
//typedef std::shared_ptr<IswypeInterface> CswypeInterfacePtr;

/* SCROLL_DIALOG_H_ */

