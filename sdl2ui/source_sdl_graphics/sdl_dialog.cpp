/*============================================================================*/
/**  @file       sdl_dialog.cpp
 **  @ingroup    sdl2ui
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
#include <stdio.h>
#include "sdl_dialog.h"
#include "sdl_swype_dialog.h"
#include "timeout.h"
#include "sdl_dialog_event.h"
#include "sdl_dialog_list.h"
#include "sdl_label.h"


pthread_mutex_t Cdialog::m_objectMutex;
bool Cdialog::m_objectLocked =false;
pthread_mutexattr_t Cdialog::m_attr;
bool Cdialog::m_useClick =true;
Iworld *Cdialog::g_defaultWorld = NULL;


#define START_FUNC
#define END_FUNC

/*============================================================================*/
///
/// @brief Constructor for dialog in the world.
///
/*============================================================================*/
Cdialog::Cdialog( Cdialog *parent, const std::string &name, const Crect &rect, Iworld *world)
: m_world(parent ? parent->m_world:NULL)
, m_alive(true)
, m_allButtons(false)
, m_backgroundColour( Cgraphics::m_defaults.background)
, m_current_thread(0)
, m_exitValue(0)
, m_in_main_thread(false)
, m_parent(parent)
, m_spaceIsLanguage(true)
, m_visible(true)
, m_invalidate(true)
, m_isMessageBox(false)
, m_running(false)
, m_root(this)
, m_rect( rect)
, m_squares_width( Cgraphics::m_defaults.width/8)
, m_squares_height( Cgraphics::m_defaults.height/8)
, m_dialogOffset(0,0)
#ifdef USE_SDL2
, m_graphics(NULL)
, m_myGraphics(NULL)
, m_render_after_paint(true)
, m_full_screen(rect.width() == m_squares_width && rect.height() == m_squares_height)
#else
, m_graphics( (parent==NULL || parent->m_graphics==NULL) ? m_mainGraph:parent->m_graphics)
#endif
, m_selfDestruct(false)
{
	if ( !m_world)
	{
		m_world = world ? world:g_defaultWorld;
	}
	if (g_defaultWorld == NULL)
	{
		g_defaultWorld = m_world;
	}
	if (rect.width()==0 && rect.height()==0)
	{
		m_rect =Crect(0,0, Cgraphics::m_defaults.width/8, Cgraphics::m_defaults.height/8);
		m_full_screen =true;
	}

	m_world->lock();
	m_name =name;
	m_current_thread =pthread_self();
	// Dialog initialised
	m_world->unlock();
}

/*============================================================================*/
///
/// @brief Destructor
///
/*============================================================================*/
Cdialog::~Cdialog()
{
#ifdef USE_SDL2
#endif

}

/** @brief Register a new child to handle for a dialog.
 *  @param child [in] New child.
 */
