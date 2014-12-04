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
 **  Copyright (c) Bart Houkes, 17 jan 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

#pragma once

/*------------- Standard includes --------------------------------------------*/
#include <string>
#include "keybutton.h"
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
  MB_WAITER_MANAGER=4,
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
	CmessageBox( const std::string &name, EtextId id);
	CmessageBox( const std::string &name, const Crect &x, EtextId id, int FLAGS);
	CmessageBox( const std::string &name, const Crect &x, const std::string &Text, int FLAGS);
	CmessageBox( const std::string &name, int x, int y, int w, int h, const std::string &Text, int FLAGS);
	CmessageBox( const std::string &name, int x, int y, int w, int h, EtextId id, int FLAGS);
	virtual ~CmessageBox();

public:
	virtual int onExecute( Cdialog *parent);
	virtual bool onInit();
	virtual void onUpdate();
	virtual void onPaint();
	virtual void onClearScreen();
	virtual void onCleanup();
	virtual Estatus onButton( SDLMod mod, keybutton sym);
	virtual void onExit();
	virtual bool onLoop();
	virtual void clear();
	//virtual Estatus onEvent( Cevent & event);
	//virtual Estatus onLButtonDown(int mX, int mY);
	keybutton key();
	virtual void registerMessageBox();
	virtual bool isSwypeDialog( const Cpoint &p) { (void)p; return false; }
	void stop(int exitValue);

public:
	Ctimeout m_timer;
 	bool m_push;
	int  m_flags;
	std::string m_text;
	bool m_selfDestruct;

protected:
	Cimage  m_ok;
	Cimage  m_cancel;
	keybutton m_key;

private:
	void setFlags( int FLAGS);
};

