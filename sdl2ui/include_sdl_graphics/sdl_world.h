/*============================================================================*/
/**  @file       sdl_world.h
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
#include <memory>
#include <vector>
#include <string>
#include <SDL.h>
#include <pthread.h>
#include "my_thread.h"
#include "sdl_world_interface.h"
#include "sdl_dialog_list.h"
#include "sdl_key_file.h"

class Cdialog;
class CmessageBox;

/// @brief Dialog on screen.
class Cworld : public Iworld
{
public:
	Cworld(std::shared_ptr<Cgraphics> mainGraph);
	virtual ~Cworld();
	void init();
	void invalidate();
	void paintAll();
	void clean();
	void addDialog( Cdialog *dialog);
	std::shared_ptr<Cgraphics> graphics();
	void registerMessageBox(Cdialog *child);
	void unregisterMessageBox(Cdialog *child);
	void invalidateAll();
	void notifyInvalidate();
	void setActiveDialog( Cdialog *dialog);
	bool onLoop();
	void lock() { m_lock.lock(); }
	void unlock() { m_lock.unlock(); }
	void onCleanup();
	Estatus tryButton(keymode mod, keybutton sym);
	Cdialog *findDialog( const Cpoint &p);
	void onRender();
	void checkInMainThread();

public:
	CdialogList	m_dialogs;
	Cdialog 	*m_active_dialog;
	CdialogList m_message_box;
	CdialogList m_dialog_list;
	static pthread_t	m_main_thread;
	std::shared_ptr<Cgraphics> m_main_graph;
	CmyLock m_lock;
protected:
	static CkeyFile		m_key_file;			///< File to repeat key pressed.

private:
	Cpoint		m_drag_point;		///< point to drag
	bool		m_invalidate;		///< Need to repaint
	static int 	m_init;				///< How many worlds?
};

/* APPLICATION_DIALOG_H_ */
