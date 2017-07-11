/*============================================================================*/
/**  @file       sdl_dialog_event.cpp
 **  @ingroup    sdl2ui
 **  @brief		 Default dialog.
 **
 **  Get event from input devices.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              CdialogEvent
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
#include "sdl_dialog_event.h"
#include "sdl_dialog_object.h"
#include "sdl_keybutton.h"
#define RANDOM(x)		(int) ((x) * (rand() / (RAND_MAX + 1.0)))

/*============================================================================*/
///
/// @brief Constructor.
///
/*============================================================================*/
CdialogEvent::CdialogEvent()
: m_interface(NULL)
, m_inputStat(MOUSE_RELEASED)
, m_mouseStat(MOUSE_RELEASED)
, m_minimumClickTime(0)
, m_numberOfPresses(0)
, m_lastMousePos(0,0)
, m_inputMousePos(0,0)
, m_repeatDelay(0)
, m_repeatSpeed(0)
, m_repeatCount(0)
, m_repeatButton(KEY_NONE)
, m_mousePressedLeft(false)
, m_scrollStart(0,0)
, m_dragPoint(0,0)
, m_dragStartPixels(0)
, m_press(0,0)
, m_isPressed(false)
, m_longDebounced(false)
, m_idiot(false)
, m_debugPosition(0,0)
{
	m_dragStartPixels =Cgraphics::m_defaults.drag_start_pixels;
	m_dragStartPixels =m_dragStartPixels*m_dragStartPixels;
	m_repeatSpeed =Cgraphics::m_defaults.repeat_speed;
	m_repeatDelay =Cgraphics::m_defaults.repeat_delay;
	m_minimumDragTime =Cgraphics::m_defaults.minimum_drag_time;
	m_minimumClickTime =Cgraphics::m_defaults.minimum_click_time;
	m_debounceTime =Cgraphics::m_defaults.touch_debounce_time;
	m_debounceDist =Cgraphics::m_defaults.touch_debounce_distance;

	while ( m_events.size()>0)
	{
		m_events.pop_front();
	}
	m_keyFile.init();
#ifdef ZHONGCAN_MULTI_THREAD
	start();
#endif
}

/*============================================================================*/
///
/// @brief Destructor
///
/*============================================================================*/
CdialogEvent::~CdialogEvent()
{
#ifdef ZHONGCAN_MULTI_THREAD
	stop();
#endif
	m_keyFile.close();
}

/** THREAD Do button repeats if a button is pressed longer, this should be done faster and faster.
 *  Do button repeats for touch screen (mouse).
 *  Do Drag and drop for items which can be dragged.
 *  Move items during dragging.
 *  Move a scroll dialog when pressed.
 *  Press a button on a scroll dialog when pressed short.
 */
void CdialogEvent::work()
{
	SDL_Event event;
	bool found =false;

	// Get events
	for (int n=0; n<25; n++)
	{
		event.type=0;
		if ( SDL_PollEvent(&event)!=1)
		{
			break;
		}
		if (event.type ==EVENT_INVALID)
		{
			continue;
		}
		Cevent c(&event, m_interface ? m_interface->spaceIsLanguage():true, false);
		// handle the event
		handleEvent(c);
	}
	switch (m_inputStat)
	{
	case MOUSE_PRESS:
		lock();
		if ( m_longDebounced==false && m_mouseLongDebounce.elapsed()>Cgraphics::m_defaults.touch_debounce_long_time)
		{
			Cevent c(EVENT_TOUCH_LONG, KEY_NONE, m_inputMousePos, false);
			m_events.push_back(c);
			m_longDebounced =true;
		}
		unlock();
		break;
	case MOUSE_RELEASE_DEBOUNCE:
		lock();
		if ( m_debounceTime==0 || m_mouseDebounce.elapsed()>m_debounceTime)
		{
			// Really press... after debounce 5 msec.
			//Cgraphics::m_defaults.log("touch release send!");
			Cevent c( EVENT_TOUCH_RELEASE, KEY_NONE, m_inputMousePos, false);
			m_inputStat =MOUSE_RELEASED;
			m_events.push_back(c);

		}
		unlock();
		break;
	case MOUSE_PRESS_DEBOUNCE:
		lock();
		if ( m_debounceTime==0 || m_mouseDebounce.elapsed()>m_debounceTime)
		{
			// Really release... after debounce 5 msec.
			//Cgraphics::m_defaults.log("touch press send??");
			Cevent c( EVENT_TOUCH_PRESS, KEY_NONE, m_inputMousePos, false);
			m_inputStat =MOUSE_PRESS;
			m_events.push_back(c);
		}
		unlock();
		break;
	default:
		break;
	}
	if ( m_idiot ==true)
	{
		if ( m_events.size()>3)
		{
			return;
		}
		//keybutton k =(keybutton)(RANDOM(500));
		int x=RANDOM(10);
		int xx =RANDOM( Cgraphics::m_defaults.width);
		int yy =RANDOM( Cgraphics::m_defaults.height);

		// Do some random action.
		switch (m_inputStat)
		{
		case MOUSE_RELEASED:
			///< Create random mouse press.
			if (x >5)
			{
				handleMousePress( Cpoint(xx,yy));
				}
			break;
		case MOUSE_PRESS_DEBOUNCE:
		case MOUSE_RELEASE_DEBOUNCE:
			break;
		case MOUSE_PRESS: ///< Press mouse without drag, possible repeat.
		case MOUSE_SCROLL: ///< Scrolling window.
		case MOUSE_DRAG: ///< Busy dragging.
		case MOUSE_PAINT:
		default:
			if (x ==3)
			{
				// Mouse release.
				handleMouseRelease( Cpoint(xx,yy));
			}
			else
			{
				// Mouse move.
				xx/=50;
				yy/=40;
				xx+=m_lastMousePos.x;
				yy+=m_lastMousePos.y;
				event.type =SDL_MOUSEMOTION;
				handleMouseMove( Cpoint(xx,yy));
			}
			break;
		}
	}
	if ( found ==false)
	{
		delay(5);
	}
}

