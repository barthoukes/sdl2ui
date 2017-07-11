/*============================================================================*/
/**  @file       message_box.cpp
 **  @ingroup    sdl2ui
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

/*------------- Standard includes --------------------------------------------*/
#include <assert.h>
#include "sdl_message_box.h"
#include "timeout.h"
#include "sdl_dialog_list.h"
#include "timestamp.h"

#define BTN_WIDTH  (Cgraphics::m_defaults.button_height+2)
#define BTN_HEIGHT (Cgraphics::m_defaults.button_height)


#if MBX
void *messagebox_interrupt(void *mbx)
{
	CmessageBox *mbox =
			static_cast<CmessageBox*> (mbx);
	if (mbox != NULL)
	{
		mbox->onExecute();
	}
	return NULL;
}
#endif

/** @brief Constructor message box
 *  @param name [in] Name for message box
 *  @param id [in] Text id for top string
 */
CmessageBox::CmessageBox( const std::string &name, textId id)
: Cdialog( NULL, name, Crect( 56, 4, 30, 18))
, m_ok( this, Crect( 0,0, BTN_WIDTH,BTN_HEIGHT), KEY_CR, Cgraphics::m_defaults.icon_ok48)
, m_cancel( this, Crect( 0,0, BTN_WIDTH,BTN_HEIGHT), KEY_CANCEL, Cgraphics::m_defaults.icon_cancel48)
{
	m_push =true;

	m_text =Cgraphics::m_defaults.get_translation( id, Cgraphics::m_defaults.country);
	setFlags( MB_TIME2|MB_BEEP);
	registerMessageBox();
	if ( m_flags & MB_DARKEN)
	{
		m_graphics->darken( 0,0,0,0);
	}

	if ( !(m_flags & MB_RETURN_IMMEDIATELY))
	{
		onExecute( NULL);
	}
	else if ( m_in_main_thread ==true)
	{
		m_world->lock();
		onInit();
		onClearScreen();
		onPaintButtons();
		onPaint();
		if (m_render_after_paint)
		{
			onRender();
		}
		onDisplay();

		m_world->unlock();
	}
}

/*============================================================================*/
///
/// @brief 		I am the messagebox and want to register.
///
/// @post       Message box removed, should not be displayed anymore.
///
/*============================================================================*/
void CmessageBox::registerMessageBox()
{
	m_push =true;
	m_isMessageBox=true;
	m_world->registerMessageBox(this);
}

/*============================================================================*/
///
/// @brief 		Standard execution for a message box.
/// First a dialog is displayed. When a stop event is created, the dialog closes
/// Keys are handled by the class and display is updated when invalidated.
///
/// @post       Class executed and ready. Not destroyed yet, parent should do that.
///
/*============================================================================*/
int CmessageBox::onExecute( Cdialog *parent)
{
	CdialogBase *a;
	(void)parent;

	// "begin CmessageBox::onExecute:%s %d", m_name.c_str(), m_in_main_thread);
	m_running =true;

	if (onInit() == false)
	{
		m_running =false;
	}
	if ( m_running )
	{
		onExpose();
	}
	m_touchList.clear();
	startExecute();

	while (m_running)
	{
		bool no_action =true;
#ifndef ZHONGCAN_MULTI_THREAD
		CdialogEvent::Instance()->work();
#endif
  		//if (m_invalidate && m_in_main_thread)
  		if ( m_in_main_thread)
		{
			m_world->lock();
			if ( m_visible)
			{
				onClearScreen();
				onPaintButtons();
				onPaint();
				if (m_render_after_paint)
				{
					onRender();
				}
				m_invalidate =false;
			}
			onDisplay();
			m_invalidate =false;
			m_world->unlock();
		}
		if ( onLoop() ==false)
		{
			m_running =false;
		}
		if ( !m_running)
		{
			break;
		}
		if (m_in_main_thread)
		{
			EpollStatus poll;
			while ( m_running ==true && ( poll=CdialogEvent::Instance()->pollEvent( this))!=POLL_EMPTY)
			{
				no_action =false;
				if ( poll ==POLL_TESTING)
				{
					break;
				}
			}
#if 0
			if ( m_idiot)
			{
				no_action =false;
				createRandomEvent(&m_event);
				onEvent(&m_event);
			}
#endif
			if ( !m_running)
			{
				break;
			}
			if ( m_touchList.update( CdialogEvent::Instance()->pressMouse(), CdialogEvent::Instance()->lastMouse()))
			{
				invalidate();
			}
			if ( no_action==true)
			{
				delay(10);
			}
		}
		else
		{
			delay(100);
		}
	}
	stopExecute();
	// "Cdialog::onExecute onCleanUp");
	onCleanup();
	for ( a=m_children.firstDialog(); a !=NULL; a =m_children.nextDialog( a))
	{
		Cdialog *d=dynamic_cast<Cdialog*>(a);
		if (d) d->onCleanup();
	}
	// ("end Cdialog::onExecute:%s %d", m_name.c_str(), m_exitValue);
	delay(50);
	if ( m_selfDestruct)
	{
		m_alive =false;
	}
	//if ( parent !=NULL)
	//{
	//	dlgLock();
	//}

	return m_exitValue;
}

