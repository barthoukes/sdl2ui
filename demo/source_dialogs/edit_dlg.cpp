/*============================================================================*/
/**  @file    	edit_dlg.cpp
 **  @ingroup   demo
 **  @brief		Dialog for edit text
 **
 **  @author    mensfort
 **
 **  @par Classes:
 **             CeditDlg
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

/*------------- Standard includes --------------------------------------------*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "edit_dlg.h"
#include "lingual.h"

CeditDlg::CeditDlg(Iworld *world)
: Cdialog( NULL,"CmainDialog", Crect(0,0,0,0), world)
, m_header( Crect(0,0,m_squares_width,6), "EDIT", KEY_NONE)
, m_bottom1( this, Crect(m_squares_width*0/4 ,m_squares_height-6, m_squares_width/4, 6), KEY_F1, _CANCEL)
, m_bottom2( this, Crect(m_squares_width*1/4 ,m_squares_height-6, m_squares_width/4, 6), KEY_F2, _EMPTY, "left*.png")
, m_bottom3( this, Crect(m_squares_width*2/4, m_squares_height-6, m_squares_width/4, 6), KEY_F3, _EMPTY, "right*.png")
, m_bottom4( this, Crect(m_squares_width*3/4, m_squares_height-6, m_squares_width/4, 6), KEY_F4, _ENTER, "enter*.png")
{
	registerObject( &m_header);
	registerObject( &m_bottom1);
	registerObject( &m_bottom2);
	registerObject( &m_bottom3);
	registerObject( &m_bottom4);
}

CeditDlg::~CeditDlg()
{
}

bool CeditDlg::onInit()
{
	return true;
}

/*============================================================================*/
///
/// @brief 		Update called regularly, even when no messages are coming.
///
/// @post       Update performed.
///
/*============================================================================*/
void CeditDlg::onUpdate()
{
	assert(NULL);
}

/*============================================================================*/
///
///  @brief 	Paint the dialog once. Use invalidate() to re-paint soon.
///
///	 @pre		Called when dialog is invalidated.
///
///  @post		Dialog updated. Buttons, drag and background are done automatic.
///
/*============================================================================*/
void CeditDlg::onPaint()
{
}

bool CeditDlg::onLoop()
{
//	Ctimestamp c;
//	if (c.getSeconds() !=m_time.getSeconds())
//	{
//		m_time =c;
//		char s[256];
//		sprintf( s, "%02d :%02d :%02d", c.getHours(), c.getMinutes(), c.getSeconds());
//		//sprintf( s, "%02d :%02d :%02d", 0, 0, 0);
//		m_clock.setText(s);
//		m_clock.disableBackground();
//		invalidate();
//	}
	return true;
}

void CeditDlg::onExit()
{
}

void CeditDlg::onCleanup()
{
}

/*============================================================================*/
///
///  @brief 	Our button handler for this dialog
///
///  @param		sym [in] Symbol typed.
///  @param		mod [in] Mode like SHIFT, TAB, CTRL etc.
///
///  @post      Key handled then DIALOG_EVENT_PROCESSED.
///
/*============================================================================*/
Estatus CeditDlg::onButton(keymode mode, keybutton sym)
{
	if ( sym == KEY_CR )
	{
		stop(0);
		return DIALOG_EVENT_PROCESSED; // No need to show this button to other dialogs
	}
	switch (sym)
	{
	case KEY_F1:
		stop(0);
		break;
	case KEY_F2:
		stop(10);
		break;
	case KEY_F3:
		stop(12);
		break;
	default:
		break;
	}
	return DIALOG_EVENT_OPEN; // Show this button to next dialog... still open
}
