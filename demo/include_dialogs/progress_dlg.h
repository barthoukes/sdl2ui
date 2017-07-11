/*============================================================================*/
/**  @file       progress_dlg.h
 **  @ingroup    demo
 **  @brief		 dialog
 **
 **  Demo for sdl2ui
 **
 **  @author     mensfort
 **
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
#include "sdl_message_box.h"
#include "sdl_button.h"
#include "timestamp.h"

class CprogressDlg : public Cdialog
{
public:
	CprogressDlg(Iworld *world);
	~CprogressDlg();

private:
	virtual bool onInit();
	virtual void onUpdate();
	virtual void onPaint();
	virtual void onCleanup();

	Estatus onButton( keymode mod, keybutton sym);
	virtual void onExit();
	virtual bool onLoop();

private:
	CheaderButton 		m_header; ///< Header for dialog
	CbottomButton		m_bottom1; ///< First button
	CbottomButton		m_bottom2; ///< 2nd button
	CbottomButton		m_bottom3; ///< 3rd button
	CbottomButton		m_bottom4; ///< 4st button
};
