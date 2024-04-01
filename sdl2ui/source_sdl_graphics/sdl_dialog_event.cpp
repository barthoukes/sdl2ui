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
#include "sdl_graphics.h"
#include "sdl_dialog_event.h"
#include "sdl_dialog_object.h"

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
, m_touchBottom(20000)
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

/*----------------------------------------------------------------------------*/
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
	event.type = 0;
	bool found =false;

	// Get events
	for (int n=0; n<25; n++)
	{
		event.type=0;
		if ( SDL_PollEvent(&event)==1 && event.type !=EVENT_INVALID)
		{
			found =true;
			if (m_interface)
			{
				m_interface->lockEvent();
				Cevent c(&event, m_interface->spaceIsLanguage(), false);
				m_interface->unlockEvent();
				// handle the event
				handleRawEvent(c);
			}
			else
			{
				Cevent c(&event, true, false);
				// handle the event
				handleRawEvent(c);
			}
		}
		stateMachine();
		createRandomEvent();
	}
	if ( found ==false)
	{
		delay(5);
	}
}

/*----------------------------------------------------------------------------*/
void CdialogEvent::logState()
{
	static EmouseStatus lastState = MOUSE_RELEASED;
	if (lastState != m_inputStat)
	{
		Cgraphics::m_defaults.log("State =%d, position=%d,%d", m_inputStat, m_lastMousePos.x, m_lastMousePos.y);
		lastState =m_inputStat;
	}
}

/*----------------------------------------------------------------------------*/
void CdialogEvent::setTouchBottom(int bottom)
{
    m_touchBottom = bottom;
}

/*----------------------------------------------------------------------------*/
void CdialogEvent::createRandomEvent()
{
	if ( m_idiot ==true)
	{
		if ( m_events.size()>3)
		{
			return;
		}
		//keybutton k =(keybutton)(RANDOM(500));
		int x=RANDOM(100);
		int xx =RANDOM( Cgraphics::m_defaults.width);
		int yy =RANDOM( Cgraphics::m_defaults.height);

		// Do some random action.
		switch (m_inputStat)
		{
		case MOUSE_RELEASED:
			///< Create random mouse press.
			if (x>96)
			{
				handleRawKeyPress(KMOD_NONE, KEY_CR);
			}
			else if (x >50)
			{
				handleRawMousePress( Cpoint(xx,yy));
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
			if (x ==30)
			{
				// Mouse release.
				handleRawMouseRelease( Cpoint(xx,yy));
			}
			else
			{
				// Mouse move.
				xx/=50;
				yy/=40;
				xx+=m_lastMousePos.x;
				yy+=m_lastMousePos.y;
				handleRawMouseMove( Cpoint(xx,yy));
			}
			break;
		}
	}
}

/*----------------------------------------------------------------------------*/
void CdialogEvent::stateMachine()
{
	switch (m_inputStat)
	{
	case MOUSE_PRESS:
		lock();
		if ( m_longDebounced==false
		   && m_inputMousePos.y < m_touchBottom
		   && m_mouseLongDebounce.elapsed()>Cgraphics::m_defaults.touch_debounce_long_time)
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
			// Really release... after debounce >5 msec.
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
}

/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
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
	case SDL_KEYDOWN:
		if ( (keybutton)event->key.keysym.sym ==KEY_NONE)
		{
			break;
		}
		mod = (SDLMod)(event->key.keysym.mod &(~KMOD_CAPS));
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
#ifndef USE_SDL2
		case SDLK_KP0: 		button =KEY_0; break;
		case SDLK_KP1: 		button =KEY_1; break;
		case SDLK_KP2: 		button =KEY_2; break;
		case SDLK_KP3: 		button =KEY_3; break;
		case SDLK_KP4: 		button =KEY_4; break;
		case SDLK_KP5: 		button =KEY_5; break;
		case SDLK_KP6: 		button =KEY_6; break;
		case SDLK_KP7: 		button =KEY_7; break;
		case SDLK_KP8: 		button =KEY_8; break;
		case SDLK_KP9: 		button =KEY_9; break;
#endif
		case SDLK_KP_PERIOD: button =KEY_DOT; break;
		case SDLK_KP_DIVIDE: button =KEY_FORWARDSLASH; break;
		case SDLK_KP_MULTIPLY: button =KEY_STAR; break;
		case SDLK_KP_MINUS: button =KEY_MINUS; break;
		case SDLK_KP_ENTER: button =KEY_CR; break;
		case SDLK_KP_EQUALS: button =KEY_EQUALS; break;
		default: button =(keybutton)event->key.keysym.sym; break;
		}
		break;
#ifdef USE_SDL2
	case SDL_USEREVENT:
#else
	case SDL_EVENT_RESERVEDA:
#endif
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
#ifndef USE_SDL2
		case SDL_BUTTON_WHEELUP:
			type =EVENT_WHEEL_UP;
			break;
		case SDL_BUTTON_WHEELDOWN:
			type =EVENT_WHEEL_DOWN;
			break;
#endif
		default:
			//Cgraphics::m_defaults.log("Cevent::Cevent ???");
			break;
		}
		break;

