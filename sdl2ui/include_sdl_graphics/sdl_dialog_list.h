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
#include "my_thread.h"
#include "sdl_types.h"

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

typedef std::vector<CdialogBase*>::iterator dialogBaseIterator;

/** @brief List of dialogs available.
 */
class CdialogList: public CmyLock
{
public:
	CdialogList();
	virtual ~CdialogList();

public:
	void addDialog( CdialogBase		*interface);
	void removeDialog( CdialogBase	*interface);
	CdialogBase *firstDialog();
	CdialogBase *lastDialog();
	CdialogBase *nextDialog( CdialogBase *current);
	CdialogBase *previousDialog( CdialogBase *current);
	Estatus onButton(keymode mod, keybutton sym);
	void clear();
	bool onPaint(); ///< Paint all buttons to the graphic layer
	dialogBaseIterator begin();
	dialogBaseIterator end();
	int size() { return (int)m_dialogs.size(); }
	void onRender(); ///< Move from graphic layer to the root

private:
	std::vector<CdialogBase*> m_dialogs;///< All dialogs available.
	CdialogBase *m_interface; ///< Interface dialog.
};

/* END  SDL_DIALOG_LIST_H_ */
