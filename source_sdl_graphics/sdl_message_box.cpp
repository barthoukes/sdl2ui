/*============================================================================*/
/**  @file       message_box.cpp
 **  @ingroup    zhongcan_sdl
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
 **  Copyright (c) Bart Houkes, 7 mrt 2011
 **
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **===========================================================================*/

/*------------- Standard includes --------------------------------------------*/
#include <assert.h>
#include "sdl_message_box.h"
#include "graphics_widgets.h"
#include "timeout.h"
#include "sdl_dialog_list.h"

#define D false

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

CmessageBox::CmessageBox( const std::string &name, EtextId id)
: Cdialog( NULL, name, Crect( 56, 4, 30, 18))
, m_ok( this, Crect( 0,0,13,6), KEY_CR, Cgraphics::m_defaults.icon_ok48)
, m_cancel( this, Crect( 0,0,13,6), KEY_CANCEL, Cgraphics::m_defaults.icon_cancel48)
{
	m_push =true;

	m_text = Lin[id][Lang];
	setFlags( MB_TIME2|MB_BEEP);
	registerMessageBox();
	if ( m_flags & MB_DARKEN)
	{
		DarkenAT(0,0, SQUARES_WIDTH, SQUARES_HEIGHT);
	}	if ( m_flags & MB_DARKEN)
	{
		DarkenAT(0,0, SQUARES_WIDTH, SQUARES_HEIGHT);
	}

	if ( !(m_flags & MB_RETURN_IMMEDIATELY))
	{
		onExecute( NULL);
	}
	else if ( m_in_main_thread ==true)
	{
		g_messageBox.lock();

		onClearScreen();
		onPaintButtons();
		onPaint();
		onDisplay();

		g_messageBox.unlock();
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
	Cdialog::registerMessageBox(this);
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

	if(D)Log.write( "begin CmessageBox::onExecute:%s %d", m_name.c_str(), m_in_main_thread);
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
			g_myWorld.lock();
			Lang =m_language;
			if ( m_visible)
			{
				onClearScreen();
				onPaintButtons();
				onPaint();
				m_invalidate =false;
			}
			onDisplay();
			m_invalidate =false;
			g_myWorld.unlock();
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
	if(D)Log.write("Cdialog::onExecute onCleanUp");
	onCleanup();
	for ( a=m_children.firstDialog(); a !=NULL; a =m_children.nextDialog( a))
	{
		Cdialog *d=dynamic_cast<Cdialog*>(a);
		if (d) d->onCleanup();
	}
	//Log.write("end Cdialog::onExecute:%s %d", m_name.c_str(), m_exitValue);
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

void CmessageBox::stop(int exitValue)
{
	if(D)Log.write( "Stop message box %s exit=%d", m_name.c_str(), exitValue);
	m_exitValue=exitValue;
	m_running =false;
	invalidate();
}

/*============================================================================*/
///
/// @brief 		Set flags for message box.
///
/// @post       Flags set, maybe popup sound played.
///
/*============================================================================*/
void CmessageBox::setFlags( int FLAGS)
{
	m_flags =FLAGS;
	if ( m_flags & MB_BEEP)
	{
		Caudio::Instance()->play(AUDIO_POPUP);
		m_flags &=~MB_BEEP;
	}

	/* Return if nessecary */
	if ( m_flags & MB_TIME1)
	{
		m_timer.setTime( CFG("messagebox_time")*2, 0, 1);
	}
	if (m_flags & MB_TIME2)
	{
		m_timer.setTime( CFG("messagebox_time")*4, 0, 1);
	}
	if (m_flags & MB_TIME3)
	{
		m_timer.setTime( CFG("messagebox_time")*8, 0, 1);
	}
	if (m_flags & MB_TIME5)
	{
		m_timer.setTime( CFG("messagebox_time")*10, 0, 1);
	}
	if (m_flags & MB_TIME10)
	{
		m_timer.setTime( CFG("messagebox_time")*20, 0, 1);
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
}

CmessageBox::CmessageBox( const std::string &name, int x, int y, int w, int h,
		const std::string &Text, int FLAGS) :
	Cdialog( NULL, name, Crect(x, y, w, h))
, m_ok( this, Crect( 0,0,13,6), KEY_CR, ICON_OK_TEXT48)
, m_cancel( this, Crect( 0,0,13,6), KEY_CANCEL, ICON_CANCEL_TEXT48)
{
	m_text =Text;
	setFlags(FLAGS);
	registerMessageBox();
	if ( m_flags & MB_DARKEN)
	{
		DarkenAT(0,0, SQUARES_WIDTH, SQUARES_HEIGHT);
	}
	if ( m_flags & MB_DISPLAY_IMMEDIATELY)
	{
		g_messageBox.lock();

		onClearScreen();
		onPaintButtons();
		onPaint();
		onDisplay();

		g_messageBox.unlock();
	}
	if ( !(m_flags & (MB_SELF_DESTRUCT | MB_RETURN_IMMEDIATELY)))
	{
		onExecute( NULL);
	}
	g_messageBox.lock();
	//m_in_main_thread =false;
	m_running =true;
	g_messageBox.unlock();
}

CmessageBox::CmessageBox(  const std::string &name, int x, int y, int w, int h, EtextId id, int FLAGS) :
			Cdialog( NULL, name, Crect(x, y, w, h))
, m_ok( this, Crect( 0,0,13,6), KEY_CR, ICON_OK_TEXT48)
, m_cancel( this, Crect( 0,0,13,6), KEY_CANCEL, ICON_CANCEL_TEXT48)
{
	m_text = Lin[id][Lang];
	setFlags(FLAGS);
	registerMessageBox();
	if ( m_flags & MB_DARKEN)
	{
		DarkenAT(0,0, SQUARES_WIDTH, SQUARES_HEIGHT);
	}
	if ( m_flags & MB_DISPLAY_IMMEDIATELY)
	{
		g_messageBox.lock();

		onClearScreen();
		onPaintButtons();
		onPaint();
		onDisplay();

		g_messageBox.unlock();
	}

	if ( !(m_flags & MB_RETURN_IMMEDIATELY))
	{
		onExecute( NULL);
	}
	//m_in_main_thread =false;
	m_running =true;
}

CmessageBox::CmessageBox( const std::string &name, const Crect &xx, EtextId id, int FLAGS)
: Cdialog( NULL, name, xx)
, m_ok( this, Crect( 0,0,13,6), KEY_CR, ICON_OK_TEXT48)
, m_cancel( this, Crect( 0,0,13,6), KEY_CANCEL, ICON_CANCEL_TEXT48)
{
	m_text = Lin[id][Lang];
	setFlags(FLAGS);
	registerMessageBox();
	if ( m_flags & MB_DARKEN)
	{
		DarkenAT(0,0, SQUARES_WIDTH, SQUARES_HEIGHT);
	}
	if ( m_flags & MB_DISPLAY_IMMEDIATELY)
	{
		g_messageBox.lock();
		onClearScreen();
		onPaintButtons();
		onPaint();
		onDisplay();
		g_messageBox.unlock();
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
, m_ok( this, Crect( 0,0,13,6), KEY_CR, ICON_OK_TEXT48)
, m_cancel( this, Crect( 0,0,13,6), KEY_CANCEL, ICON_CANCEL_TEXT48)
{
	// TODO Auto-generated constructor stub
	//pushScreen();
	registerMessageBox();

	if ( m_flags & MB_DISPLAY_IMMEDIATELY)
	{
		g_messageBox.lock(); // Dialog list should not change.
		CdialogEvent::Instance()->lock();

		onClearScreen();
		onPaintButtons();
		onPaint();
		onDisplay();

		CdialogEvent::Instance()->unlock();
		g_messageBox.unlock();
	}
}

CmessageBox::~CmessageBox()
{
	if(D)Log.write("begin CmessageBox::~CmessageBox %s", m_name.c_str() );
	if ( m_push ==true)
	{
		unregisterMessageBox();
		m_push =false;
	}
	if(D)Log.write("end CmessageBox::~CmessageBox");
}

bool CmessageBox::onInit()
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
void CmessageBox::onUpdate()
{
}

void CmessageBox::onClearScreen()
{
	DarkenAT( m_rect.left()+2, m_rect.top()+2, m_rect.width(), m_rect.height());
    Cbackground( NULL, m_rect, KEY_NONE, COLOUR("COLOUR_MESSAGEBOX_BACKGROUND2"),
    		     0, FILL_GRADIENT, COLOUR("COLOUR_MESSAGEBOX_BACKGROUND1")).onPaint(0);
    Egravity gravity =(m_flags&MB_TEXT_CENTER) ? GRAVITY_CENTER:GRAVITY_LEFT;
    Ctext( this, m_rect, KEY_NONE, (Sfont)CtextFont("msb_box"), m_text,
    		COLOUR("COLOUR_MESSAGEBOX_TEXT"), gravity).onPaint(0);
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
    //int big =(m_flags & MB_BIG ) ? BIG:0;
	m_ok.setImage(ICON_OK_TEXT48, GRAVITY_CENTER);
	m_cancel.setImage(ICON_ESC_TEXT48, GRAVITY_CENTER);
	int x=m_rect.right();
 	if ( m_flags & MB_PRINTER_READY)
	{
		imageAT( ICON_PRINTER48, m_rect.left(), m_rect.top(), m_rect.left()+6, m_rect.top()+6);
		CcalculatorButton( Crect( m_rect.left()+6, m_rect.top(), m_rect.left()+12, m_rect.top()+6),
			               "ok", KEY_NONE);
	}
	if ( m_flags & MB_PRINTING)
	{
		imageAT( ICON_PRINTER48, m_rect.left(), m_rect.top(), m_rect.left()+6, m_rect.top()+6);
	}
	switch ( m_flags & MB_STYLE)
	{
	case MB_OK:
		m_ok.onPaint( Cpoint( m_rect.right()-1-m_ok.m_rect.width(), m_rect.bottom()-7), NO_TOUCH);
		break;

	case MB_QUIT_OK:
		m_ok.onPaint( Cpoint( m_rect.right()-2-m_ok.m_rect.width(), m_rect.bottom()-7), NO_TOUCH);
		m_cancel.onPaint( Cpoint( m_rect.right()-1-m_ok.m_rect.width()-m_cancel.m_rect.width(), m_rect.bottom()-7), NO_TOUCH);
		break;

	case MB_QUIT:
		m_cancel.onPaint( Cpoint( m_rect.right()-1-m_ok.m_rect.width(), m_rect.bottom()-7), 0);
		break;

	case MB_WAITER_MANAGER:
		CimageButton( Crect( x + 2, m_rect.bottom()-7, 13, 6), KEY_W, _WAITER, ICON_VLOER17).onPaint(0);
		CimageButton( Crect( x + 17, m_rect.bottom()-7, 13, 6), KEY_M, _MANAGER, ICON_VLOER17).onPaint(0);
		break;
	}
}

Estatus CmessageBox::onButton( SDLMod mod, keybutton sym)
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

void CmessageBox::onExit()
{
	if (m_flags & MB_RETURN)
	{

	}
}

void CmessageBox::onCleanup()
{
	if ( m_push)
	{
		unregisterMessageBox();
	}
}

void CmessageBox::clear()
{
	if ( m_push ==true)
	{
		//pollScreen();
		unregisterMessageBox();
		m_push =false;
	}
}
