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
#include <stdio.h>
#include <assert.h>

#include "lingual.h"
#include "sdl_label.h"
#include "timeout.hpp"
#include "sdl_dialog.h"
#include "sdl_dialog_list.h"
#include "sdl_swype_dialog.h"
#include "sdl_dialog_event.h"

int Cdialog::m_init = 0;
std::atomic<bool> Cdialog::m_invalidate(true);
//pthread_mutex_t Cdialog::m_objectMutex;
//bool Cdialog::m_objectLocked =false;
//pthread_mutexattr_t Cdialog::m_attr;
int Cdialog::m_main_thread =0;
bool Cdialog::m_useClick =true;
CkeyFile Cdialog::m_keyFile;
CdragObject Cdialog::m_dragObject;		///< Object to drag.
CdialogList Cdialog::m_messageBox;

#define START_FUNC
#define END_FUNC



/*============================================================================*/
///
/// @brief Constructor.
///
/*============================================================================*/
Cdialog::Cdialog( Cdialog *parent, const std::string &name, const Crect &rect)
: m_alive(true)
, m_allButtons(false)
, m_backgroundColour( Cgraphics::m_defaults.background)
, m_current_thread(0)
, m_exitValue(0)
, m_in_main_thread(false)
, m_parent(parent)
, m_spaceIsLanguage(true)
, m_visible(true)
, m_isChild(false)
, m_isMessageBox(false)
, m_running(false)
, m_root(this)
, m_rect( rect)
, m_squares_width( Cgraphics::m_defaults.width/8)
, m_squares_height( Cgraphics::m_defaults.height/8)
, m_dialogOffset(0,0)
, m_graphics( (parent==NULL || parent->m_graphics==NULL) ? m_mainGraph:parent->m_graphics)
, m_isOwner(false)
, m_selfDestruct(false)
{
    lock();

	if (rect.width()==0 && rect.height()==0)
	{
		m_rect =Crect(0,0, Cgraphics::m_defaults.width/8, Cgraphics::m_defaults.height/8);
	}
	if (!m_init)
	{
		m_main_thread =pthread_self();
		m_keyFile.init();
	}
	m_init++;
	m_name =name;
	m_current_thread =pthread_self();
	m_in_main_thread =(m_main_thread ==m_current_thread);

	unlock();
}

/*============================================================================*/
///
/// @brief Destructor
///
/*============================================================================*/
Cdialog::~Cdialog()
{
}

/*----------------------------------------------------------------------------*/
/** @brief Register a new child to handle for a dialog.
 *  @param child [in] New child.
 */
void Cdialog::registerChild(Cdialog *child)
{
	assert( (int)pthread_self()==m_main_thread);
	m_children.addDialog( child);
	child->setChild();
}

/*----------------------------------------------------------------------------*/
/** @brief Register a new child to handle for a dialog.
 *  @param child [in] New child.
 */
void Cdialog::registerChild(CdialogPtr child)
{
    assert( (int)pthread_self()==m_main_thread);
    m_children.addDialog( child);
}

/*----------------------------------------------------------------------------*/
/** @brief Unregister a new child to handle for a dialog.
 *  @param child [in] New child.
 */
void Cdialog::unregisterChild(Cdialog *child)
{
	m_children.removeDialog( child);
}

/*----------------------------------------------------------------------------*/
/** @brief Unregister a new child to handle for a dialog.
 *  @param child [in] New child.
 */
void Cdialog::unregisterChild(CdialogPtr child)
{
	m_children.removeDialog( child);
}

/*----------------------------------------------------------------------------*/
/** @brief Unregister an object to handle for a dialog.
 *  @param child [in] New child.
 */
void Cdialog::unregisterObject(CdialogObjectPtr child)
{
	lock(); // Object list should not change.

	for (std::vector<CdialogObjectPtr>::iterator i=m_objects.begin();
		 i!=m_objects.end(); ++i)
	{
		if (*i==child)
		{
			m_objects.erase(i);
			//Log.write("Cdialog::unregisterObject:%s now has lost child object %s", m_name.c_str(), child->m_name.c_str());
			break;
		}
	}
	unlock(); // Object list should not change end.
}

