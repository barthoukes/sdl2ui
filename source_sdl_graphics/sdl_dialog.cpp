/*============================================================================*/
/**  @file       sdl_dialog.cpp
 **  @ingroup    zhongcan_sdl
 **  @brief		 Default dialog.
 **
 **  Create a default dialog. This is a basic template for all dialogs, including message boxes.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CDialog
 */
/*------------------------------------------------------------------------------
 **  Copyright (c) Bart Houkes, 28 jan 2011
 ** 
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

/*------------- Standard includes --------------------------------------------*/

#include <assert.h>
#include "sdl_dialog.h"
#include "sdl_swype_dialog.h"
#include "graphics_widgets.h"
#include "timeout.h"
#include "sdl_dialog_event.h"
#include "sdl_dialog_list.h"

int Cdialog::m_init = 0;
bool Cdialog::m_invalidate =true;
pthread_mutex_t Cdialog::m_objectMutex;
bool Cdialog::m_objectLocked =false;
pthread_mutexattr_t Cdialog::m_attr;
int Cdialog::m_main_thread =0;
bool Cdialog::m_useClick =true;
CkeyFile Cdialog::m_keyFile;
CdialogObject *Cdialog::m_dragObject =NULL;		///< Object to drag.
Cpoint Cdialog::m_dragOffset( 0,0);
Cpoint Cdialog::m_dragStart( 0,0);
Cpoint Cdialog::m_dragPoint( 0,0);

/*============================================================================*/
///
/// @brief Constructor.
///
/*============================================================================*/
Cdialog::Cdialog( Cdialog *parent, const std::string &name, const Crect &rect)
: m_alive(true)
, m_allButtons(false)
, m_backgroundColour( Cgraphics::m_defaults.background)
, m_exitValue(0)
, m_parent(parent)
, m_spaceIsLanguage(true)
, m_visible(true)
, m_isMessageBox(false)
, m_running(false)
, m_root(this)
, m_rect( rect)
, m_graphics( (parent==NULL || parent->m_graphics==NULL) ? m_mainGraph:parent->m_graphics)
{
	if (rect.width()==0 && rect.height()==0)
	{
		m_rect =Crect(0,0, Cgraphics::m_defaults.width, Cgraphics::m_defaults.height);
	}
	if (!m_init)
	{
		pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init( &m_objectMutex, &m_attr);
		m_main_thread =pthread_self();
		//m_language =static_cast<Taal>( CFG("default_language"));
		m_keyFile.init();
	}
	g_myWorld.lock();
	m_init++;
	m_name =name;
	m_current_thread =pthread_self();
	m_in_main_thread =(m_main_thread ==m_current_thread);
	// Dialog initialised
	g_myWorld.unlock();
}

/*============================================================================*/
///
/// @brief Destructor
///
/*============================================================================*/
Cdialog::~Cdialog()
{
	// Cdialog::~Cdialog
	if (m_init == 0)
	{
		g_myWorld.lock(); // Children+Messageboxes should not change.
		CdialogEvent::Instance()->stop();
		CdialogBase *a;
		for ( a=g_messageBox.firstDialog(); a!=NULL; a=g_messageBox.nextDialog( a))
		{
			Cdialog *t=dynamic_cast<Cdialog*>(a);
			if (t) delete t;
		}
		g_messageBox.clear();
		g_myWorld.unlock(); // Children+Messageboxes should not change end.

		delete m_graphics;
		m_graphics =NULL;
		CdialogEvent::Instance()->KillInstance();
	}
}

/** @brief Register a new child to handle for a dialog.
 *  @param child [in] New child.
 */
void Cdialog::registerChild(Cdialog *child)
{
	assert( (int)pthread_self()==m_main_thread);
	m_children.addDialog( child);
}

/** @brief Unregister a new child to handle for a dialog.
 *  @param child [in] New child.
 */
void Cdialog::unregisterChild(Cdialog *child)
{
	m_children.removeDialog( child);
}

/** @brief Unregister an object to handle for a dialog.
 *  @param child [in] New child.
 */