/** @brief Convert an input event to a string.
 *  @return Event converted to string.
 */
std::string Cevent::toString()
{
	std::string s;
	switch (type)
	{
	case EVENT_TOUCH_PRESS: return "touch press";
	case EVENT_TOUCH_RELEASE: return "touch release";
	case EVENT_TOUCH_MOVE: return "touch move";
	case EVENT_KEY_PRESS: return "key press";
	case EVENT_KEY_RELEASE: return "key release";
	case EVENT_DRAG_START: return "drag start";
	case EVENT_DRAG_STOP: return "drag stop";
	case EVENT_APPMOUSEFOCUS: return "mouse focus";
	case EVENT_APPMOUSEBLUR: return "mouse blur";
	case EVENT_APPINPUTFOCUS: return "focus";
	case EVENT_APPINPUTBLUR: return "blur";
	case EVENT_RESTORE: return "restore";
	case EVENT_MINIMIZE: return "minimize";
	case EVENT_LANGUAGE_CHANGE: return "language";
	case EVENT_MAIN_DIALOG: return "dialog";
	case EVENT_BUTTON: return "button "+(std::string)Ckeybutton(button);
	case EVENT_MOUSE_CLICK: return "mouse click "+(std::string)Ckeybutton(button);
	case EVENT_WHEEL_DOWN: return "wheel down";
	case EVENT_WHEEL_UP: return "wheel up";
	case EVENT_QUIT: return "quit";
	default: return "---";
	}
}

/** Constructor, which converts input event to a real event.
 *  event [in] What event we get.
 *  spaceIsLanguage [in] If we want space to be a language change.
 *  test [in] If it is a test event.
 */
