/*============================================================================*/
/**  @file       message_box.cpp
 **  @ingroup    user_interface
 **  @brief		 Mai dan.
 **
 **  Simple message box.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CmessageBox
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
#include "sdl_keybutton.h"
#include "sdl_dialog.h"
#include "timeout.h"
#include "sdl_button.h"
#include "sdl_image.h"

typedef enum
{
  MB_READY =0,
  MB_OK=1,
  MB_QUIT=2,
  MB_QUIT_OK=3,
  MB_STYLE=7,
  MB_RETURN=8,
  MB_TIME1=16,
  MB_TIME2=32,
  MB_TIME3=64,
  MB_TIME5=128,
  MB_TIME10=256,
  MB_INVERSE=1024,
  MB_BEEP=512,
  MB_KEY=2048,
  MB_NETWORK=4096,
  MB_RETURN_IMMEDIATELY =16384,
  MB_BIG=32768,
  MB_START_INVISIBLE=0x10000,
  MB_SELF_DESTRUCT=0x20000,
  MB_PRINTER_READY=0x40000,
  MB_PRINTING=0x80000,
  MB_DISPLAY_IMMEDIATELY=0x100000,
  MB_DARKEN=0x200000,
  MB_TEXT_CENTER=0x400000,
} Ebutton;


class CmessageBox : public Cdialog
{
public:
	CmessageBox( const std::string &name);
	CmessageBox( const std::string &name, textId id);
	CmessageBox( const std::string &name, const Crect &x, textId id, int FLAGS);
	CmessageBox( const std::string &name, const Crect &x, const std::string &Text, int FLAGS);
	CmessageBox( const std::string &name, int x, int y, int w, int h, const std::string &Text, int FLAGS);
	CmessageBox( const std::string &name, int x, int y, int w, int h, textId id, int FLAGS);
	virtual ~CmessageBox();

public:
	virtual int onExecute( Cdialog *parent);
	virtual bool onInit();
	virtual void onUpdate();
	virtual void onPaint();
	virtual void onClearScreen();
	virtual void onCleanup();
	virtual Estatus onButton( keymode mod, keybutton sym);
	virtual void onExit();
	virtual bool onLoop();
	virtual void clear();
	keybutton key();
	virtual void registerMessageBox();
	virtual bool isSwypeDialog( const Cpoint &p) { (void)p; return false; }
	void stop(int exitValue);

private:
	Ctimeout m_timer;
 	bool m_push;
	int  m_flags;

protected:
	std::string m_text;
	Cimage  m_ok;
	Cimage  m_cancel;
	keybutton m_key;

private:
	void setFlags( int FLAGS);
};