/*----------------------------------------------------------------------------*/
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
void Cdialog::registerObject(CdialogObjectPtr child)
{
	lock();

	if (!child.get())
	{
	    Cgraphics::m_defaults.log("Cdialog::registerObject ERROR Zero pointer found!!");
	}
	bool found =false;
	for (size_t i=0; i<m_objects.size(); ++i)
	{
		if (m_objects[i] == child)
		{
			found =true;
			break;
		}
	}
	if (!found)
	{
		m_objects.push_back(child);
	}

	unlock();
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
	m_invalidate =true;
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
    lock();

    assert( (int)pthread_self()==m_main_thread);
	m_children.resetPaintedArea();
	m_invalidate =true;

	unlock();
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
	if (!m_children.onLoop())
	{
	    m_running = false;
	}
	return m_alive;
}

/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
void Cdialog::onBarcode( const std::string &barcode)
{
	(void)barcode;
}

/*----------------------------------------------------------------------------*/
void Cdialog::makeValid()
{
	m_invalidate = false;
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
	assert( (int)pthread_self()==m_main_thread);

	m_exitValue =0;
	m_running =true;
	//g_myWorld.addDialog( this);
	lock();

	CeventInterface *stack =CdialogEvent::Instance()->getInterface();
	CdialogEvent::Instance()->registerActiveDialog( dynamic_cast<CeventInterface*>(this));

	if (onInit() == false || m_children.onInit() == false)
	{
	    m_running = false;
	}
	unlock();

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
		bool no_action =true;
		if (m_invalidate)
		{
		    paintAll();
		    if (!m_isChild)
		    {
		    	m_invalidate = false;
		    }
		}
		if ( Cgraphics::m_defaults.external_loop )
		{
			Cgraphics::m_defaults.external_loop();
		}
		if (!m_children.onLoop())
		{
		    m_running = false;
            break;
		}
		EpollStatus status;
		int n=10;
		while ( m_running ==true && (status=CdialogEvent::Instance()->pollEvent( this) )!=POLL_EMPTY)
		{
			if ( !--n) { paintAll(); n=10; }
			no_action =false;
			if ( m_running ==false)
			{
				break;
			}
			if (m_messageBox.onLoopWithSelfDestruct() ==true)
			{
                invalidate();
			}

			if ( status==POLL_TESTING)
			{
				break;
			}
		}
		lock();
		if ( onLoop() ==false)
		{
			m_running =false;
		}
		if ( m_running ==false)
		{
			//unlock();
			break;
		}
		unlock();
		if ( m_touchList.update( CdialogEvent::Instance()->isMousePressed(),
		                         CdialogEvent::Instance()->lastMouse()))
		{
			invalidate();
		}
		if (no_action==true)
		{
		    if (m_messageBox.onLoopWithSelfDestruct() ==true)
		    {
		        invalidate();
	            delay(40);
		    }
		}
	}
	if (started)
	{
		paintAll();
		m_touchList.update( CdialogEvent::Instance()->isMousePressed(),
		                    CdialogEvent::Instance()->lastMouse());
		delay(5);
	}
	stopExecute();
	onCleanup();

	m_children.onCleanup();
	m_dragObject.clean();
	//m_myWorld.removeDialog( this);
	CdialogEvent::Instance()->registerActiveDialog( stack);

	// end Cdialog::onExecute: m_name m_exitValue
	return m_exitValue;
}

/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
void Cdialog::paintCoordinates( bool update)
{
	if ( Cgraphics::m_defaults.debug_coordinates)
	{
		char s[24];
		Cpoint g(CdialogEvent::Instance()->m_debugPosition);
		sprintf(s, " %d %d", g.x, g.y);
		Clabel p(this, Crect(0,0,15,3), KEY_NOCHANGE);
		std::string tt(s);
		p.setText(tt);
		p.onPaint(0);
		m_graphics->setColour(0xffffff);
		Cpoint h(CdialogEvent::Instance()->getMousePosition());
		m_graphics->line(h.x-30, h.y-30, h.x+30,h.y+30);
		m_graphics->line(h.x+30, h.y-30, h.x-30,h.y+30);
		if (update)
		{
			m_graphics->update(0,0,15*8,3*8);
			delay(2);
		}
	}
}