Cevent::Cevent( SDL_Event *event, bool spaceIsLanguage, bool test)
: type( EVENT_INVALID)
, status( test ? POLL_TESTING:POLL_USER)
, point( 0,0)
, button( KEY_NONE)
, mod( KMOD_NONE)
, which( 0)
{
	switch (event->type)
	{
//	case SDL_ACTIVEEVENT:
//		switch (event->active.state)
//		{
//		case SDL_APPMOUSEFOCUS:
//			type =(event->active.gain) ? EVENT_APPMOUSEFOCUS:EVENT_APPMOUSEBLUR;
//			break;
//		case SDL_APPINPUTFOCUS:
//			type =(event->active.gain) ? EVENT_APPINPUTFOCUS:EVENT_APPINPUTBLUR;
//			break;
//		case SDL_APPACTIVE:
//			type =(event->active.gain) ? EVENT_RESTORE:EVENT_MINIMIZE;
//		}
//		break;

	case SDL_KEYDOWN:
		if ( (keybutton)event->key.keysym.sym ==KEY_NONE)
		{
			break;
		}
		mod = (keymode)(event->key.keysym.mod &(~KMOD_CAPS));
		if ( event->key.keysym.sym ==SDLK_SPACE && spaceIsLanguage)
		{
			button =KEY_SPACE;
			type=EVENT_LANGUAGE_CHANGE;
			break;
		}
		type =EVENT_KEY_PRESS;
		if ( (event->key.keysym.mod&KMOD_LSHIFT)!=0 || (event->key.keysym.mod&KMOD_RSHIFT)!=0)
		{
			switch (event->key.keysym.sym)
			{
			case SDLK_8:      button =KEY_STAR; break;
			case SDLK_EQUALS: button =KEY_PLUS; break;
			default:
				if ( event->key.keysym.sym>=SDLK_F1 && event->key.keysym.sym<=SDLK_F12)
				{
					button =(keybutton)(event->key.keysym.sym+KEY_SHIFT_F1-KEY_F1);
				}
				break;
			}
		}
		switch (event->key.keysym.sym)
		{
		case SDLK_KP_PLUS: 	button =KEY_PLUS; break;
//		case SDLK_KP0: 		button =KEY_0; break;
//		case SDLK_KP1: 		button =KEY_1; break;
//		case SDLK_KP2: 		button =KEY_2; break;
//		case SDLK_KP3: 		button =KEY_3; break;
//		case SDLK_KP4: 		button =KEY_4; break;
//		case SDLK_KP5: 		button =KEY_5; break;
//		case SDLK_KP6: 		button =KEY_6; break;
//		case SDLK_KP7: 		button =KEY_7; break;
//		case SDLK_KP8: 		button =KEY_8; break;
//		case SDLK_KP9: 		button =KEY_9; break;
		case SDLK_KP_PERIOD: button =KEY_DOT; break;
		case SDLK_KP_DIVIDE: button =KEY_FORWARDSLASH; break;
		case SDLK_KP_MULTIPLY: button =KEY_STAR; break;
		case SDLK_KP_MINUS: button =KEY_MINUS; break;
		case SDLK_KP_ENTER: button =KEY_CR; break;
		case SDLK_KP_EQUALS: button =KEY_EQUALS; break;
		default: button =(keybutton)event->key.keysym.sym; break;
		}
		break;
	case SDL_EVENT_RESERVEDA:
		type =EVENT_MAIN_DIALOG;
		break;

	case SDL_MOUSEBUTTONDOWN:
		point =Cpoint( event->button.x, event->button.y);
		switch (event->button.button)
		{
		case SDL_BUTTON_LEFT:
			type =EVENT_TOUCH_PRESS;
			break;
		case SDL_BUTTON_RIGHT:
		case SDL_BUTTON_MIDDLE: // not used.
			type =EVENT_INVALID;
			break;
		//case SDL_BUTTON_WHEELUP:
		//	type =EVENT_WHEEL_UP;
		//	break;
		//case SDL_BUTTON_WHEELDOWN:
		//	type =EVENT_WHEEL_DOWN;
		//	break;
		default:
			//Cgraphics::m_defaults.log("Cevent::Cevent ???");
			break;
		}
		break;

	case SDL_MOUSEBUTTONUP:
		point =Cpoint( event->button.x, event->button.y);
		switch (event->button.button)
		{
		case SDL_BUTTON_LEFT:
			type =EVENT_TOUCH_RELEASE;
			break;
		//case SDL_BUTTON_WHEELUP:
		//case SDL_BUTTON_WHEELDOWN:
		case SDL_BUTTON_RIGHT:
		case SDL_BUTTON_MIDDLE: // not used.
			type =EVENT_INVALID;
			break;
		default:
			//Cgraphics::m_defaults.log("Cevent::Cevent 2???");
			break;
		}
		break;
	case SDL_MOUSEMOTION:
		type =EVENT_TOUCH_MOVE;
		mod =KMOD_NONE;
		button =KEY_NONE;
		point =Cpoint( event->motion.x, event->motion.y);
		which =0;
		break;

	case SDL_JOYBUTTONDOWN:
	   	type =EVENT_TOUCH_LONG;
	   	mod =KMOD_NONE;
	   	button =KEY_NONE;
	   	point =Cpoint( event->button.x, event->button.y);
		break;

	case SDL_QUIT:
		type =EVENT_QUIT;
		break;

		/*
	case SDL_QUIT:
		onExit();
		m_mousePressedLeft =false;
		m_mouseStatus =MOUSE_RELEASED;
		stop(MODE_STOP_PC);
		break;

	case SDL_SYSWMEVENT:
		//Ignore
		break;

	case SDL_VIDEORESIZE:
		onResize(event->resize.w, event->resize.h);
		break;

	case SDL_VIDEOEXPOSE:
		onExpose();
		break;
	*/
	}
}

/** THREAD Handle event after the dialog wants a new event. Lock the instance on m_events. Run from the event thread, not the dialog thread!!
 *  post: All events in the queue are handled.
 *  @param event [in] What event to handle.
 */