/** @brief Stop message box
 *  @param exitValue [in] Value for exit
 */
void CmessageBox::stop(int exitValue)
{
	// ( "Stop message box %s exit=%d", m_name.c_str(), exitValue);
	m_exitValue=exitValue;
	m_running =false;
	invalidate();
}

/*============================================================================*/
///
/// @brief 		Set flags for message box.
/// @param		FLAGS [in] Flags for data
/// @post       Flags set, maybe popup sound played.
///
/*============================================================================*/
void CmessageBox::setFlags( int FLAGS)
{
	m_flags =FLAGS;
	if ( m_flags & MB_BEEP)
	{
		Caudio::Instance()->play( Cgraphics::m_defaults.audio_popup);
		m_flags &=~MB_BEEP;
	}

	/* Return if nessecary */
	if ( m_flags & MB_TIME1)
	{
		m_timer.setTime( Cgraphics::m_defaults.messagebox_time*2, 1000, 1);
	}
	if (m_flags & MB_TIME2)
	{
		m_timer.setTime( Cgraphics::m_defaults.messagebox_time*4, 1000, 1);
	}
	if (m_flags & MB_TIME3)
	{
		m_timer.setTime( Cgraphics::m_defaults.messagebox_time*8, 1000, 1);
	}
	if (m_flags & MB_TIME5)
	{
		m_timer.setTime( Cgraphics::m_defaults.messagebox_time*10, 1000, 1);
	}
	if (m_flags & MB_TIME10)
	{
		m_timer.setTime( Cgraphics::m_defaults.messagebox_time*20, 1000, 1);
	}
	m_selfDestruct =false;
	if ( m_flags & MB_SELF_DESTRUCT)
	{
		m_selfDestruct =true;
	}
	if (m_flags & MB_START_INVISIBLE)
	{
		m_visible =false;
	}
	if ( m_flags & MB_OK)
	{
		registerObject( &m_ok);
	}
	if ( m_flags & MB_QUIT)
	{
		registerObject( &m_cancel);
	}
}

/** @brief Construct a message box
 *  @param name [in] name of dialog
 *  @param x [in] Position
 *  @param y [in] Position
 *  @param w [in] Width
 *  @param h [in] Height
 *  @param Text [in] Text string for top
 *  @param FLAGS [in] All floags for message box
 */
CmessageBox::CmessageBox( const std::string &name, int x, int y, int w, int h,
		const std::string &Text, int FLAGS) :
	Cdialog( NULL, name, Crect(x, y, w, h))