void Cdialog::unregisterObject(CdialogObject *child)
{
	objectLock(); // Object list should not change.

	for (std::vector<CdialogObject*>::iterator i=m_objects.begin();
		 i!=m_objects.end(); ++i)
	{
		if (*i==child)
		{
			m_objects.erase(i);
			//Log.write("Cdialog::unregisterObject:%s now has lost child object %s", m_name.c_str(), child->m_name.c_str());
			break;
		}
	}
	objectUnlock(); // Object list should not change end.
}

/** @brief Set new background colour to a dialog.
 *  @param colour [in] What colour to handle.
 */
void Cdialog::setBackgroundColour( colour colour)
{
	assert( (int)pthread_self()==m_main_thread);
	m_backgroundColour =colour;
	invalidate();
}

/** @brief Register a new child to handle for a dialog.
 *  @param child [in] New child object.
 */
void Cdialog::registerObject(CdialogObject *child)
{
	objectLock(); // Object list should not change.

	bool found =false;
	//child->m_parent =this;
	for (size_t i=0; i<m_objects.size(); ++i)
	{
		if (m_objects[i]==child)
		{
			found =true;
			break;
		}
	}
	if (!found)
	{
		m_objects.push_back(child);
	}
	//Log.write("Cdialog::registerObject:%s now has child object %s", m_name.c_str(), child->m_name.c_str());
	objectUnlock(); // Object list should not change end.
}

/*============================================================================*/
///
/// @brief 		Invalidate GUI. This will be re-painted.
///
/// @post       Class removed. Please also remove all references.
///
/*============================================================================*/
void Cdialog::invalidate()
{
	g_myWorld.lock();
	m_invalidate =true;
	g_myWorld.unlock();
}

/*============================================================================*/
///
/// @brief 		Invalidate GUI. This will be re-painted, including scroll dialogs.
///
/// @post       Class removed. Please also remove all references.
///
/*============================================================================*/
void Cdialog::invalidateAll()
{
	assert( (int)pthread_self()==m_main_thread);
	g_myWorld.lock();
	CdialogBase *a;
	for ( a =m_children.firstDialog(); a !=NULL; a =m_children.nextDialog( a))
	{
		CswypeDialog *dialog =dynamic_cast<CswypeDialog*>( a);
		if ( dialog !=NULL)
		{
			dialog->resetPaintedArea();
		}
	}
	m_invalidate =true;
	g_myWorld.unlock();
}

/*============================================================================*/
///
/// @brief 		Init the dialog.
///
/// @post       Dialog initialised.
///
/*============================================================================*/
bool Cdialog::onInit()
{
	assert( (int)pthread_self()==m_main_thread);
	invalidate();
	//Log.write( "Cdialog::onInit");
	return true;
}

/*============================================================================*/
///
/// @brief 		Loop for the dialog.
///
/// @post       Dialog changed.
///
/*============================================================================*/
bool Cdialog::onLoop()
{
	return m_alive;
}

/** @brief Stop the dialog.
 *  @param exitValue [in] How to stop a dialog.
 */
void Cdialog::stop(int exitValue)
{
	assert( (int)pthread_self()==m_main_thread);
	// Stop dialog m_name exitValue
 	m_exitValue=exitValue;
	m_running =false;
	invalidate();
}