void CdialogEvent::handleEvent( Cevent &event)
{
	//Cgraphics::m_defaults.log("handleEvent %d %s!", event.type, event.toString().c_str());
	switch ( event.type)
	{
	case EVENT_APPINPUTFOCUS:
	case EVENT_APPINPUTBLUR:
	case EVENT_APPMOUSEFOCUS:
	case EVENT_APPMOUSEBLUR:
		break;
	case EVENT_RESTORE:
	case EVENT_MINIMIZE:
	case EVENT_LANGUAGE_CHANGE:
	case EVENT_MAIN_DIALOG:
		m_events.push_back( event);
		break;

	case EVENT_TOUCH_PRESS:
		m_debugPosition=event.point;
		handleMousePress( event.point);
		break;

	case EVENT_TOUCH_RELEASE:
		m_debugPosition=event.point;
		handleMouseRelease( event.point);
		break;

	case EVENT_TOUCH_MOVE:
		m_debugPosition=event.point;
		handleMouseMove( event.point);
		break;

	case EVENT_KEY_PRESS:
		handleKeyPress( event.mod, event.button);
		m_numberOfPresses++;
		break;

	case EVENT_KEY_RELEASE:
		handleKeyRelease();
		break;

	case EVENT_DRAG_START:
	case EVENT_DRAG_STOP:
		break;

	default:
		break;
	}
}

/** @brief THREAD Handle when we release the mouse now. Run from the event thread, not the dialog thread!!
 *  @param p [in] Where the finger/mouse has left the screen/stopped pressing.
 */
void CdialogEvent::handleMouseRelease( const Cpoint &p)
{
	lock();
	switch ( m_inputStat )
	{
	case MOUSE_PRESS:
		if ( m_debounceTime==0)
		{
			// Really press... after debounce 5 msec.
			//Cgraphics::m_defaults.log("touch release send!");
			m_inputMousePos =p;
			Cevent c( EVENT_TOUCH_RELEASE, KEY_NONE, m_inputMousePos, false);
			m_inputStat =MOUSE_RELEASED;
			m_events.push_back(c);
		}
		else
		{
			//Cgraphics::m_defaults.log("mouse release in press %d,%d", p.x, p.y);
			m_inputMousePos =p;
			m_inputStat =MOUSE_RELEASE_DEBOUNCE;
		}
		break;
	case MOUSE_RELEASED:
	case MOUSE_PRESS_DEBOUNCE:
	case MOUSE_RELEASE_DEBOUNCE:
		break;
	default:
		m_inputStat =MOUSE_RELEASED;
		break;
	}
	unlock();
}

void CdialogEvent::handleMouseLong( CeventInterface *callback, const Cevent &c)
{
	lock();
	keybutton key;
	CdialogObject *object;
	m_lastMousePos =c.point;
	m_numberOfPresses++;
	m_press =c.point;

	if ( m_interface ==NULL)
	{
		unlock();
		return;
	}

	// Check for scroll window.
	bool isScrollDialog =false;
	try
	{
		isScrollDialog =m_interface->isSwypeDialog( c.point);
	}
	catch (...)
	{
		// Not a valid scroll dialog anymore.
	}
	if ( isScrollDialog)
	{
		// Yes, we have a scroll dialog.
		(void)callback->onEvent(Cevent( EVENT_TOUCH_LONG, c.point, false));
		unlock();
		return;
	}

	// Check which button pressed on which window.
	object =NULL;
	if ( object)
	{
		// Drag object or not.
		key=object->m_code;
		if ( object->m_painting)
		{
			unlock();
			return;
		}
	}
	else
	{
		try
		{
			key =m_interface->findButton( c.point );
		}
		catch (...)
		{
			key =KEY_NONE;
		}
	}
	// Single button press.
	callback->onEvent(Cevent( EVENT_TOUCH_LONG, key, c.point, false));
	unlock();
}

void CdialogEvent::handleMouseRelease( CeventInterface *callback, const Cevent &c)
{
	keybutton key;
	CdialogObject *object;
	(void)c;
	lock();
	m_isPressed =false;
	if ( m_interface ==NULL)
	{
		m_mouseStat =MOUSE_RELEASED;
	}
	switch (m_mouseStat)
	{
	case MOUSE_RELEASED: // Already released?
		break;

	case MOUSE_DRAG:
		m_mouseStat =MOUSE_RELEASED;
		callback->onEvent(Cevent( EVENT_DRAG_STOP, c.point, false));
		break;

	case MOUSE_START_DRAG: // Stop drag, send button to press object.
	 	object =m_interface ? m_interface->findObject(c.point):NULL;
		if ( object ==NULL)
		{
			break;
		}
		try
		{
			callback->onEvent(Cevent(EVENT_MOUSE_CLICK, object->m_code, c.point, false));
		}
		catch (...)
		{
			// Object or dialog already killed...
		}
		break;
	case MOUSE_START_SCROLL_OR_DRAG: // Stop drag, decide what code to send.
		// Just change into a button press.
		key =m_interface->findButton( m_scrollStart);
		if ( key!=KEY_NONE)
		{
			callback->onEvent(Cevent(EVENT_MOUSE_CLICK, key, m_scrollStart, false));
		}
		break;

	case MOUSE_SCROLL:
		callback->onEvent(Cevent(EVENT_TOUCH_RELEASE, m_scrollStart, false));
		break;

	case MOUSE_PAINT:
		callback->onEvent(Cevent(EVENT_PAINT_STOP, c.point, false));
		break;

	case MOUSE_PRESS:
	case MOUSE_DRAG_DIALOG:
		break;
	default: // No debounce commands here
		break;
	}
	m_mouseStat =MOUSE_RELEASED;
	unlock();
}