, m_ok( this, Crect( 0,0, BTN_WIDTH,BTN_HEIGHT), KEY_CR, Cgraphics::m_defaults.icon_ok48)
, m_cancel( this, Crect( 0,0, BTN_WIDTH,BTN_HEIGHT), KEY_CANCEL, Cgraphics::m_defaults.icon_cancel48)
{
	m_text =Text;
	setFlags(FLAGS);
	registerMessageBox();
	if ( m_flags & MB_DARKEN)
	{

		graphics()->darken(0,0,0,0);
	}
	onInit();
	if ( m_flags & MB_DISPLAY_IMMEDIATELY)
	{
		m_world->lock();

		onInit();
		onClearScreen();
		onPaintButtons();
		onPaint();
		if (m_render_after_paint)
		{
			onRender();
		}
		onDisplay();

		m_world->unlock();
	}
	if ( !(m_flags & (MB_SELF_DESTRUCT | MB_RETURN_IMMEDIATELY)))
	{
		onExecute( NULL);
	}
	m_world->lock();
	//m_in_main_thread =false;
	m_running =true;
	m_world->unlock();
}

/** @brief Construct a message box
 *  @param name [in] name of dialog
 *  @param x [in] Position
 *  @param y [in] Position
 *  @param w [in] Width
 *  @param h [in] Height
 *  @param id [in] Text id for top string
 *  @param FLAGS [in] All floags for message box
 */
CmessageBox::CmessageBox(  const std::string &name, int x, int y, int w, int h, textId id, int FLAGS) :
			Cdialog( NULL, name, Crect(x, y, w, h))
, m_ok( this, Crect( 0,0,Cgraphics::m_defaults.button_height+2,Cgraphics::m_defaults.button_height), KEY_CR, Cgraphics::m_defaults.icon_ok48)
, m_cancel( this, Crect( 0,0, BTN_WIDTH,BTN_HEIGHT), KEY_CANCEL, Cgraphics::m_defaults.icon_cancel48)
{
	m_text =Cgraphics::m_defaults.get_translation( id, Cgraphics::m_defaults.country);
	setFlags(FLAGS);
	registerMessageBox();
	if ( m_flags & MB_DARKEN)
	{
		graphics()->darken( 0,0, Cgraphics::m_defaults.width, Cgraphics::m_defaults.height);
	}
	onInit();

	if ( m_flags & MB_DISPLAY_IMMEDIATELY)
	{
		m_world->lock();

		onInit();
		onClearScreen();
		onPaintButtons();
		onPaint();
		if (m_render_after_paint)
		{
			onRender();
		}
		onDisplay();

		m_world->unlock();
	}

	if ( !(m_flags & MB_RETURN_IMMEDIATELY))
	{
		onExecute( NULL);
	}
	//m_in_main_thread =false;
	m_running =true;
}

/** @brief Construct a message box
 *  @param name [in] name of dialog
 *  @param xx [in] Position and size dialog
 *  @param id [in] Text id for top string
 *  @param FLAGS [in] All floags for message box
 */
CmessageBox::CmessageBox( const std::string &name, const Crect &xx, textId id, int FLAGS)
: Cdialog( NULL, name, xx)
, m_ok( this, Crect( 0,0, BTN_WIDTH,BTN_HEIGHT), KEY_CR, Cgraphics::m_defaults.icon_ok48)
, m_cancel( this, Crect( 0,0, BTN_WIDTH,BTN_HEIGHT), KEY_CANCEL, Cgraphics::m_defaults.icon_cancel48)
{
	m_text =Cgraphics::m_defaults.get_translation( id, Cgraphics::m_defaults.country);
	setFlags(FLAGS);
	registerMessageBox();
	if ( m_flags & MB_DARKEN)
	{
		m_graphics->darken( 0,0,0,0);
	}
	if ( m_flags & MB_DISPLAY_IMMEDIATELY)
	{
		m_world->lock();
		onInit();
		onClearScreen();
		onPaintButtons();
		onPaint();
		if (m_render_after_paint)
		{
			onRender();
		}
		onDisplay();
		m_world->unlock();
	}

	if ( !(m_flags & MB_RETURN_IMMEDIATELY))
	{
		onExecute( NULL);
	}
	//m_in_main_thread =false;
	m_running =true;
}

keybutton CmessageBox::key()
{
	keybutton retVal =m_key;
	m_key =KEY_NONE;
	return retVal;
}

CmessageBox::CmessageBox( const std::string &name) :
			Cdialog( NULL, name, Crect(56, 4, 20, 8))