#ifdef USE_SDL2
	case SDL_MOUSEWHEEL:
		type =(event->wheel.y >0) ? EVENT_WHEEL_UP:EVENT_WHEEL_DOWN;
		break;
#endif

	case SDL_MOUSEBUTTONUP:
		point =Cpoint( event->button.x, event->button.y);
		switch (event->button.button)
		{
		case SDL_BUTTON_LEFT:
			type =EVENT_TOUCH_RELEASE;
			break;
#ifndef USE_SDL2
		case SDL_BUTTON_WHEELUP:
		case SDL_BUTTON_WHEELDOWN:
#endif
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

	case SDL_SYSWMEVENT:
		type = EVENT_BARCODE;
		barcode = (char*)event->user.data1;
		break;
	}
}

/*----------------------------------------------------------------------------*/
/** THREAD Handle event after the dialog wants a new event. Lock the instance on m_events. Run from the event thread, not the dialog thread!!
 *  post: All events in the queue are handled.
 *  @param event [in] What event to handle.
 */
void CdialogEvent::handleRawEvent( Cevent &event)
{
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
		handleRawMousePress( event.point);
		break;

	case EVENT_TOUCH_RELEASE:
		m_debugPosition=event.point;
		handleRawMouseRelease( event.point);
		break;

	case EVENT_TOUCH_MOVE:
		m_debugPosition=event.point;
		handleRawMouseMove( event.point);
		break;

	case EVENT_KEY_PRESS:
		handleRawKeyPress( event.mod, event.button);
		m_numberOfPresses++;
		break;

	case EVENT_KEY_RELEASE:
		handleRawKeyRelease();
		break;

	case EVENT_DRAG_START:
	case EVENT_DRAG_STOP:
		break;

	default:
		break;
	}
}

/*----------------------------------------------------------------------------*/
/** @brief THREAD Handle when we release the mouse now. Run from the event thread, not the dialog thread!!
 *  @param p [in] Where the finger/mouse has left the screen/stopped pressing.
 */
void CdialogEvent::handleRawMouseRelease( const Cpoint &p)
{
	if (p.y >= m_touchBottom)
	{
		return;
	}
	lock();
	switch ( m_inputStat )
	{
	case MOUSE_PRESS:
		if ( m_debounceTime <7 && p.y<m_touchBottom)
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
			m_mouseDebounce.setTime( 1000000, 5000,2);
			m_inputStat =MOUSE_RELEASE_DEBOUNCE;
		}
		break;
	case MOUSE_RELEASED:
	case MOUSE_RELEASE_DEBOUNCE:
		break;
	case MOUSE_PRESS_DEBOUNCE:
	default:
		m_inputStat =MOUSE_RELEASED;
		break;
	}
	unlock();
}