/** THREAD Handle mouse event when the user requests an event. Run from the event thread, not the dialog thread!!
 *  @param p [in] Location where we move the mouse to
 */
void CdialogEvent::handleMouseMove( const Cpoint &p)
{
	switch (m_inputStat)
	{
	case MOUSE_RELEASE_DEBOUNCE:
		lock();
		//Cgraphics::m_defaults.log("move during debounce release %d,%d", p.x, p.y);
		if ( m_inputMousePos.distance(p)>=m_debounceDist)
		{
			m_events.push_back( Cevent( EVENT_TOUCH_MOVE, p, false));
			m_inputMousePos =p;
			m_longDebounced =true;
			m_mouseDebounce.setTime(10000,100,2);
		}
		unlock();
		break;
	default:
	case MOUSE_PRESS_DEBOUNCE:
	case MOUSE_RELEASED: ///< No activity.
		break;
	case MOUSE_SCROLL:
	case MOUSE_PRESS:
		lock();
		//Cgraphics::m_defaults.log("move during press %d,%d", p.x, p.y);
		if ( m_inputMousePos.distance(p)>=m_debounceDist)
		{
			m_events.push_back( Cevent( EVENT_TOUCH_MOVE, p, false));
			m_longDebounced =true;
			m_inputMousePos =p;
		}
		unlock();
		break;
	}
}

void CdialogEvent::handleMouseMove( CeventInterface *callback, const Cevent &c)
{
	lock();

	if ( m_interface ==NULL)
	{
		m_mouseStat =MOUSE_RELEASED;
	}
	switch (m_mouseStat)
	{
	case MOUSE_RELEASED: ///< No activity.
		break;
	case MOUSE_PAINT:
		callback->onEvent(Cevent( EVENT_PAINT_MOVE, c.point, false));
		break;

	case MOUSE_START_DRAG: ///< Wait a second before starting to drag.
		if ( m_press.distance( m_lastMousePos)>m_dragStartPixels)
		{
			m_mouseStat =MOUSE_DRAG;
			callback->onEvent(Cevent( EVENT_DRAG_START, m_press, false));
			callback->onEvent(Cevent( EVENT_DRAG_MOVE, c.point, false));
			// Tell where we go to the scroll dialog.
			//if ( m_interface)
			//{
			//	m_interface->dragObject( m_dragObject, p);
			//}
		}
		break;
	case MOUSE_START_SCROLL_OR_DRAG: ///< Start a scroll dialog.
		if ( m_interface->isScrollDragDialog(c.point/8))
		{
			// Check if it's horizontal or vertical.
			int dist_slider, dist_object;
			if ( m_interface->isHorizontalScrollDialog(c.point/8))
			{
				dist_slider =m_scrollStart.horizontalDistance(m_lastMousePos);
				dist_object =m_scrollStart.verticalDistance(m_lastMousePos);
			}
			else
			{
				dist_object =m_scrollStart.horizontalDistance(m_lastMousePos);
				dist_slider =m_scrollStart.verticalDistance(m_lastMousePos);
			}
			if ( dist_object*dist_object >m_dragStartPixels)
			{
				m_mouseStat =MOUSE_DRAG;
				callback->onEvent(Cevent( EVENT_DRAG_START, m_press, false));
				callback->onEvent(Cevent( EVENT_DRAG_MOVE, c.point, false));
			}
			else if ( dist_slider*dist_slider >m_dragStartPixels)
			{
				m_mouseStat =MOUSE_SCROLL;
				// Tell where we go to the scroll dialog.
				if ( m_interface)
				{
					m_interface->scrollDialog( m_scrollStart, m_scrollStart-c.point);
				}
			}
		}
		else if ( m_scrollStart.distance( m_lastMousePos)>m_dragStartPixels)
		{
			m_mouseStat =MOUSE_SCROLL;
			// Tell where we go to the scroll dialog.
			if ( m_interface)
			{
				m_interface->scrollDialog( m_scrollStart, m_scrollStart-c.point);
			}
		}
		break;
	case MOUSE_SCROLL: ///< Scrolling window.
		// Tell where we go to the scroll dialog.
		m_interface->scrollDialog(m_scrollStart, m_lastMousePos-c.point);
		break;
	case MOUSE_DRAG: ///< Busy dragging object.
		callback->onEvent(Cevent(EVENT_DRAG_MOVE, c.point, false));
		//m_mouseStatus =MOUSE_RELEASED;
		//m_events.push( Cevent( EVENT_DRAG_STOP, p, false));
		break;
	case MOUSE_PRESS: ///< Press mouse without drag, possible repeat.
		callback->onEvent(Cevent(EVENT_MOUSE_MOVE, c.point, false));
		break;
	case MOUSE_DRAG_DIALOG:	///< Drag entire dialog.
		break;
	default: // No debounce commands here
		break;
	}
	m_lastMousePos =c.point;
	unlock();
}