, m_ok( this, Crect( 0,0, BTN_WIDTH,BTN_HEIGHT), KEY_CR, Cgraphics::m_defaults.icon_ok48)
, m_cancel( this, Crect( 0,0, BTN_WIDTH,BTN_HEIGHT), KEY_CANCEL, Cgraphics::m_defaults.icon_cancel48)
{
	// TODO Auto-generated constructor stub
	//pushScreen();
	registerMessageBox();

	if ( m_flags & MB_DISPLAY_IMMEDIATELY)
	{
		m_world->lock(); // Dialog list should not change.
		onInit();
		CdialogEvent::Instance()->lock();

		onClearScreen();
		onPaintButtons();
		onPaint();
		if (m_render_after_paint)
		{
			onRender();
		}
		onDisplay();

		CdialogEvent::Instance()->unlock();
		m_world->unlock();
	}
}

CmessageBox::~CmessageBox()
{
	//("begin CmessageBox::~CmessageBox %s", m_name.c_str() );
	if ( m_push ==true)
	{
		m_world->unregisterMessageBox(this);
		m_push =false;
	}
	//("end CmessageBox::~CmessageBox");
}

/** @brief Init the message box
 *  @return true on success
 */
bool CmessageBox::onInit()
{
	int x=m_rect.right()-Cgraphics::m_defaults.button_height*2-2;
	int y=m_rect.bottom()-Cgraphics::m_defaults.button_height-1;

	switch ( m_flags & MB_STYLE)
	{
	case MB_OK:
		m_ok.m_rect.setPosition( Cpoint(x,y));
		m_graphics->setCode( m_ok.m_rect.offset(0,0,Cgraphics::m_defaults.button_height*2+1,Cgraphics::m_defaults.button_height), KEY_CR);
		x=x-Cgraphics::m_defaults.button_height*2-1;
		break;

	case MB_QUIT_OK:
		m_ok.m_rect.setPosition( Cpoint(x,y));
		m_graphics->setCode( m_ok.m_rect.offset(0,0,Cgraphics::m_defaults.button_height*2+1,Cgraphics::m_defaults.button_height), KEY_CR);
		x=x-2*Cgraphics::m_defaults.button_height-1;
		m_cancel.m_rect.setPosition( Cpoint(x,y));
		m_graphics->setCode( m_ok.m_rect.offset(0,0,Cgraphics::m_defaults.button_height*2+1,Cgraphics::m_defaults.button_height), KEY_ESCAPE);
		x=x-Cgraphics::m_defaults.button_height*2-1;
		break;

	case MB_QUIT:
		m_cancel.m_rect.setPosition( Cpoint(x,y));
		m_graphics->setCode( m_ok.m_rect.offset(0,0,Cgraphics::m_defaults.button_height*2+1,Cgraphics::m_defaults.button_height), KEY_ESCAPE);
		x=x-Cgraphics::m_defaults.button_height*2-1;
		break;
	}
	return true;
}

/*============================================================================*/
///
/// @brief 		Update called regularly, even when no messages are coming.
///
/// @post       Update performed.
///
/*============================================================================*/
void CmessageBox::onUpdate()
{
}

/** @brief Clear the dialog content before sending to the screen
 */
