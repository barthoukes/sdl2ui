/*============================================================================*/
/**  @file       sdl_world_interface.h
 **  @ingroup    zhongcan_sdl
 **  @brief		 Default dialog.
 **
 **  Paint the world, with dialogs and message boxes
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cworld
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
#include <map>
#include <vector>
#include <memory>
#include <string>
#include <SDL.h>
#include <pthread.h>
#include "sdl_dialog_event.h"
#include "sdl_keybutton.h"

class Cdialog;
class Cgraphics;
class CmessageBox;

class Iworld
{
public:
	Iworld(std::shared_ptr<Cgraphics> mainGraph):
		m_main_graph(mainGraph) {}
	virtual ~Iworld() {}
	virtual std::shared_ptr<Cgraphics> graphics() { return m_main_graph; }
	virtual void init() = 0;
	virtual void paintAll() = 0;
	virtual void setActiveDialog(Cdialog *dialog) = 0;
	virtual void lock() = 0;
	virtual void unlock() = 0;
	virtual void invalidateAll() = 0;
	virtual void notifyInvalidate() = 0;
	virtual bool onLoop() = 0;
	virtual void onCleanup() = 0;
	virtual Estatus tryButton(keymode mod, keybutton sym) = 0;
	virtual Cdialog *findDialog( const Cpoint &p) = 0;
	virtual void registerMessageBox(Cdialog *child) = 0;
	virtual void unregisterMessageBox(Cdialog *child) = 0;
	virtual void checkInMainThread() = 0;

public:
	std::shared_ptr<Cgraphics> m_main_graph;  ///< Main graph for this world
};

/* APPLICATION_DIALOG_H_ */