/** @brief THREAD New mouse press. Run from the event thread, not the dialog thread!!
 *  @param p [in] Location mouse press.
 */
void CdialogEvent::handleMousePress( const Cpoint &p)
{
	switch ( m_inputStat )
	{
	case MOUSE_RELEASED:
	{
		lock();
		//Cgraphics::m_defaults.log("press");
		//Cgraphics::m_defaults.log("touch press send %d,%d!", p.x, p.y);
		m_inputMousePos =p;
		Cevent c( EVENT_TOUCH_PRESS, KEY_NONE, m_inputMousePos, false);
		m_mouseLongDebounce.setTime(5000000, 2000, 2);
		m_longDebounced =false;
		m_inputStat =MOUSE_PRESS;
		m_events.push_back(c);

//		m_mouseDebounce.setTime( 1000000, 5000,2);
//		m_inputStat =MOUSE_PRESS_DEBOUNCE;
//		m_inputMousePos =p;
		unlock();
		break;
	}
	default:
	case MOUSE_RELEASE_DEBOUNCE:
	case MOUSE_PRESS_DEBOUNCE:
		//Cgraphics::m_defaults.log("press unused");
		break;
	}
}

void CdialogEvent::handleMousePress(CeventInterface *callback, const Cevent &c)
{
	lock();
	keybutton key;
	CdialogObject *object;
	m_mousePress.setTime( 10000000, 500,2);
	m_lastMousePos =c.point;
	m_numberOfPresses++;
	m_press =c.point;
	m_isPressed =true;

	if ( m_interface ==NULL)
	{
		unlock();
		return;
	}

	// Check for scroll window.
	bool isScrollDialog =false;
	try
	{
		isScrollDialog =m_interface->isSwypeDialog( c.point);
	}
	catch (...)
	{
		// Not a valid scroll dialog anymore.
	}
	if ( isScrollDialog)
	{
		// Yes, we have a scroll dialog.
		m_scrollStart =c.point;
		m_mouseStat =MOUSE_START_SCROLL_OR_DRAG;
		m_mousePress.setTime( m_minimumDragTime, 100,2);
		(void)callback->onEvent(Cevent( EVENT_TOUCH_PRESS, c.point, false));
		unlock();
		return;
	}

	// Check which button pressed on which window.
	object =NULL;
	try
	{
		object =m_interface->findObject( c.point );
		m_interface->notifyTouch( c.point );
	}
	catch (...)
	{
		object =NULL;
	}
	if ( object)
	{
		// Drag object or not.
		key=object->m_code;
		if ( object->m_dragEnable)
		{
			m_mouseStat =MOUSE_START_DRAG;
			m_mousePress.setTime( m_minimumDragTime, 100,2);
			m_interface->onEvent( Cevent( EVENT_TOUCH_PRESS, c.point, false) );
		}
		if ( object->m_painting)
		{
			m_mouseStat =MOUSE_PAINT;
			m_interface->onEvent( Cevent( EVENT_PAINT_START, c.point, false) );
			unlock();
			return;
		}
	}
	else
	{
		try
		{
			key =m_interface->findButton( c.point );
		}
		catch (...)
		{
			key =KEY_NONE;
		}
	}
	bool spaceIsLanguage =m_interface ? m_interface->spaceIsLanguage():true;

	if ( key==KEY_SPACE && spaceIsLanguage)
	{
		callback->onEvent(Cevent( EVENT_LANGUAGE_CHANGE, Cpoint(0,0),false));
		unlock();
		return;
	}
	if ( key==KEY_NONE)
	{
		unlock();
		return;
	}
	if ( !object)
	{
		m_mouseStat =MOUSE_PRESS;
		m_mousePress.stop();
	}
	// Single button press.
	callback->onEvent(Cevent( key, KMOD_NONE, false));
	unlock();
}

void CdialogEvent::handleKeyRelease()
{
	lock();
	m_repeatButton =KEY_NONE;
	m_repeatTimer.stop();
	m_isPressed =false;
	unlock();
}