/*----------------------------------------------------------------------------*/
void CdialogEvent::handlePostMouseLong( CeventInterface *callback, const Cevent &c)
{
	keybutton key;
	CdialogObjectPtr object;
	m_lastMousePos =c.point;
	m_numberOfPresses++;
	m_press =c.point;

	if ( m_interface ==NULL)
	{
		return;
	}

	// Check for scroll window.
	bool isScrollDialog =false;
	try
	{
		m_interface->lockEvent();
		lock();
		isScrollDialog =m_interface->isSwypeDialog( c.point);
		unlock();
		m_interface->unlockEvent();
	}
	catch (...)
	{
		// Not a valid scroll dialog anymore.
	}
	if ( isScrollDialog)
	{
		// Yes, we have a scroll dialog.
		(void)callback->onEvent(Cevent( EVENT_TOUCH_LONG, c.point, false));
		return;
	}

	// Check which button pressed on which window.
	if ( object.get())
	{
		// Drag object or not.
		key=object->getKey(c.point);
		if ( object->isPaintingDuringDrag())
		{
			// Painting objects have never a long press
			return;
		}
	}
	else
	{
		try
		{
			m_interface->lockEvent();
			lock();
			key =m_interface->findButton( c.point );
			unlock();
			m_interface->unlockEvent();
		}
		catch (...)
		{
			key =KEY_NONE;
		}
	}
	// Single button press.
	callback->onEvent(Cevent( EVENT_TOUCH_LONG, key, c.point, false));
}

/*----------------------------------------------------------------------------*/
void CdialogEvent::handlePostMouseRelease( CeventInterface *callback, const Cevent &c)
{
	keybutton key;
	CdialogObjectPtr object;
	(void)c;
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
		m_interface->lockEvent();
		lock();
		object = m_interface->findObject(c.point);
		unlock();
		m_interface->unlockEvent();
		if ( object.get())
		{
			try
			{
				callback->onEvent(Cevent(EVENT_MOUSE_CLICK, object->getKey(c.point), c.point, false));
			}
			catch (...)
			{
				// Object or dialog already killed...
			}
		}
		break;
	case MOUSE_START_SCROLL_OR_DRAG: // Stop drag, decide what code to send.
		// Just change into a button press.
		m_interface->lockEvent();
		lock();
		key =m_interface->findButton( m_scrollStart);
		unlock();
		m_interface->unlockEvent();
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
}

/*----------------------------------------------------------------------------*/
/** THREAD Handle mouse event when the user requests an event. Run from the event thread, not the dialog thread!!
 *  @param p [in] Location where we move the mouse to
 */
void CdialogEvent::handleRawMouseMove( const Cpoint &p)
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
			m_mouseDebounce.setTime(100000,100,2);
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

/*----------------------------------------------------------------------------*/
void CdialogEvent::handlePostMouseMove( CeventInterface *callback, const Cevent &c)
{
	bool isScrollDrag;
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
		}
		break;

	case MOUSE_START_SCROLL_OR_DRAG: ///< Start a scroll dialog.
		m_interface->lockEvent();
        lock();
		isScrollDrag = m_interface->isScrollDragDialog(c.point/8);
		unlock();
		m_interface->unlockEvent();

		if ( isScrollDrag)
		{
			// Check if it's horizontal or vertical.
			int dist_slider, dist_object;
			m_interface->lockEvent();
			lock();
            bool horizontal = m_interface->isHorizontalScrollDialog(c.point/8);
            unlock();
			m_interface->unlockEvent();
			if ( horizontal)
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
				m_interface->scrollDialog( m_scrollStart, m_scrollStart-c.point);
			}
		}
		else if ( m_scrollStart.distance( m_lastMousePos)>m_dragStartPixels)
		{
			m_mouseStat =MOUSE_SCROLL;
			// Tell where we go to the scroll dialog.
			m_interface->scrollDialog( m_scrollStart, m_scrollStart-c.point);
		}
		break;

	case MOUSE_SCROLL: ///< Scrolling window.
		// Tell where we go to the scroll dialog.
		m_interface->lockEvent();
		lock();
		m_interface->scrollDialog(m_scrollStart, m_lastMousePos-c.point);
		unlock();
		m_interface->unlockEvent();
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
}