/*============================================================================*/
///
/// @brief 		Standard execution for a dialog.
/// First a dialog is displayed. When a stop event is created, the dialog closes
/// Keys are handled by the class and display is updated when invalidated.
///
/// @post       Class executed and ready. Not destroyed yet, parent should do that.
///
/*============================================================================*/
int Cdialog::onExecute( Cdialog *parent)
{
	assert( (int)pthread_self()==m_main_thread);
	CdialogBase* a;

	m_exitValue =0;
	m_running =true;
	g_myWorld.addDialog( this);
	g_myWorld.lock();

	CeventInterface *stack =CdialogEvent::Instance()->getInterface();
	CdialogEvent::Instance()->registerActiveDialog( dynamic_cast<CeventInterface*>(this));

	if (onInit() == false)
	{
		m_running =false;
	}
	for ( a=m_children.firstDialog(); a!=NULL; a=m_children.nextDialog(a))
	{
		Cdialog *t=dynamic_cast<Cdialog*>(a);
		if ( t && t->onInit() ==false)
		{
			m_running =false;
		}
	}
	g_myWorld.unlock();

	if ( m_running )
	{
		onExpose();
	}
	m_touchList.clear();
	startExecute();

	while (m_running)
	{
#ifndef ZHONGCAN_MULTI_THREAD
		CdialogEvent::Instance()->work();
#endif

		bool no_action =true;
		paintAll();
		for ( a=m_children.firstDialog(); a!=NULL; a=m_children.nextDialog(a))
		{
			Cdialog *t=dynamic_cast<Cdialog*>(a);
			if (t==NULL)
			{
				continue;
			}
			if ( t->onLoop()==false)
			{
				m_running =false;
				break;
			}
		}
		if ( m_running ==false)
		{
			break;
		}
		EpollStatus status;
		while ( m_running ==true && (status=CdialogEvent::Instance()->pollEvent( this) )!=POLL_EMPTY)
		{
			no_action =false;
			if ( m_running ==false)
			{
				break;
			}
			int cnt3=0;
			for ( a=g_messageBox.firstDialog(); a !=NULL; a=g_messageBox.nextDialog( a))
			{
				Cdialog *t=dynamic_cast<Cdialog*>(a);
				if (!t)
				{
					continue;
				}
				if (t->onLoop()==false)
				{
					// Close the dialog, not the layers below.
					if ( t->m_alive ==false)
					{
						//Cdialog::onExecute  delete the message box  t->m_name
						invalidate();
						t->unregisterMessageBox();
						delete t;
						break;
					}
				}
				cnt3++;
			}

			if ( status==POLL_TESTING)
			{
				break;
			}
		}
		g_myWorld.lock();
		if ( onLoop() ==false)
		{
			m_running =false;
		}
		if ( m_running ==false)
		{
			g_myWorld.unlock();
			break;
		}
		g_myWorld.unlock();
		if ( m_touchList.update( CdialogEvent::Instance()->pressMouse(), CdialogEvent::Instance()->lastMouse()))
		{
			invalidate();
		}
		if (no_action==true)
		{
			delay(10);
		}
	}
	stopExecute();
	onCleanup();

	for ( a =m_children.firstDialog(); a !=NULL; a =m_children.nextDialog(a))
	{
		Cdialog *t=dynamic_cast<Cdialog*>(a);
		if (t) t->onCleanup();
	}
	m_dragObject =NULL;
	g_myWorld.removeDialog( this);
	CdialogEvent::Instance()->registerActiveDialog( stack);

	// end Cdialog::onExecute: m_name m_exitValue
	return m_exitValue;
}

/** @brief Change rectangle dimensions for a rectangle.
 *  @param rect [in] New size and position.
 */
void Cdialog::setRect( const Crect &rect)
{
	if ( rect.left() !=m_rect.left() || rect.top()!=m_rect.top())
	{
		m_rect.setTop( rect.top());
		m_rect.setLeft( rect.left());
	}
	if ( rect.size() !=m_rect.size())
	{
		m_rect.setSize( rect.size());
	}
}

/** @brief Paint all from this dialog. */
void Cdialog::paintAll()
{
	CdialogBase *a;

	if ( !m_invalidate)
	{
		return;
	}
	g_myWorld.lock();
	CdialogEvent::Instance()->lock();
	onClearScreen();
	if ( m_visible)
	{
		onPaint();
		onPaintButtons();
	}
	for ( a=m_children.firstDialog(); a!=NULL; a=m_children.nextDialog(a))
	{
		CswypeDialog *dialog =dynamic_cast<CswypeDialog*>(a);
		if ( dialog !=NULL)
		{
			dialog->invalidate(-1);
		}
		Cdialog *t=dynamic_cast<Cdialog*>(a);
		if ( t && t->m_visible)
		{
			t->onPaint();
			t->onPaintButtons();
		}
	}

	int cnt=0;
	for ( a=g_messageBox.firstDialog(); a !=NULL; a=g_messageBox.nextDialog( a))
	{
		Cdialog *t=dynamic_cast<Cdialog*>(a);
		if (t->m_visible)
		{
			t->onClearScreen();
			t->onPaint();
			t->onPaintButtons();
		}
		cnt++;
	}
	onDisplay();
	m_invalidate = false;
	CdialogEvent::Instance()->unlock();
	g_myWorld.unlock();
}