/// @brief New key press.
void CdialogEvent::handleKeyPress( keymode mod, keybutton key)
{
	if ( key==KEY_NONE)
	{
		return;
	}
	lock();
	m_keyPress.setTime( 10000000, 500,2);
	m_numberOfPresses++;

	if ( m_interface ==NULL)
	{
		unlock();
		return;
	}

	// Check which button pressed on which window.
	bool spaceIsLanguage =m_interface ? m_interface->spaceIsLanguage():true;

	if ( key==KEY_SPACE && spaceIsLanguage)
	{
		m_events.push_back( Cevent( EVENT_LANGUAGE_CHANGE, Cpoint(0,0),false));
		unlock();
		return;
	}
	// Single button press.
	m_events.push_back( Cevent( key, mod, false));
	m_repeatButton =key;
	unlock();
}

/// @brief Check for existing events.
EpollStatus CdialogEvent::pollEvent( CeventInterface *callback)
{
	if ( callback ==NULL)
	{
		return POLL_EMPTY;
	}
	EpollStatus retVal =POLL_EMPTY;
	switch (m_mouseStat)
	{
	case MOUSE_RELEASED: ///< No activity.
		break;
	case MOUSE_START_DRAG: ///< Wait a second before starting to drag.
		if ( m_mousePress.elapsed()>m_minimumDragTime)
		{
			//found =true;
			m_mouseStat =MOUSE_DRAG;
			callback->onEvent(Cevent( EVENT_DRAG_START, m_press, false));
			callback->onEvent(Cevent( EVENT_DRAG_MOVE, m_lastMousePos, false));
			retVal =POLL_USER;
			return retVal;
		}
		break;

	case MOUSE_START_SCROLL_OR_DRAG: ///< Start a scroll dialog.
		if ( m_mousePress.elapsed()>m_minimumDragTime)
		{
			//found =true;
			m_mouseStat =MOUSE_SCROLL;
			if ( m_interface)
			{
				m_interface->scrollDialog( m_scrollStart, m_scrollStart-m_lastMousePos);
			}
			retVal =POLL_USER;
		}
		break;
	case MOUSE_PAINT:
		break;
	case MOUSE_SCROLL: ///< Scrolling window.
		break;
	case MOUSE_DRAG: ///< Busy dragging.
		break;
	case MOUSE_PRESS: ///< Press mouse without drag, possible repeat.
		break;
	case MOUSE_DRAG_DIALOG:	///< Drag entire dialog.
		break;
	default:
		break;
	}
	if ( m_events.empty() ==true)
	{
		SDL_Event event;
		if ( Cgraphics::m_defaults.get_test_event &&
			 Cgraphics::m_defaults.get_test_event( event) ==true)
		{
			Cevent c(&event, m_interface ? m_interface->spaceIsLanguage():true, false);
			m_keyFile.onEvent( &event);
			handleEvent(callback, c);
			return c.status;
		}
		return retVal;
	}
	Cevent event =m_events.front();
	//Cgraphics::m_defaults.log("poll event %s", event.toString().c_str());

	m_events.pop_front();
	m_keyFile.onEvent( event);
	handleEvent( callback, event);
	return event.status;
}

// Now we want to convert an event just before we give it to the dialog.
void CdialogEvent::handleEvent( CeventInterface *callback, const Cevent &event)
{
	switch (event.type)
	{
	case EVENT_TOUCH_PRESS:
		handleMousePress( callback, event );
		break;
	case EVENT_TOUCH_RELEASE:
		handleMouseRelease( callback, event );
		break;
	case EVENT_TOUCH_LONG:
		handleMouseLong( callback, event );
		break;
	case EVENT_TOUCH_MOVE:
	case EVENT_MOUSE_MOVE:
		handleMouseMove( callback, event );
		break;
	case EVENT_DRAG_START:
	case EVENT_DRAG_MOVE:
	case EVENT_MAIN_DIALOG:
	case EVENT_KEY_PRESS: ///< Input/Output event.
	case EVENT_LANGUAGE_CHANGE:
		callback->onEvent(event);
		break;

	default:
		break;
//	EVENT_KEY_RELEASE,		///< Input event.
//	,		///< Input event.
//	EVENT_PAINT_START,		///< Start painting.
//	EVENT_DRAG_MOVE,		///< Output event.
//	EVENT_PAINT_MOVE,		///< Move finger while painting.
//	EVENT_MOUSE_MOVE,		////< Just move the mouse when pressed.
//	EVENT_DRAG_STOP,		///< Input event.
//	EVENT_PAINT_STOP,		///< Stop painting.
//	EVENT_APPMOUSEFOCUS,	///< Input/Output event.
//	EVENT_APPMOUSEBLUR, 	///< Input/Output event.
//	EVENT_APPINPUTFOCUS,	///< Input/Output event.
//	EVENT_APPINPUTBLUR, 	///< Input/Output event.
//	EVENT_RESTORE,      	///< Input/Output event.
//	EVENT_MINIMIZE,     	///< Input/Output event.
//	EVENT_LANGUAGE_CHANGE,  ///< Output event.
//	EVENT_MAIN_DIALOG,		///< Output event.
//	EVENT_BUTTON,			///< Output event.
//	EVENT_MOUSE_CLICK,		///< Short click for a button.
//	EVENT_WHEEL_DOWN,		///< Wheel rotate (mouse).
//	EVENT_WHEEL_UP,			///< Wheel rotate (mouse).
//	EVENT_QUIT,				///< Quit application.
//	EVENT_INVALID,

	}
}