/*----------------------------------------------------------------------------*/
/** @brief Add barcode event to the list.
 *  @param barcode [in] Barcode found by reader.
 */
void CdialogEvent::handleBarcode( const std::string &barcode)
{
	Cevent c(barcode, false);
	m_events.push_back(c);
}

/*----------------------------------------------------------------------------*/
/** @brief THREAD New mouse press. Run from the event thread, not the dialog thread!!
 *  @param p [in] Location mouse press.
 */
void CdialogEvent::handleRawMousePress( const Cpoint &p)
{
	if (p.y >= m_touchBottom)
	{
		return;
	}
	switch ( m_inputStat )
	{
	case MOUSE_RELEASED:
	{
		lock();
		if ( m_debounceTime<7)
		{
			m_inputMousePos =p;
			Cevent c( EVENT_TOUCH_PRESS, KEY_NONE, m_inputMousePos, false);
			m_mouseLongDebounce.setTime(5000000, 2000, 2);
			m_longDebounced =false;
			m_inputStat =MOUSE_PRESS;
			m_events.push_back(c);
		}
		else
		{
			m_mouseDebounce.setTime( 1000000, 5000,2);
			m_inputStat =MOUSE_PRESS_DEBOUNCE;
			m_inputMousePos =p;
		}
		unlock();
		break;
	}
	default:
	case MOUSE_RELEASE_DEBOUNCE:
		lock();
		if ( m_inputMousePos.distance(p)<m_debounceDist)
		{
			// same button
			m_mouseDebounce.setTime( 1000000, 5000,2);
		}
		else
		{
			// Send release command
			m_inputMousePos =p;
 			Cevent c( EVENT_TOUCH_RELEASE, KEY_NONE, m_inputMousePos, false);
			m_inputStat =MOUSE_RELEASED;
			m_events.push_back(c);

			// Start new debounce key
			m_mouseDebounce.setTime( 1000000, 5000,2);
			m_inputStat =MOUSE_PRESS_DEBOUNCE;
			m_inputMousePos =p;
		}
		unlock();
		break;
	case MOUSE_PRESS_DEBOUNCE:
		break;
	}
}