void CmessageBox::onClearScreen()
{
	graphics()->darken( 8*(m_rect.left()+2), 8*(m_rect.top()+2),
			             8*m_rect.width(), 8*m_rect.height());
    Cbackground( NULL, m_rect, KEY_NONE, Cgraphics::m_defaults.messagebox_background2,
    		     0, FILL_GRADIENT, Cgraphics::m_defaults.messagebox_background1).onPaint(0);
    Egravity gravity =(m_flags&MB_TEXT_CENTER) ? GRAVITY_CENTER:GRAVITY_LEFT;
    Ctext( this, m_rect, KEY_NONE, (Sfont)CtextFont("msb_box"), m_text,
    		Cgraphics::m_defaults.messagebox_text, gravity).onPaint(0);
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
void CmessageBox::onPaint()
{
	int btn1 =Cgraphics::m_defaults.button_height;
    //int big =(m_flags & MB_BIG ) ? BIG:0;
	if ( m_flags & MB_PRINTER_READY)
	{
		graphics()->image( Cgraphics::m_defaults.icon_printer48,
				 m_rect.left()*8, m_rect.top()*8, (m_rect.left()+btn1)*8, (m_rect.top()+btn1)*8);
		CcalculatorButton( Crect( m_rect.left()+btn1, m_rect.top(), m_rect.left()+btn1*2, m_rect.top()+btn1),
			               "ok", KEY_NONE);
	}
	if ( m_flags & MB_PRINTING)
	{
		graphics()->image( Cgraphics::m_defaults.icon_printer48,
				            m_rect.left()*8, m_rect.top()*8, (m_rect.left()+btn1)*8,btn1*8);
	}
//	switch ( m_flags & MB_STYLE)
//	{
//	case MB_OK:
//		m_graphics->setCode( m_ok.m_rect.offset(0,0,13,6), KEY_CR);
//		m_ok.onPaint( Cpoint( m_rect.right()-1-m_ok.m_rect.width(), m_rect.bottom()-7), NO_TOUCH);
//		break;
//
//	case MB_QUIT_OK:
//		m_graphics->setCode( m_ok.m_rect.offset(0,0,13,6), KEY_CR);
//		m_ok.m_rect.setPosition( Cpoint( m_rect.right-2-m_ok.m_rect.width(), m_rect.bottom()-7));
//		m_cancel.m_rect.setPosition( m_ok.m_rect.m_origin-Cpoint(13,0));
//		m_graphics->setCode( m_ok.m_rect, KEY_CR);
//		m_graphics->setCode( m_cancel.m_rect, KEY_ESCAPE);
//		m_ok->onPaint(0);
//		m_cancel->onPaint(0);
//		break;
//
//	case MB_QUIT:
//		m_graphics->setCode( m_cancel.m_rect.offset(0,0,13,6), KEY_CR);
//		m_cancel.onPaint( Cpoint( m_rect.right()-1-m_ok.m_rect.width(), m_rect.bottom()-7), 0);
//		break;
//	}
}

/** @brief Handle key events
 *  @param mod [in] Shift, Space, Ctrl and other main keys
 *  @param sym [in] The real symbol picked
 *  @return Return status, to see if the event was handled. If not it is sent to parents, brothers etc.
 */
Estatus CmessageBox::onButton( keymode mod, keybutton sym)
{
	(void)mod;
	Ctimestamp d;
	if (!( m_flags & MB_STYLE) )
	{
		return DIALOG_EVENT_OPEN;
	}
	if ( sym==KEY_NONE)
	{
		return DIALOG_EVENT_PROCESSED;
	}
	m_key=sym;

	switch (sym)
	{
	case KEY_ESCAPE:

		m_visible =false;
		invalidate();
		if ( m_selfDestruct)
		{
			m_alive =false;
		}
		m_running =false;
		//stop(0);
		break;
	case KEY_CR:
		m_visible =false;
		invalidate();
		if ( m_selfDestruct)
		{
			m_alive =false;
		}
		m_running =false;
		//stop(0);
		break;
	default:
		return DIALOG_EVENT_OPEN;
	}
	return DIALOG_EVENT_PROCESSED;
}

/** @brief Regularly called function during the mailbox
 *  @return true on success
 */
bool CmessageBox::onLoop()
{
	if ( m_flags & (MB_TIME1|MB_TIME2|MB_TIME3|MB_TIME5|MB_TIME10))
	{
		if ( m_timer.expired() && ( m_visible==true || m_running==true) )
		{
			m_visible =false;
			m_running =false;
			invalidate();
		}
	}
	return m_alive;
}

/** @brief Exit function for the dialog */
void CmessageBox::onExit()
{
	if (m_flags & MB_RETURN)
	{

	}
}

/** @brief Called to clear all variables after closing the dialog */
void CmessageBox::onCleanup()
{
	if ( m_push)
	{
		m_world->unregisterMessageBox(this);
	}
}

/** @brief Clear the dialog at the end of the software, exit software */
void CmessageBox::clear()
{
	if ( m_push ==true)
	{
		//pollScreen();
		m_world->unregisterMessageBox(this);
		m_push =false;
	}
}