/// For debugging the application.
void CdialogEvent::createRandomEvent()
{
#if 0
	static int x =RANDOM(5);
	int xx,yy;
	keybutton k;
	switch (x)
	{
	case 0: //Keyboard event
	case 1:
		event->type =SDL_KEYDOWN;
		x=RANDOM(10);
		xx =RANDOM(m_squares_width);
		yy =RANDOM(m_squares_height);
		k =m_mainGraph->m_touch.getKey( Cpoint( xx,yy));
		//event->key.keysym.sym =(SDLKey)RANDOM(KEY_MAXIMUM);
		if (RANDOM(1000)<50)
		{
			m_events.push_back( Cevent( k, KMOD_NONE, true));
			return;
		}
		if (RANDOM(1000)<25)
		{
			m_events.push_back( Cevent( KEY_CR, KMOD_NONE, true));
		}
		if (RANDOM(1000)<5)
		{
			m_events.push_back( Cevent( KEY_ESCAPE, KMOD_NONE, true));
		}
		return;

	case 2: // Mouse press
	case 3: // Mouse move
	case 4: // Mouse release

#if 0
	case 1: //Mouse move
		event->type =SDL_MOUSEMOTION;
		event->motion.x =RANDOM( m_squares_width*8);
		event->motion.y =RANDOM( m_squares_height*8);
		x=RANDOM(10);
		break;

	case 2: //Mouse release
		event->type =SDL_MOUSEBUTTONUP;
		event->motion.x =RANDOM( m_squares_width*8);
		event->motion.y =RANDOM( m_squares_height*8);
		event->button.button =SDL_BUTTON_LEFT;
		if (RANDOM(100)<25)
		{
			event->button.button =SDL_BUTTON_RIGHT;
		}
		if (RANDOM(100)<4)
		{
			event->button.button =SDL_BUTTON_MIDDLE;
		}
		x=RANDOM(10);
		break;

	default: //Mouse press
		event->type =SDL_MOUSEBUTTONDOWN;
		event->motion.x =RANDOM( m_squares_width*8);
		event->motion.y =RANDOM( m_squares_height*8);
		event->button.button =SDL_BUTTON_LEFT;
		if (RANDOM(100)<5)
		{
			event->button.button =SDL_BUTTON_RIGHT;
		}
		if (RANDOM(100)<4)
		{
			event->button.button =SDL_BUTTON_MIDDLE;
		}
		x=RANDOM(10);
		break;
#endif
	default:
		break;
	}
#endif
}

void CdialogEvent::stopDrag()
{
	//m_dragObject =NULL;
	m_mouseStat =MOUSE_RELEASED;
}

/** Set new dialog. Unless we're just about to check a dialog.
 *  @param interface [in] New interface.
 */
void CdialogEvent::registerActiveDialog( CeventInterface *interface) //{ m_interface =interface; }
{
	lock();
	m_interface =interface;
	unlock();
}

/// @brief Queue for events.
CeventQueue::CeventQueue()
: m_size(0)
{}

/** @brief Destructor */
CeventQueue::~CeventQueue()
{}

/** @brief Add event to the event queue.
 *  @param event [in] New event.
 */
void CeventQueue::push_back( const Cevent &event)
{
	m_lock.lock();
	m_eventList.push( event);
	m_size++;
	m_lock.unlock();
}

/** @brief Get the first event from the queue.
 *  @return First event from stack.
 */
Cevent CeventQueue::front()
{
	m_lock.lock();
	Cevent evt =m_eventList.front();
	m_lock.unlock();
	return evt;
}

/** @brief Remove first element from event stack. */
void CeventQueue::pop_front()
{
	m_lock.lock();
	m_eventList.pop();
	m_size--;
	m_lock.unlock();
}

/** @brief Find how many items in the queue.
 *  @return Number of events.
 */
int CeventQueue::size()
{
	m_lock.lock();
	int n =m_eventList.size();
	m_lock.unlock();
	return n;
}

/** @brief Check if the event queue is empty.
 *  @return true when it's really empty.
 */
bool CeventQueue::empty()
{
	m_lock.lock();
	bool b =m_eventList.empty();
	m_lock.unlock();
	return b;
}