/*----------------------------------------------------------------------------*/
void CdialogEvent::handlePostMousePress(CeventInterface *callback, const Cevent &c)
{
	keybutton key;
	CdialogObjectPtr object;
	m_mousePress.setTime( 10000000, 500,2);
	m_lastMousePos =c.point;
	m_numberOfPresses++;
	m_press =c.point;
	m_isPressed =true;

	if ( m_interface ==NULL)
	{
		return;
	}

	// Check for scroll window.
	bool isScrollDialog =false;
	try
	{
		m_interface->lockEvent();
		lock();
		isScrollDialog =m_interface->isSwypeDialog( c.point);
		unlock();
		m_interface->unlockEvent();
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
		return;
	}

	// Check which button pressed on which window.
	try
	{
		m_interface->lockEvent();
		lock();
		object =m_interface->findObject( c.point );
		m_interface->notifyTouch( c.point );
		m_interface->unlockEvent();
		unlock();
	}
	catch (...)
	{
		object = nullptr;
	}
	if (object.get())
	{
		// Drag object or not.
		key=object->getKey(c.point/8);
		if ( object->isDragEnabled())
		{
			m_mouseStat =MOUSE_START_DRAG;
			m_mousePress.setTime( m_minimumDragTime, 100,2);
			m_interface->lockEvent();
			lock();
			m_interface->onEvent( Cevent( EVENT_TOUCH_PRESS, c.point, false) );
			unlock();
			m_interface->unlockEvent();
		}
		if ( object->isPaintingDuringDrag())
		{
			m_mouseStat =MOUSE_PAINT;
			m_interface->lockEvent();
			lock();
			m_interface->onEvent( Cevent( EVENT_PAINT_START, c.point, false) );
			unlock();
			m_interface->unlockEvent();
			return;
		}
	}
	else
	{
		try
		{
			m_interface->lockEvent();
			lock();
			key =m_interface->findButton( c.point );
			unlock();
			m_interface->unlockEvent();
		}
		catch (...)
		{
			key =KEY_NONE;
		}
	}
	m_interface->lockEvent();
	lock();
	bool spaceIsLanguage =m_interface ? m_interface->spaceIsLanguage():true;
	unlock();
	m_interface->unlockEvent();

	if ( key==KEY_SPACE && spaceIsLanguage)
	{
		callback->onEvent(Cevent( EVENT_LANGUAGE_CHANGE, Cpoint(0,0),false));
		return;
	}
	if ( key==KEY_NONE)
	{
		return;
	}
	if ( !object)
	{
		m_mouseStat =MOUSE_PRESS;
		m_mousePress.stop();
	}
	// Single button press.
	callback->onEvent(Cevent( key, KMOD_NONE, false));
}

/*----------------------------------------------------------------------------*/
void CdialogEvent::handleRawKeyRelease()
{
	lock();
	m_repeatButton =KEY_NONE;
	m_repeatTimer.stop();
	m_isPressed =false;
	unlock();
}

/*----------------------------------------------------------------------------*/
/// @brief New key press.
void CdialogEvent::handleRawKeyPress( SDLMod mod, keybutton key)
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
	m_interface->lockEvent();
	bool spaceIsLanguage =m_interface ? m_interface->spaceIsLanguage():true;
	m_interface->unlockEvent();

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

/*----------------------------------------------------------------------------*/
void CdialogEvent::updateLastMouse( const SDL_Event &event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
	case SDL_JOYBUTTONDOWN:
		m_lastMousePos =Cpoint( event.button.x, event.button.y);
		break;
	case SDL_MOUSEMOTION:
		m_lastMousePos =Cpoint( event.motion.x, event.motion.y);
		break;
	default:
		break;
	}
}

/*----------------------------------------------------------------------------*/
void CdialogEvent::stop()
{
    CmyThread::stop();
}

/*----------------------------------------------------------------------------*/
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
				m_interface->lockEvent();
				m_interface->scrollDialog( m_scrollStart, m_scrollStart-m_lastMousePos);
				m_interface->unlockEvent();
			}
			retVal =POLL_USER;
		}
		break;

	case MOUSE_PAINT:
	case MOUSE_SCROLL: ///< Scrolling window.
	case MOUSE_DRAG: ///< Busy dragging.
	case MOUSE_PRESS: ///< Press mouse without drag, possible repeat.
	case MOUSE_DRAG_DIALOG:	///< Drag entire dialog.
	default:
		break;
	}
	if ( m_events.empty() ==true)
	{
		SDL_Event event;
		if ( Cgraphics::m_defaults.get_test_event &&
			 Cgraphics::m_defaults.get_test_event( event) ==true)
		{
			updateLastMouse( event);
			Cevent c(&event, m_interface ? m_interface->spaceIsLanguage():true, false);
			m_keyFile.onEvent( &event);
			handlePostEvent(callback, c);
			return c.status;
		}
		return retVal;
	}
	Cevent event =m_events.front();

	m_events.pop_front();
	m_keyFile.onEvent( event);
	handlePostEvent( callback, event);
	return event.status;
}

