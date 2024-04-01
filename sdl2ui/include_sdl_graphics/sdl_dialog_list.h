/*============================================================================*/
/**  @file       sdl_dialog_list.h
 **  @ingroup    zhongcan_sdl
 **  @brief		 Keep any list of dialogs.
 **
 **  We have 2 standard lists, one for message boxes, another for our world.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CdialogList
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
#include <vector>
#include <memory>
#include <SDL_keysym.h>
#include "my_thread.hpp"
#include "sdl_keybutton.h"
#include "sdl_dialog_event.h"

/** @brief Basic interface for dialog. */
class CdialogBase
{
public:
	/** @brief Constructor. */
	CdialogBase() {}
	/** @brief Destructor. */
	virtual ~CdialogBase() {}
	virtual std::string getName()=0;
};

typedef std::shared_ptr<CdialogBase> CdialogBasePtr;

/** @brief List of dialogs available.
 */
class CdialogList: public CmyLock
{
public:
	CdialogList();
	virtual ~CdialogList();

public:
	void addDialog( CdialogBase	     *interface);
    void addDialog( CdialogBasePtr    interface);
    void removeDialog( CdialogBase   *interface);
	void removeDialog( CdialogBasePtr interface);
	void deleteDialogs();
	void clear();
    void resetPaintedArea();
    Cdialog *findDialog( const Cpoint &point);
    bool onInit();
    bool onLoop();
    bool onLoopWithSelfDestruct();
    void onCleanup();
    void onPaint();
    void onClearScreen();
    void invalidateSwypeDialogs();
    void clearScreenAndPaint();
    Estatus onButton(SDLMod mod, keybutton sym, Cdialog *parent, Estatus stat);

private:
    Cdialog *findDialogInternal( const Cpoint &point);
    CdialogBase *firstDialog();
    CdialogBase *lastDialog();
	std::vector<CdialogBase*> m_dialogs1;///< All dialogs available.
    std::vector<CdialogBasePtr> m_dialogs2;///< All dialogs available.
};

/* END  SDL_DIALOG_LIST_H_ */