/*----------------------------------------------------------------------------*/
/** @brief Paint all from this dialog. */
void Cdialog::paintAll()
{
	if ( !m_invalidate)
	{
		paintCoordinates( true);
		//return;
	}
	lock();
	m_mainGraph->setPixelOffset( m_dialogOffset.x, m_dialogOffset.y);
	CdialogEvent::Instance()->lock();
	onClearScreen();
	if ( m_visible)
	{
		onPaint();
		onPaintButtons();
	}
	m_children.invalidateSwypeDialogs();
	m_children.onPaint();
	m_messageBox.clearScreenAndPaint();
	paintCoordinates( false);
	onDisplay();
	if (!m_isChild && m_invalidate)
	{
		m_invalidate = false;
	}
	m_mainGraph->setPixelOffset(0,0);
	CdialogEvent::Instance()->unlock();
	unlock();
}

/*----------------------------------------------------------------------------*/
/** @brief Paint all from this dialog. */
void Cdialog::paintAllDark()
{
	lock();
	m_mainGraph->setPixelOffset( m_dialogOffset.x, m_dialogOffset.y);
	CdialogEvent::Instance()->lock();
	onClearScreen();
	if ( m_visible)
	{
		onPaint();
		onPaintButtons();
	}
	m_children.invalidateSwypeDialogs();
	m_children.onPaint();
	m_messageBox.clearScreenAndPaint();
	paintCoordinates( false);
    if (!m_isChild && m_invalidate)
    {
    	m_invalidate = false;
    }
	m_mainGraph->setPixelOffset(0,0);
	CdialogEvent::Instance()->unlock();
    darken(0);
	unlock();
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
	m_mainGraph->setColour( m_backgroundColour );
	m_mainGraph->bar( m_rect.left()*8, m_rect.top()*8, m_rect.right()*8, m_rect.bottom()*8,0);
	if ( m_rect.width()==Cgraphics::m_defaults.width/8 &&
		 m_rect.height()==Cgraphics::m_defaults.height/8)
	{
		m_graphics->image( Cgraphics::m_defaults.full_screen_image_background,
				           m_rect.left()*8, m_rect.top()*8,
				m_rect.width()*8, m_rect.height()*8);
	}
	m_graphics->setKey( m_rect, KEY_NONE);
	m_children.onClearScreen();
}

/*----------------------------------------------------------------------------*/
bool Cdialog::isVisible() const
{
	return m_visible;
}

/*----------------------------------------------------------------------------*/
int Cdialog::width() const
{
	return m_rect.width();
}

/*----------------------------------------------------------------------------*/
int Cdialog::height() const
{
	return m_rect.height();
}

/*----------------------------------------------------------------------------*/
Crect Cdialog::getRect() const
{
	return m_rect;
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
	lock(); // Object list should not change.

	for (CdialogObjectPtr b : m_objects)
	{
		int level;
		m_touchList.getFactor( b, &level);
		b->onPaint( level);
	}
	unlock(); // Object list should not change end.
}

/*----------------------------------------------------------------------------*/
bool Cdialog::inside( const Cpoint &point) const
{
	return m_rect.inside(point);
}