/*----------------------------------------------------------------------------*/
// Now we want to convert an event just before we give it to the dialog.
void CdialogEvent::handlePostEvent( CeventInterface *callback, const Cevent &event)
{
	switch (event.type)
	{
	case EVENT_TOUCH_PRESS:
		handlePostMousePress( callback, event );
		break;
	case EVENT_TOUCH_RELEASE:
		handlePostMouseRelease( callback, event );
		break;
	case EVENT_TOUCH_LONG:
		handlePostMouseLong( callback, event );
		break;
	case EVENT_TOUCH_MOVE:
	case EVENT_MOUSE_MOVE:
		handlePostMouseMove( callback, event );
		break;
	case EVENT_DRAG_START:
	case EVENT_DRAG_MOVE:
	case EVENT_MAIN_DIALOG:
	case EVENT_KEY_PRESS: ///< Input/Output event.
	case EVENT_LANGUAGE_CHANGE:
		callback->onEvent(event);
		break;
	case EVENT_BARCODE:
		callback->onBarcode(event.barcode);
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

/*----------------------------------------------------------------------------*/
void CdialogEvent::stopDrag()
{
	//m_dragObject =NULL;
	m_mouseStat =MOUSE_RELEASED;
}

/*----------------------------------------------------------------------------*/
Cpoint CdialogEvent::lastMouse() const
{
    return m_lastMousePos;
}

/*----------------------------------------------------------------------------*/
bool CdialogEvent::isMousePressed() const
{
    return m_isPressed;
}

/*----------------------------------------------------------------------------*/
EmouseStatus CdialogEvent::getStatus() const
{
    return m_mouseStat;
}

/*----------------------------------------------------------------------------*/
void CdialogEvent::startIdiotTesting()
{
    m_idiot =true;
}

/*----------------------------------------------------------------------------*/
/** Set new dialog. Unless we're just about to check a dialog.
 *  @param interface [in] New interface.
 */
void CdialogEvent::registerActiveDialog( CeventInterface *interface)
{
	lock();
	m_interface =interface;
	unlock();
}

/*----------------------------------------------------------------------------*/
void CdialogEvent::forceDrag( const Cpoint &dragPoint)
{
    lock();
    m_dragPoint =dragPoint;
    m_mouseStat =MOUSE_DRAG;
    unlock();
}

/*----------------------------------------------------------------------------*/
CeventInterface *CdialogEvent::getInterface()
{
    return m_interface;
}

/*----------------------------------------------------------------------------*/
/// @brief Queue for events.
CeventQueue::CeventQueue()
: m_size(0)
{}

/*----------------------------------------------------------------------------*/
/** @brief Destructor */
CeventQueue::~CeventQueue()
{
	while (!m_eventList.empty())
	{
		m_eventList.pop();
	}
}

/*----------------------------------------------------------------------------*/
/** @brief Add event to the event queue.
 *  @param event [in] New event.
 */
void CeventQueue::push_back( const Cevent &event)
{
	lock();
	m_eventList.push( event);
	m_size++;
	unlock();
}

/*----------------------------------------------------------------------------*/
/** @brief Get the first event from the queue.
 *  @return First event from stack.
 */
Cevent CeventQueue::front()
{
	lock();
	Cevent evt =m_eventList.front();
	unlock();
	return evt;
}

/*----------------------------------------------------------------------------*/
/** @brief Remove first element from event stack. */
void CeventQueue::pop_front()
{
	lock();
	m_eventList.pop();
	m_size--;
	unlock();
}

/*----------------------------------------------------------------------------*/
/** @brief Find how many items in the queue.
 *  @return Number of events.
 */
int CeventQueue::size()
{
	lock();
	int n =m_eventList.size();
	unlock();
	return n;
}

/*----------------------------------------------------------------------------*/
/** @brief Check if the event queue is empty.
 *  @return true when it's really empty.
 */
bool CeventQueue::empty()
{
	lock();
	bool b =m_eventList.empty();
	unlock();
	return b;
}