void Cdialog::registerChild(Cdialog *child)
{
	m_world->checkInMainThread();
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
	m_world->checkInMainThread();
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
void Cdialog::invalidate( bool needs_repaint)
{
	m_world->lock();
	m_invalidate =needs_repaint;
	m_world->unlock();
}

/*============================================================================*/
///
/// @brief 		Invalidate GUI. This will be re-painted.
///
/// @post       Class removed. Please also remove all references.
///
/*============================================================================*/
bool Cdialog::isInvalidated()
{
	bool retVal;
	m_world->lock();
	retVal =m_invalidate;
	m_world->unlock();
	return retVal;
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
	m_world->checkInMainThread();
	m_world->invalidateAll();
	CdialogBase *a;
	m_world->lock();
	for ( a =m_children.firstDialog(); a !=NULL; a =m_children.nextDialog( a))
	{
		CswypeDialog *dialog =dynamic_cast<CswypeDialog*>( a);
		if ( dialog !=NULL)
		{
			dialog->resetPaintedArea();
		}
	}
	m_invalidate =true;
	m_world->unlock();
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
	m_world->checkInMainThread();
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
	m_world->checkInMainThread();
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
	bool started=false;
	(void)parent;
	m_world->checkInMainThread();
	CdialogBase* a;

	m_exitValue =0;
	m_running =true;
	if (m_full_screen)
	{
		m_world->setActiveDialog(this);
	}
#ifdef USE_SDL2
	if (!m_graphics && !m_full_screen)
	{
		m_graphics = std::make_shared<Cgraphics>(m_rect.size(), false);
		m_graphics->init();
	}
	if (!m_graphics)
	{
		m_graphics = m_world->graphics();
	}
#endif
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

	if ( m_running )
	{
		onExpose();
	}
	m_touchList.clear();
	startExecute();

	while (m_running)
	{
		started=true;
#ifndef ZHONGCAN_MULTI_THREAD
		CdialogEvent::Instance()->work();
#endif
		m_world->paintAll();
		bool no_action = m_world->onLoop();

		if ( m_running ==false)
		{
			m_world->unlock();
			break;
		}
		m_world->unlock();
		if ( m_touchList.update( CdialogEvent::Instance()->pressMouse(), CdialogEvent::Instance()->lastMouse()))
		{
			invalidate();
		}
		if (no_action==true)
		{
			delay(10);
		}
	}
	if (started)
	{
		m_world->paintAll();
		m_touchList.update( CdialogEvent::Instance()->pressMouse(), CdialogEvent::Instance()->lastMouse());
		delay(5);
	}
	stopExecute();
	m_world->onCleanup();

	for ( a =m_children.firstDialog(); a !=NULL; a =m_children.nextDialog(a))
	{
		Cdialog *t=dynamic_cast<Cdialog*>(a);
		if (t) t->onCleanup();
	}
	onCleanup();
	m_dragObject.clean();
	m_world->setActiveDialog(NULL);
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

/*============================================================================*/
///
/// @brief Paint all grey.
///
/// @post  Ready to paint screen again.
///
/*============================================================================*/
void Cdialog::onClearScreen()
{
    bool fullscreen = ( m_rect.width()==Cgraphics::m_defaults.width/8 &&
			 m_rect.height()==Cgraphics::m_defaults.height/8);

    m_graphics->setColour( m_backgroundColour );
#ifdef USE_SDL2
	if (fullscreen)
	{
		m_world->graphics()->cleardevice();
 		m_world->graphics()->image( Cgraphics::m_defaults.full_screen_image_background,
				           m_rect.left()*8, m_rect.top()*8,
			               m_rect.width()*8, m_rect.height()*8);
	}
	else
	{
		m_world->graphics()->bar( m_rect.left()*8, m_rect.top()*8, m_rect.right()*8, m_rect.bottom()*8,0);
	}
#else
	m_world->graphics()->bar( m_rect.left()*8, m_rect.top()*8, m_rect.right()*8, m_rect.bottom()*8,0);
	if ( fullscreen )
	{
		m_graphics->image( Cgraphics::m_defaults.full_screen_image_background,
				           m_rect.left()*8, m_rect.top()*8,
				m_rect.width()*8, m_rect.height()*8);
	}
#endif
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
	m_world->checkInMainThread();
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
Estatus Cdialog::onEvent( const Cevent &event)
{
	m_world->checkInMainThread();
	std::vector<Cdialog*>::iterator a;

	if ( m_touchList.update( CdialogEvent::Instance()->pressMouse(), CdialogEvent::Instance()->lastMouse()))
	{
		invalidate();
		m_world->paintAll();
	}

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
	case EVENT_TOUCH_LONG:
		touchLong( event.point, event.button);
		break;
	case EVENT_MOUSE_MOVE:
		onMouseMove( event.point);
		break;
	case EVENT_KEY_PRESS:
		stat =tryButton( event.mod, event.button);
		break;
	case EVENT_DRAG_START:
		m_dragObject.start(event.point, findObject( event.point));
		break;
	case EVENT_DRAG_MOVE:
		if (m_dragObject.moveTo(event.point))
		{
			m_world->notifyInvalidate();
			onDrag(m_dragObject.getDragPoint());
		}
		break;
	case EVENT_DRAG_STOP:
		try
		{
			if ( !m_dragObject.isEmpty())
			{
				if ( m_dragObject.stop( event.point)==true)
				{
					invalidate();
				}
				else
				{
					onDragRelease( m_dragObject.getTopLeft( event.point), m_dragObject.m_dragObject);
				}
			}
		}
		catch (...)
		{
			// Object already removed...
		}
		m_dragObject.clean();
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
		if ( Cgraphics::m_defaults.next_language)
		{
			Cgraphics::m_defaults.country =Cgraphics::m_defaults.next_language( Cgraphics::m_defaults.country);
			invalidateAll();
		}
		break;
	case EVENT_MAIN_DIALOG:
		if ( Cgraphics::m_defaults.start_from_main)
		{
			Cgraphics::m_defaults.start_from_main( this); // Main dialog
		}
		break;
	case EVENT_PAINT_START:
		onPaintingStart( event.point);
		//invalidate();
		break;
	case EVENT_PAINT_MOVE:
		onPaintingMove( event.point);
		//invalidate();
		break;
	case EVENT_PAINT_STOP:
		onPaintingStop( event.point);
		//invalidate();
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
Estatus Cdialog::onKeyUp(keybutton sym, keymode mod)
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
	return m_dragObject.dragTo( mouse);
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
Estatus Cdialog::tryButton( keymode mod, keybutton sym)
{
	m_world->checkInMainThread();
	// Also dialogs should be killed in this thread alone!!!!!!
	Estatus stat =DIALOG_EVENT_OPEN;
	(void)mod;

	if (sym != KEY_NONE)
	{
		stat = m_world->tryButton(mod,sym);
		CdialogBase *a;

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
	if ( !m_dragObject.isEmpty() && m_dragObject.m_dragObject->m_graphics !=NULL)
	{
		m_dragObject.onPaint();
	}
}
/** @brief Paint the display after all buttons are painted with onPaint() */
void Cdialog::onDisplay()
{
	m_world->checkInMainThread();

	try
	{
		// Now put on the display.
		insertDragObject();
		//m_graphics->update();
	}
	catch (...)
	{
	}
	try
	{
		if ( !m_dragObject.isEmpty() && m_dragObject.m_dragObject->m_graphics !=NULL)
		{
			m_dragObject.m_dragObject->m_graphics->unlock_keycodes();
		}
	}
	catch(...)
	{

	}
	m_graphics->update();
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
	Cdialog *retVal =m_world->findDialog(p);
	CdialogBase *a;

	if ( retVal ==NULL)
	{
		m_world->lock();
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
		m_world->unlock();
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
		if ( !scroll)
		{
			key =m_graphics->m_touch.getKey( q);
		}
		else
		{
			key =scroll->findButton( p);
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
	m_world->lock();
	CswypeDialog *dialog =dynamic_cast<CswypeDialog*>( findDialog( start/8) );
	if ( dialog)
	{
		dialog->scrollRelative( dialog->m_horizontal ? (double)distance.x:(double)distance.y, true);
	}
	m_world->unlock();
}

/** Overload to do some with drag release. */
void Cdialog::onDragRelease( const Cpoint &position, CdialogObject * movingObject)
{
	(void)position;
	(void)movingObject;
}

/** Create my own graphic layer, so we don't need to repaint everything each time */
void Cdialog::createMyGraph()
{
	m_myGraphics =std::shared_ptr<Cgraphics>( new Cgraphics( m_rect.size(), false) );
}

/** Overload to do some with drag release. */
void Cdialog::onPaintingStart( const Cpoint &position)
{
	try
	{
		CdialogObject *object =findObject( position);
		if ( object)
		{
			object->onPaintingStart( position);
		}
	}
	catch (...)
	{
	}
}

/** Overload to do some with drag release. */
void Cdialog::onPaintingMove( const Cpoint &position)
{
	try
	{
		CdialogObject *object =findObject( position);
		if ( object)
		{
			object->onPaintingMove( position);
		}
	}
	catch (...)
	{
	}
}

/** Overload to do some with drag release. */
void Cdialog::onPaintingStop( const Cpoint &position)
{
	try
	{
		CdialogObject *object =findObject( position);
		if ( object)
		{
			object->onPaintingStop( position);
		}
	}
	catch (...)
	{
	}
}

/** At start of executing the dialog */
void Cdialog::startExecute()
{
	//int n=pthread_mutex_trylock( &m_dlgMutex); //EBUSY);
	//m_prevLock =( n!=0);
	//pthread_mutex_unlock( &m_dlgMutex);
}

/** Render to the output graph */
void Cdialog::onRender()
{
	if (m_myGraphics.get() !=NULL)
	{
		m_graphics->renderGraphics(m_myGraphics.get(), m_rect*8);
	}
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