/*----------------------------------------------------------------------------*/
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
Estatus Cdialog::onEvent( const Cevent &event)
{
	assert( (int)pthread_self()==m_main_thread);
	std::vector<Cdialog*>::iterator a;

	if ( m_touchList.update( CdialogEvent::Instance()->isMousePressed(),
	                         CdialogEvent::Instance()->lastMouse()))
	{
		invalidate();
		paintAll();
	}

	Estatus stat =DIALOG_EVENT_GENERAL;
	if (!m_running)
	{
		// ignore events during closing.
		return stat;
	}
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
		m_dragObject.start( event.point, findObject( event.point));
		break;
	case EVENT_DRAG_MOVE:
		if ( m_dragObject.moveTo( event.point))
		{
			onDrag( m_dragObject.getDragPoint());
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
	    setLanguage( ChangeNextLanguage( getLanguage()));
        invalidateAll();
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

/*----------------------------------------------------------------------------*/
/** @brief Roll the wheel in the middle.
 *  @param mx [in] X-position.
 *  @param my [in] Y-position.
 */
void Cdialog::wheelUp( int mx, int my)
{
	CdialogObjectPtr p =findObject( Cpoint(mx,my));
	if ( p)
	{
		if ( p->wheelUp())
		{
			invalidate();
		}
	}
}

/*----------------------------------------------------------------------------*/
/** @brief Roll the wheel in the middle.
 *  @param mx [in] X-position.
 *  @param my [in] Y-position.
 */
void Cdialog::wheelDown( int mx, int my)
{
	CdialogObjectPtr p =findObject( Cpoint(mx,my));
	if ( p.get())
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

/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
// @brief A keypress is when the button is released, but sometimes when pressed here.
Estatus Cdialog::onKeyPressed( keybutton sym)
{
	(void)sym;
	return DIALOG_EVENT_OPEN;
}

/*----------------------------------------------------------------------------*/
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

        // First try message-boxes
		lock();
		stat =m_messageBox.onButton(mod, sym, this, stat);
		unlock();

		// Then try all children.
		stat = m_children.onButton(mod, sym, this, stat);

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
	}
	return stat;
}

/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/** @brief Callback to tell something is pressed, remember this for live colours */
void Cdialog::notifyTouch( const Cpoint &p)
{
	Cpoint q=p/8;
	Cdialog *dialog =findDialog( q);
	if ( dialog)
	{
		CdialogObjectPtr object =dialog->findObject( p);
		if ( object.get())
		{
			dialog->m_touchList.addObject( object);
		}
	}
}

/*----------------------------------------------------------------------------*/
void Cdialog::insertDragObject()
{
	// Now put on the display.
	if ( !m_dragObject.isEmpty() && m_dragObject.m_dragObject->hasGraphics())
	{
		Cgraphics *graph =m_graphics;
		m_graphics =m_mainGraph;
		m_dragObject.onPaint();
		m_graphics =graph;
	}
}

/*----------------------------------------------------------------------------*/
/** Darken the screen first
 */
void Cdialog::darken( int bottomMargin)
{
	m_mainGraph->darken(0,0, m_squares_width*8, (m_squares_height-bottomMargin)*8);
}

/*----------------------------------------------------------------------------*/
/** @brief Paint the display after all buttons are painted with onPaint() */
void Cdialog::onDisplay()
{
	assert( (int)pthread_self()==m_main_thread);
	try
	{
		// Now put on the display.
		insertDragObject();
	}
	catch (...)
	{
	}
	try
	{
		if ( !m_dragObject.isEmpty() && m_dragObject.m_dragObject->hasGraphics())
		{
			m_dragObject.m_dragObject->unlockKeycodes();
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
    lock();
    m_messageBox.addDialog( child);
    unlock();

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
    lock();
	m_messageBox.removeDialog( child);
	unlock();

	invalidate();
}

/*============================================================================*/
///
/// @brief 		I am the messagebox and want to register.
///
/// @post       Message box removed, should not be displayed anymore.
///
/*============================================================================*/
//void Cdialog::registerMessageBox()
//{
//	m_isMessageBox=true;
//	Cdialog::registerMessageBox(this);
//}

/*============================================================================*/
///
/// @brief 		I am the messagebox and want to unregister.
///
/// @post       Message box removed, should not be displayed anymore.
///
/*============================================================================*/
//void Cdialog::unregisterMessageBox()
//{
//	Cdialog::unregisterMessageBox(this);
//}

/*----------------------------------------------------------------------------*/
/** @brief Find object for a certain point. Not in sub-dialogs!
 *  @param p [in] Object point on display.
 *  @return Dialog object found.
 */
CdialogObjectPtr Cdialog::findObject( const Cpoint &p)
{
	CdialogObjectPtr retVal;
	Cpoint p2=p/8;

	// Find which dialog should we search in.
	Cdialog *dialog =findDialog( p2);

	if ( !dialog)
	{
		return retVal;
	}
	if ( dialog !=this)
	{
		return dialog->findObject( p);
	}

	lock(); // Object list should not change.

	for (dialogObjectReverseIterator di=m_objects.rbegin();
			di!=m_objects.rend(); ++di)
	{
		CdialogObjectPtr cd =*di;
		if ( cd->isVisible() ==true && cd->inside( p2))
		{
			retVal =cd;
			break;
		}
	}
	unlock(); // Object list should not change.
	return retVal;
}

/*----------------------------------------------------------------------------*/
/** Find dialog in Children or return this dialog */
Cdialog *Cdialog::findDialogInChild( const Cpoint &point )
{
    Cdialog *retVal = m_children.findDialog(point);
    if (!retVal)
    {
        retVal = this;
    }
    return retVal;
}

/*----------------------------------------------------------------------------*/
Cdialog *Cdialog::findDialogInMbx( const Cpoint &point)
{
	lock();

	Cdialog *retVal = m_messageBox.findDialog(point);

	unlock();
	return retVal;
}

/*============================================================================*/
///
/// @brief 		Which dialog is at a certain co-ordinate?
/// @param      p [in] Location to find.
/// @post       Tell which dialog. Called from all threads.
///
/*============================================================================*/
Cdialog *Cdialog::findDialog( const Cpoint &point)
{
    lock();
    Cdialog *retVal = m_messageBox.findDialog(point);

	if (!retVal)
	{
		retVal =findDialogInChild(point);
	}
	unlock();

	return retVal;
}

/*----------------------------------------------------------------------------*/
/** For debugging the application. */
void Cdialog::idiotTest()
{
	CdialogEvent::Instance()->m_idiot =true;
}

/*----------------------------------------------------------------------------*/
/** @brief We start to drag an object.
 *  @param movingObject [in] Which object we move.
 */
void Cdialog::onStartDrag( CdialogObjectPtr movingObject)
{
	(void)movingObject;
}

/*----------------------------------------------------------------------------*/
/** @brief Overload function to drag object to other location.
 *  @param movingObject [in] What object is moved.
 *  @param to [in] Where we release the mouse. Left top corner of item.
 */
void Cdialog::dragObject( CdialogObjectPtr movingObject, const Cpoint &to)
{
	(void)movingObject;
	(void)to;
}

/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
Cpoint Cdialog::getTouchPosition()
{
	return CdialogEvent::Instance()->lastMouse();
}

/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/** @brief Scroll a dialog to a certain point.
 *  @param start [in] Start point for the scroll.
 *  @param distance [in] Distance to scroll to.
 */
void Cdialog::scrollDialog( const Cpoint &start, const Cpoint &distance)
{
	//assert( (int)pthread_self()==m_main_thread);

	lock();
	CswypeDialog *dialog =dynamic_cast<CswypeDialog*>( findDialog( start/8) );
	if ( dialog)
	{
		bool isHorizontal = dialog->isHorizontalScrollDialog(Cpoint(0,0));
		dialog->scrollRelative( isHorizontal ? (double)distance.x:(double)distance.y, true);
	}
	unlock();
}

/*----------------------------------------------------------------------------*/
/** Overload to do some with drag release. */
void Cdialog::onDragRelease( const Cpoint &position, CdialogObjectPtr movingObject)
{
	(void)position;
	(void)movingObject;
}

/*----------------------------------------------------------------------------*/
/** Overload to do some with drag release. */
void Cdialog::onPaintingStart( const Cpoint &position)
{
	try
	{
		CdialogObjectPtr object =findObject( position);
		if ( object.get())
		{
			object->onPaintingStart( position);
		}
	}
	catch (...)
	{
	}
}

/*----------------------------------------------------------------------------*/
/** Overload to do some with drag release. */
void Cdialog::onPaintingMove( const Cpoint &position)
{
	try
	{
		CdialogObjectPtr object =findObject( position);
		if ( object.get())
		{
			object->onPaintingMove( position);
		}
	}
	catch (...)
	{
	}
}

/*----------------------------------------------------------------------------*/
/** Overload to do some with drag release. */
void Cdialog::onPaintingStop( const Cpoint &position)
{
	try
	{
		CdialogObjectPtr object =findObject( position);
		if ( object)
		{
			object->onPaintingStop( position);
		}
	}
	catch (...)
	{
	}
}

/*----------------------------------------------------------------------------*/
void Cdialog::lockEvent()
{
	lock();
}
/*----------------------------------------------------------------------------*/
void Cdialog::unlockEvent()
{
	unlock();
}

/*----------------------------------------------------------------------------*/
/** At start of executing the dialog */
void Cdialog::startExecute()
{
}

/*----------------------------------------------------------------------------*/
/** At start of executing the dialog */
void Cdialog::setChild()
{
	m_isChild = true;
}

/*----------------------------------------------------------------------------*/
bool Cdialog::isChild() const
{
	return m_isChild;
}

/*----------------------------------------------------------------------------*/
/** At stop of executing the dialog */
void Cdialog::stopExecute()
{
}