/*============================================================================*/
///
/// @brief Paint all grey.
///
/// @post  Ready to paint screen again.
///
/*============================================================================*/
void Cdialog::onClearScreen()
{
	::ColorAT( m_rect.left(), m_rect.top(),
			m_rect.width(), m_rect.height(),
			 m_backgroundColour);
	if ( m_rect.width()==Cgraphics::m_defaults.width/8 &&
		 m_rect.height()==Cgraphics::m_defaults.height)
	{
		m_graphics->image( Cgraphics::m_defaults.full_screen_image_background,
				           m_rect.left(), m_rect.top(),
				m_rect.width(), m_rect.height());
	}
	m_graphics->setCode( m_rect, KEY_NONE);
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
void Cdialog::onPaintButtons()
{
	objectLock(); // Object list should not change.

	for (dialogObjectIterator a=m_objects.begin(); a!=m_objects.end(); ++a)
	{
		CdialogObject *b=*a;

		int level;
		m_touchList.getFactor( b, &level);
		b->onPaint( level);
	}
	objectUnlock(); // Object list should not change end.
}

#define DEBUG_LOCK

#ifdef DEBUG_LOCK
int locked =0;
#endif

/** Cannot remove objects. */
void Cdialog::objectLock()
{
	pthread_mutex_lock( &m_objectMutex);
	m_objectLocked =true;
}

/** Can use objects again. */
void Cdialog::objectUnlock()
{
	pthread_mutex_unlock( &m_objectMutex);
	m_objectLocked =false;
}

/** @brief Set dialog visible or unvisible.
 *  @param visible [in] true when you want to see sth.
 */
void Cdialog::setVisible( bool visible)
{
	assert( (int)pthread_self()==m_main_thread);
	m_visible=visible; invalidate();
}

/*============================================================================*/
///
/// @brief 		Event coming in from SDL.
///
/// @param		event [in] What to do.
///
/// @post       Event handled.
///
/// @return		DIALOG_EVENT_PROCESSED when event is handled.
///
/*============================================================================*/
Estatus Cdialog::onEvent( Cevent &event)
{
	assert( (int)pthread_self()==m_main_thread);
	std::vector<Cdialog*>::iterator a;

	Estatus stat =DIALOG_EVENT_GENERAL;
	if (!m_running)
	{
		// ignore events during closing.
		return stat;
	}
	//Log.write("Cdialog::onEvent");
	switch (event.type)
	{
	case EVENT_MOUSE_CLICK:
	case EVENT_BUTTON:
		tryButton( event.mod, event.button);
		break;
	case EVENT_TOUCH_PRESS: // Just press button in touch-screen.
		touchPress( event.point);
		break;
	case EVENT_TOUCH_RELEASE:
		break;
	case EVENT_TOUCH_MOVE:
		break;
	case EVENT_MOUSE_MOVE:
		onMouseMove( event.point);
		break;
	case EVENT_KEY_PRESS:
		stat =tryButton( event.mod, event.button);
		break;
	case EVENT_DRAG_START:
		try
		{
		m_dragStart =event.point;
		m_dragObject =findObject( event.point);
		if ( m_dragObject)
		{
			if ( m_dragObject->m_dragEnable ==false)
			{
				m_dragObject=NULL;
			}
			else
			{
				// Cdialog::onEvent  drag start
				m_dragOffset =m_dragStart-m_dragObject->m_rect.origin()*8;
			}
		}
		}
		catch(...)
		{
		}
		break;
	case EVENT_DRAG_MOVE:
		if ( m_dragObject !=NULL)
		{
			event.point -=m_dragOffset;
			if ( m_dragPoint.x !=event.point.x || event.point.y!=m_dragPoint.y)
			{
				m_dragPoint =event.point;
				invalidate();
				onDrag( m_dragPoint);
			}
		}
		break;
	case EVENT_DRAG_STOP:
		try
		{
			if ( m_dragObject)
			{
				if ( m_dragObject->onDragEnd( event.point)==true)
				{
					invalidate();
				}
				else
				{
					onDragRelease( dragPoint( event.point), m_dragObject);
				}
			}
		}
		catch (...)
		{
			// Object already removed...
		}
		m_dragObject =NULL;
		break;
	case EVENT_APPMOUSEFOCUS:
		onMouseFocus();
		break;
	case EVENT_APPMOUSEBLUR:
		onMouseBlur();
		break;
	case EVENT_APPINPUTFOCUS:
		onInputFocus();
		break;
	case EVENT_APPINPUTBLUR:
		onInputBlur();
		break;
	case EVENT_RESTORE:
		onRestore();
		break;
	case EVENT_MINIMIZE:
		onMinimize();
		break;
	case EVENT_LANGUAGE_CHANGE:
		Cgraphics::m_defaults.country =Cgraphics::m_defaults.next_language( Cgraphics::m_defaults.country);
		invalidateAll();
		break;
	case EVENT_MAIN_DIALOG:
		stop( -1); // Main dialog
		break;
	default:
	case EVENT_INVALID:
		break;
	}
	return stat;
}

/** @brief Roll the wheel in the middle.
 *  @param mx [in] X-position.
 *  @param my [in] Y-position.
 */
void Cdialog::wheelUp( int mx, int my)
{
	CdialogObject *p =findObject( Cpoint(mx,my));
	if ( p)
	{
		if ( p->wheelUp())
		{
			invalidate();
		}
	}
}

/** @brief Roll the wheel in the middle.
 *  @param mx [in] X-position.
 *  @param my [in] Y-position.
 */
void Cdialog::wheelDown( int mx, int my)
{
	CdialogObject *p =findObject( Cpoint(mx,my));
	if ( p)
	{
		if ( p->wheelDown())
		{
			invalidate();
		}
	}
}

/*============================================================================*/
///
/// @brief 		Focus for input. No use yet.
///
/// @post       Focus here.
///
/*============================================================================*/
void Cdialog::onInputFocus()
{
	//Pure virtual, do nothing
}

/*============================================================================*/
///
/// @brief 		Blur the input keys. Not implemented yet.
///
/// @post       Blur starts for next keys.
///
/*============================================================================*/
void Cdialog::onInputBlur()
{
	//Pure virtual, do nothing
}

/*============================================================================*/
///
///  @brief 	Keyboard handling. Please override function.
///				Handle key events. Also in sub-dialogs for items and menu.
///
///  @param		sym [in] Symbol typed.
///  @param		mod [in] Mode like SHIFT, TAB, CTRL etc.
///
///  @post      Key handled then DIALOG_EVENT_PROCESSED.
///
/*============================================================================*/


/*============================================================================*/
///
/// @brief 		Handle key event released.
///
/// @post       Key event handled.
///
/*============================================================================*/
Estatus Cdialog::onKeyUp(keybutton sym, SDLMod mod)
{
	(void)sym;
	(void)mod;
	//Pure virtual, do nothing
	return DIALOG_EVENT_PROCESSED;
}

/*============================================================================*/
///
/// @brief 		Focus the mouse.
///
/// @post       Mouse at default position.
///
/*============================================================================*/
void Cdialog::onMouseFocus()
{
	//Pure virtual, do nothing
}

/** Drag an object to another location. */
Cpoint Cdialog::dragPoint( const Cpoint &mouse)
{
	Cpoint m=mouse;
	m =m-m_dragOffset;
	int x, y;
	try
	{
		x=Cgraphics::m_defaults.width-(m_dragObject->m_rect.width()*8);
		y=Cgraphics::m_defaults.height-(m_dragObject->m_rect.height()*8);
	}
	catch (...)
	{
		x =m_dragOffset.x;
		y =m_dragOffset.y;
	}
	m.limit( Cpoint( 0,0), Cpoint( x,y));
	return m;
}

/*============================================================================*/
///
/// @brief 		onMouseBlur
///
/// @post       Blur something.
///
/*============================================================================*/
void Cdialog::onMouseBlur()
{
	//Pure virtual, do nothing
}

/*============================================================================*/
///
/// @brief		Mouse is moved event.
///
/// @post       Overload if you need this event.
///
/*============================================================================*/
void Cdialog::onMouseMove( const Cpoint &position)
{
	(void)position;
}

/*============================================================================*/
///
/// @brief 		Mouse wheel turned.
///
/// @post       Wheel message handled.
///
/*============================================================================*/
void Cdialog::onMouseWheel(bool Up, bool Down)
{
	(void)Up;
	(void)Down;
	//Pure virtual, do nothing
}

// @brief A keypress is when the button is released, but sometimes when pressed here.
Estatus Cdialog::onKeyPressed( keybutton sym)
{
	(void)sym;
	return DIALOG_EVENT_OPEN;
}

/** @brief Send a button to all sub-dialogs.
 *  @param mod [in] Shift, control etc.
 *  @param sym [in] What key pressed.
 */
Estatus Cdialog::onButton( SDLMod mod, keybutton sym)
{
	(void)mod;
	(void)sym;
	return DIALOG_EVENT_PROCESSED;
}

/** @brief Send a button to all sub-dialogs.
 *  @param mod [in] Shift, control etc.
 *  @param sym [in] What key pressed.
 */
Estatus Cdialog::tryButton( SDLMod mod, keybutton sym)
{
	assert( (int)pthread_self()==m_main_thread);
	// Also dialogs should be killed in this thread alone!!!!!!

	(void)mod;
	Estatus stat =DIALOG_EVENT_OPEN;
	if ( sym !=KEY_NONE)
	{
		stat =DIALOG_EVENT_GENERAL;
		CdialogBase *a;

		g_messageBox.lock();
		// First try message-boxes
		for ( a=g_messageBox.firstDialog();
			  a !=NULL && stat!=DIALOG_EVENT_PROCESSED && stat!=DIALOG_EVENT_EXIT;
			  a=g_messageBox.nextDialog( a))
		{
			Cdialog *t=dynamic_cast<Cdialog*>(a);
			if (  t->m_visible ==true)
			{
				stat =t->onButton( mod, sym);
				if ( stat==DIALOG_EVENT_EXIT)
				{
					stop(0);
					stat =DIALOG_EVENT_PROCESSED;
				}
			}
		}
		g_messageBox.unlock();

		//g_myWorld.lock();
		// Then try all children.
		for ( a =m_children.lastDialog();
			  a !=NULL && stat!=DIALOG_EVENT_PROCESSED && stat!=DIALOG_EVENT_EXIT;
			  a =m_children.previousDialog( a))
		{
			Cdialog *t=dynamic_cast<Cdialog*>(a);
			if (  t->m_visible ==true)
			{
				stat =t->onButton( mod, sym);
				if ( stat==DIALOG_EVENT_EXIT)
				{
					stop(0);
					stat =DIALOG_EVENT_PROCESSED;
				}
			}
		}
		// Then try myself.
		if ( stat !=DIALOG_EVENT_PROCESSED && stat!=DIALOG_EVENT_EXIT)
		{
			stat =onButton( mod, sym);
			if ( stat==DIALOG_EVENT_EXIT)
			{
				stop(0);
				stat =DIALOG_EVENT_PROCESSED;
			}
		}
		//g_myWorld.unlock();
	}
	return stat;
}

void Cdialog::onStopDrag()
{
}

/*============================================================================*/
///
/// @brief 		Minimize event.
///
/// @post       Event handled.
///
/*============================================================================*/
void Cdialog::onMinimize()
{
	//Pure virtual, do nothing
}

/*============================================================================*/
///
/// @brief 		Restore.
///
/// @post       REstore event handled.
///
/*============================================================================*/
void Cdialog::onRestore()
{
	//Pure virtual, do nothing
}

/*============================================================================*/
///
/// @brief 		Resize the dialog.
///
/// @post       Window resized.
///
/*============================================================================*/
void Cdialog::onResize(int w, int h)
{
	(void)h;
	(void)w;
	//Pure virtual, do nothing
}

/*============================================================================*/
///
/// @brief 		First time display function before the real paint is done.
///
/// @post       First draw is done, e.g. a background.
///
/*============================================================================*/
void Cdialog::onExpose()
{
	//Pure virtual, do nothing
}

/*============================================================================*/
///
/// @brief 		Exit function for dialog.
///
/// @post       Dialog will not destroy, but closer. Lower layer will be visible.
///
/// @remarks    DEBUGGED
///
/*============================================================================*/
void Cdialog::onExit()
{
}

/** @brief Callback to tell something is pressed, remember this for live colours */
void Cdialog::notifyTouch( const Cpoint &p)
{
	Cpoint q=p/8;
	Cdialog *dialog =findDialog( q);
	if ( dialog)
	{
		CdialogObject *object =dialog->findObject( p);
		if ( object)
		{
			dialog->m_touchList.addObject( object);
		}
	}
}

void Cdialog::insertDragObject()
{
	// Now put on the display.
	if ( m_dragObject !=NULL && m_dragObject->m_graphics !=NULL)
	{
		m_dragObject->m_graphics->lock_keycodes();
		m_dragObject->onPaint( m_dragPoint.div8(),0);
		m_dragObject->m_graphics->unlock_keycodes();
	}

}
/** @brief Paint the display after all buttons are painted with onPaint() */
void Cdialog::onDisplay()
{
	assert( (int)pthread_self()==m_main_thread);
	try
	{
		// Now put on the display.
		insertDragObject();
		m_graphics->update();
	}
	catch (...)
	{
	}
	try
	{
		if (m_dragObject !=NULL && m_dragObject->m_graphics !=NULL)
		{
			m_dragObject->m_graphics->unlock_keycodes();
		}
	}
	catch(...)
	{

	}
	m_graphics->update();
}

/*============================================================================*/
///
/// @brief 		Add a message box to our system.
///
/// @post       Message box added, but not ready to display.
///
/*============================================================================*/
void Cdialog::registerMessageBox(Cdialog *child)
{
    g_messageBox.addDialog( child);
	invalidate();
}

/*============================================================================*/
///
/// @brief 		Remove a message box to our system.
///
/// @post       Message box removed, should not be displayed anymore.
///
/*============================================================================*/
void Cdialog::unregisterMessageBox(Cdialog *child)
{
	g_messageBox.removeDialog( child);
	invalidate();
}

/*============================================================================*/
///
/// @brief 		I am the messagebox and want to register.
///
/// @post       Message box removed, should not be displayed anymore.
///
/*============================================================================*/
void Cdialog::registerMessageBox()
{
	m_isMessageBox=true;
	Cdialog::registerMessageBox(this);
}

/*============================================================================*/
///
/// @brief 		I am the messagebox and want to unregister.
///
/// @post       Message box removed, should not be displayed anymore.
///
/*============================================================================*/
void Cdialog::unregisterMessageBox()
{
	Cdialog::unregisterMessageBox(this);
}

/** @brief Find object for a certain point. Not in sub-dialogs!
 *  @param p [in] Object point on display.
 *  @return Dialog object found.
 */
CdialogObject * Cdialog::findObject( const Cpoint &p)
{
	Cpoint p2=p/8;

	// Find which dialog should we search in.
	Cdialog *dialog =findDialog( p2);

	if ( !dialog)
	{
		return NULL;
	}
	if ( dialog !=this)
	{
		return dialog->findObject( p);
	}
	CdialogObject *retVal =NULL;

	objectLock(); // Object list should not change.

	for (dialogObjectReverseIterator di=m_objects.rbegin();
			di!=m_objects.rend(); ++di)
	{
		CdialogObject *cd =*di;
		if ( cd->m_visible ==true && cd->m_rect.inside( p2))
		{
			retVal =cd;
			break;
		}
	}
	objectUnlock(); // Object list should not change.
	return retVal;
}


/*============================================================================*/
///
/// @brief 		Which dialog is at a certain co-ordinate?
/// @param      p [in] Location to find.
/// @post       Tell which dialog. Called from all threads.
///
/*============================================================================*/
Cdialog *Cdialog::findDialog( const Cpoint &p)
{
	Cdialog *retVal =NULL;
	CdialogBase *a;

	g_messageBox.lock();
	for ( a=g_messageBox.lastDialog(); a!=NULL; a=g_messageBox.previousDialog( a))
	{
		Cdialog *t=dynamic_cast<Cdialog*>(a);
		if ( t==NULL)
		{
			continue;
		}
		if (  t->m_visible ==false)
		{
			continue;
		}
		if ( t->m_rect.inside( p))
		{
			retVal =t;
			break;
		}
		if (p.x==0 && p.y==0)
		{
			retVal =t;
			break;
		}
	}
	g_messageBox.unlock();
	if ( retVal ==NULL)
	{
		g_myWorld.lock();
		for ( a=m_children.lastDialog(); a !=NULL; a=m_children.previousDialog( a))
		{
			Cdialog *t=dynamic_cast<Cdialog*>(a);
			if ( t==NULL)
			{
				continue;
			}
			if ( t->m_visible ==false)
			{
				continue;
			}
			if ( t->m_rect.inside( p) ==true)
			{
				retVal =t;
				break;
			}
			if ( p.x==0 && p.y==0)
			{
				retVal =t;
				break;
			}
		}
		g_myWorld.unlock();
	}
	if ( retVal ==NULL)
	{
		retVal =this;
	}
	return retVal;
}

/** For debugging the application. */
void Cdialog::idiotTest()
{
	CdialogEvent::Instance()->m_idiot =true;
}

/** @brief We start to drag an object.
 *  @param movingObject [in] Which object we move.
 */
void Cdialog::onStartDrag( CdialogObject * movingObject)
{
	(void)movingObject;
}

/** @brief Overload function to drag object to other location.
 *  @param movingObject [in] What object is moved.
 *  @param to [in] Where we release the mouse. Left top corner of item.
 */
void Cdialog::dragObject( CdialogObject * movingObject, const Cpoint &to)
{
	(void)movingObject;
	(void)to;
}

/** Check if we have a scroll dialog which can drag.
 *  @param p [in] Point for mouse.
 *  @return true for a dragging window.
 */
bool Cdialog::isScrollDragDialog( const Cpoint &p)
{
	Cdialog *dialog =findDialog( p);
	CswypeDialog *scroll =dynamic_cast<CswypeDialog*>(dialog);
	if (scroll)
	{
		return scroll->isScrollDragDialog( p);
	}
	return false;
}

/** Check if it is an horizontal scroll dialog.
 *  @param p [in] Mouse position.
 *  @return true for a horizontal scroll dialog.
 */
bool Cdialog::isHorizontalScrollDialog( const Cpoint &p)
{
	Cdialog *dialog =findDialog( p);
	CswypeDialog *scroll =dynamic_cast<CswypeDialog*>(dialog);
	if (scroll)
	{
		return scroll->isHorizontalScrollDialog( p);
	}
	return false;
}

/** @brief Find which button at input location. Run from any thread.
 *  @param p [in] What position to find for.
 *  @return Key pressed with touch/mouse.
 */
keybutton Cdialog::findButton( const Cpoint &p)
{
	keybutton key =KEY_NONE;
	Cpoint q=p/8;
	Cdialog *dialog =findDialog( q);
	if ( dialog !=NULL)
	{
		CswypeDialog *scroll =dynamic_cast<CswypeDialog*>(dialog);
		if ( scroll)
		{
			key =scroll->findButton( p);
		}
		else
		{
			key =m_graphics->m_touch.getKey( q);
		}
	}
	return key;
}

Cpoint Cdialog::getTouchPosition()
{
	return CdialogEvent::Instance()->lastMouse();
}

/** @brief Just check if we have a scrolling window inside.
 *  Check for scrolling window from other thread, be aware!!
 *  @param p [in] Position to check.
 */
bool Cdialog::isSwypeDialog( const Cpoint &p)
{
	Cpoint q=p/8;
	CswypeDialog *dialog =dynamic_cast<CswypeDialog*>( findDialog( q) );
	return dialog ? true:false;
}

/** @brief Scroll a dialog to a certain point.
 *  @param start [in] Start point for the scroll.
 *  @param distance [in] Distance to scroll to.
 */
void Cdialog::scrollDialog( const Cpoint &start, const Cpoint &distance)
{
	//assert( (int)pthread_self()==m_main_thread);

	g_myWorld.lock();
	CswypeDialog *dialog =dynamic_cast<CswypeDialog*>( findDialog( start/8) );
	if ( dialog)
	{
		dialog->scrollRelative( dialog->m_horizontal ? (double)distance.x:(double)distance.y, true);
	}
	g_myWorld.unlock();
}

/** Overload to do some with drag release. */
void Cdialog::onDragRelease( const Cpoint &position, CdialogObject * movingObject)
{
	(void)position;
	(void)movingObject;
}

/** At start of executing the dialog */
void Cdialog::startExecute()
{
	//int n=pthread_mutex_trylock( &m_dlgMutex); //EBUSY);
	//m_prevLock =( n!=0);
	//pthread_mutex_unlock( &m_dlgMutex);
}

/** At stop of executing the dialog */
void Cdialog::stopExecute()
{
	//if ( m_prevLock ==true)
	//{
	//	m_prevLock =false;
	//	pthread_mutex_lock( &m_dlgMutex);
	//}
}
