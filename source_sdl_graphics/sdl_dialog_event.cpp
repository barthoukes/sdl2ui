/*============================================================================*/
/**  @file       sdl_dialog_event.cpp
 **  @ingroup    zhongcan_sdl
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
 **  Copyright (c) Bart Houkes, 28 jan 2011
 ** 
 **  Copyright notice:
 **  This software is property of Bart Houkes.
 **  Unauthorized duplication and disclosure to third parties is forbidden.
 **============================================================================*/

/*------------- Standard includes --------------------------------------------*/
#include "sdl_dialog_event.h"
#include "sdl_dialog_object.h"

#define RANDOM(x)		(int) ((x) * (rand() / (RAND_MAX + 1.0)))

/*============================================================================*/
///
/// @brief Constructor.
///
/*============================================================================*/
CdialogEvent::CdialogEvent()
: m_interface( NULL)
, m_mouseStatus( MOUSE_RELEASED)
, m_minimumClickTime( 0)
, m_numberOfPresses( 0)
, m_lastMouse( 0,0)
, m_repeatDelay( 0)
, m_repeatSpeed( 0)
, m_repeatCount( 0)
, m_repeatButton( KEY_NONE)
, m_mousePressedLeft( false)
, m_scrollStart( 0,0)
, m_dragPoint( 0,0)
, m_dragStartPixels(0)
, m_press( 0,0)
, m_isPressed(false)
, m_idiot( false)
{
	m_dragStartPixels =Cgraphics::m_defaults.drag_start_pixels;
	m_dragStartPixels =m_dragStartPixels*m_dragStartPixels;
	m_repeatSpeed =Cgraphics::m_defaults.repeat_speed;
	m_repeatDelay =Cgraphics::m_defaults.repeat_delay;
	m_minimumDragTime =Cgraphics::m_defaults.minimum_drag_time;
	m_minimumClickTime =Cgraphics::m_defaults.minimum_click_time;
	while ( m_events.size()>0)
	{
		m_events.pop_front();
	}
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
}

/** Do button repeats if a button is pressed longer, this should be done faster and faster.
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
	for ( int n=0; n<10; n++)
	{
		lock();
		if ( SDL_PollEvent( &event)>0)
		{
			unlock();
			bool spaceIsLanguage =m_interface ? m_interface->spaceIsLanguage():true;
			Cevent c( &event, spaceIsLanguage, false);

			handleEvent( c);
			// CdialogEvent::work event
			m_keyFile.onEvent( &event);
			if ( c.type ==EVENT_KEY_PRESS)
			{
				m_idiot =false;
			}
			found =true;
		}
		else
		{
			unlock();
		}
		switch (m_mouseStatus)
		{
		case MOUSE_RELEASED: ///< No activity.
			break;
		case MOUSE_START_DRAG: ///< Wait a second before starting to drag.
			if ( m_mousePress.elapsed()>m_minimumDragTime)
			{
				found =true;
				m_mouseStatus =MOUSE_DRAG;
				m_events.push_back( Cevent( EVENT_DRAG_START, m_press, false));
				m_events.push_back( Cevent( EVENT_DRAG_MOVE, m_lastMouse, false));
			}
			break;

		case MOUSE_START_SCROLL_OR_DRAG: ///< Start a scroll dialog.
			if ( m_mousePress.elapsed()>m_minimumDragTime)
			{
				found =true;
				m_mouseStatus =MOUSE_SCROLL;
				if ( m_interface)
				{
					m_interface->scrollDialog( m_scrollStart, m_scrollStart-m_lastMouse);
				}
			}
			break;
		case MOUSE_SCROLL: ///< Scrolling window.
			break;
		case MOUSE_DRAG: ///< Busy dragging.
			break;
		case MOUSE_PRESS: ///< Press mouse without drag, possible repeat.
			break;
		case MOUSE_DRAG_DIALOG:	///< Drag entire dialog.
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
			switch (m_mouseStatus)
			{
			case MOUSE_RELEASED:
				///< Create random mouse press.
				if (x >5)
				{
					event.type =SDL_MOUSEBUTTONDOWN;
					event.motion.x =xx;
					event.motion.y =yy;
					event.button.button =SDL_BUTTON_LEFT;
					bool spaceIsLanguage =m_interface ? m_interface->spaceIsLanguage():true;
					Cevent c( &event, spaceIsLanguage, true);
					handleEvent( c);
				}
				break;
			case MOUSE_PRESS: ///< Press mouse without drag, possible repeat.
			case MOUSE_START_DRAG: ///< Wait a second before starting to drag.
			case MOUSE_START_SCROLL_OR_DRAG: ///< Start a scroll dialog.
			case MOUSE_SCROLL: ///< Scrolling window.
			case MOUSE_DRAG: ///< Busy dragging.
			case MOUSE_DRAG_DIALOG:	///< Drag entire dialog.
				// In some cases do a mouse release at same position.
				if (x ==3)
				{
					// Mouse release.
					event.type =SDL_MOUSEBUTTONUP;
					event.motion.x =xx;
					event.motion.y =yy;
					event.button.button =SDL_BUTTON_LEFT;
					bool spaceIsLanguage =m_interface ? m_interface->spaceIsLanguage():true;
					Cevent c( &event, spaceIsLanguage, true);
					handleEvent( c);
				}
				else
				{
					// Mouse move.
					event.type =SDL_MOUSEMOTION;
					event.motion.x =xx;
					event.motion.y =yy;
					bool spaceIsLanguage =m_interface ? m_interface->spaceIsLanguage():true;
					Cevent c( &event, spaceIsLanguage, true);
					handleEvent( c);
				}
				break;
			}
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
		case SDL_BUTTON_WHEELUP:
			type =EVENT_WHEEL_UP;
			break;
		case SDL_BUTTON_WHEELDOWN:
			type =EVENT_WHEEL_DOWN;
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
		case SDL_BUTTON_WHEELUP:
		case SDL_BUTTON_WHEELDOWN:
		case SDL_BUTTON_RIGHT:
		case SDL_BUTTON_MIDDLE: // not used.
			type =EVENT_INVALID;
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

/** Handle event after the dialog wants a new event. Lock the instance on m_events.
 *  post: All events in the queue are handled.
 *  @param event [in] What event to handle.
 */
void CdialogEvent::handleEvent( Cevent &event)
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
		handleMousePress( event.point);
		break;

	case EVENT_TOUCH_RELEASE:
		handleMouseRelease( event.point);
		break;

	case EVENT_TOUCH_MOVE:
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

/** @brief Handle when we release the mouse now.
 *  @param p [in] Where the finger/mouse has left the screen/stopped pressing.
 */
void CdialogEvent::handleMouseRelease( const Cpoint &p)
{
	keybutton key;
	CdialogObject *object;
	(void)p;
	lock();
	m_isPressed =false;
	switch (m_mouseStatus)
	{
	case MOUSE_RELEASED: // Already released?
		break;
	case MOUSE_DRAG:
		m_mouseStatus =MOUSE_RELEASED;
		m_events.push_back( Cevent( EVENT_DRAG_STOP, p, false));
		break;

	case MOUSE_START_DRAG: // Stop drag, send button to press object.
	 	object =m_interface->findObject( p);
		if ( object ==NULL)
		{
			break;
		}
		try
		{
			m_events.push_back( Cevent( EVENT_MOUSE_CLICK, object->m_code, p, false));
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
			m_events.push_back( Cevent( EVENT_MOUSE_CLICK, key, m_scrollStart, false));
		}
		break;

	case MOUSE_SCROLL:
		m_events.push_back( Cevent( EVENT_TOUCH_RELEASE, m_scrollStart, false));
		break;

	case MOUSE_PRESS:
	case MOUSE_DRAG_DIALOG:
		break;
	}
	m_mouseStatus =MOUSE_RELEASED;
	unlock();
}

/** Handle mouse event when the user requests an event.
 *  @param p [in] Location where we move the mouse to
 */
void CdialogEvent::handleMouseMove( const Cpoint &p)
{
	lock();
	switch (m_mouseStatus)
	{
	case MOUSE_RELEASED: ///< No activity.
		break;
	case MOUSE_START_DRAG: ///< Wait a second before starting to drag.
		if ( m_press.distance( m_lastMouse)>m_dragStartPixels)
		{
			m_mouseStatus =MOUSE_DRAG;
			m_events.push_back( Cevent( EVENT_DRAG_START, m_press, false));
			m_events.push_back( Cevent( EVENT_DRAG_MOVE, p, false));
			// Tell where we go to the scroll dialog.
			//if ( m_interface)
			//{
			//	m_interface->dragObject( m_dragObject, p);
			//}
		}
		break;
	case MOUSE_START_SCROLL_OR_DRAG: ///< Start a scroll dialog.
		if ( m_interface->isScrollDragDialog(p/8))
		{
			// Check if it's horizontal or vertical.
			int dist_slider, dist_object;
			if ( m_interface->isHorizontalScrollDialog(p/8))
			{
				dist_slider =m_scrollStart.horizontalDistance( m_lastMouse);
				dist_object =m_scrollStart.verticalDistance( m_lastMouse);
			}
			else
			{
				dist_object =m_scrollStart.horizontalDistance( m_lastMouse);
				dist_slider =m_scrollStart.verticalDistance( m_lastMouse);
			}
			if ( dist_object*dist_object >m_dragStartPixels)
			{
				m_mouseStatus =MOUSE_DRAG;
				m_events.push_back( Cevent( EVENT_DRAG_START, m_press, false));
				m_events.push_back( Cevent( EVENT_DRAG_MOVE, p, false));
			}
			else if ( dist_slider*dist_slider >m_dragStartPixels)
			{
				m_mouseStatus =MOUSE_SCROLL;
				// Tell where we go to the scroll dialog.
				if ( m_interface)
				{
					m_interface->scrollDialog( m_scrollStart, m_scrollStart-p);
				}
			}
		}
		else if ( m_scrollStart.distance( m_lastMouse)>m_dragStartPixels)
		{
			m_mouseStatus =MOUSE_SCROLL;
			// Tell where we go to the scroll dialog.
			if ( m_interface)
			{
				m_interface->scrollDialog( m_scrollStart, m_scrollStart-p);
			}
		}
		break;
	case MOUSE_SCROLL: ///< Scrolling window.
		// Tell where we go to the scroll dialog.
		m_interface->scrollDialog( m_scrollStart, m_lastMouse-p);
		break;
	case MOUSE_DRAG: ///< Busy dragging object.
		m_events.push_back( Cevent( EVENT_DRAG_MOVE, p, false));
		//m_mouseStatus =MOUSE_RELEASED;
		//m_events.push( Cevent( EVENT_DRAG_STOP, p, false));
		break;
	case MOUSE_PRESS: ///< Press mouse without drag, possible repeat.
		m_events.push_back( Cevent( EVENT_MOUSE_MOVE, p, false));
		break;
	case MOUSE_DRAG_DIALOG:	///< Drag entire dialog.
		break;
	}
	m_lastMouse =p;
	unlock();
}

/** @brief New mouse press. Run from any thread.
 *  @param p [in] Location mouse press.
 */
void CdialogEvent::handleMousePress( const Cpoint &p)
{
	lock();
	keybutton key;
	CdialogObject *object;
	m_mousePress.setTime( 10000000, 500,2);
	m_lastMouse =p;
	m_numberOfPresses++;
	m_press =p;
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
		isScrollDialog =m_interface->isSwypeDialog( p);
	}
	catch (...)
	{
		// Not a valid scroll dialog anymore.
	}
	if ( isScrollDialog)
	{
		// Yes, we have a scroll dialog.
		m_scrollStart =p;
		m_mouseStatus =MOUSE_START_SCROLL_OR_DRAG;
		m_mousePress.setTime( m_minimumDragTime, 100,2);
		m_events.push_back( Cevent( EVENT_TOUCH_PRESS, p, false));
		unlock();
		return;
	}

	// Check which button pressed on which window.
	object =NULL;
	try
	{
		m_interface->notifyTouch( p);
	}
	catch (...)
	{
		object =NULL;
	}
	if ( object)
	{
		// Drag object or not.
		if ( object->m_dragEnable)
		{
			m_mouseStatus =MOUSE_START_DRAG;
			m_mousePress.setTime( m_minimumDragTime, 100,2);
			m_events.push_back( Cevent( EVENT_TOUCH_PRESS, p, false));
		}
	}
	else
	{
		try
		{
			key =m_interface->findButton( p);
		}
		catch (...)
		{
			key =KEY_NONE;
		}
	}
	bool spaceIsLanguage =m_interface ? m_interface->spaceIsLanguage():true;

	if ( key==KEY_SPACE && spaceIsLanguage)
	{
		m_events.push_back( Cevent( EVENT_LANGUAGE_CHANGE, Cpoint(0,0),false));
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
		m_mouseStatus =MOUSE_PRESS;
		m_mousePress.stop();
	}
	// Single button press.
	m_events.push_back( Cevent( key, KMOD_NONE, false));
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
void CdialogEvent::handleKeyPress( SDLMod mod, keybutton key)
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
	if ( m_events.empty() ==true)
	{
		SDL_Event event;
		if ( Cgraphics::m_defaults.get_test_event &&
			 Cgraphics::m_defaults.get_test_event( event) ==true)
		{
			//Log.enableAll();
			bool spaceIsLanguage =m_interface ? m_interface->spaceIsLanguage():true;
			Cevent c( &event, spaceIsLanguage, true);
			// CdialogEvent::work  test event
			m_keyFile.onEvent( &event);
			unlock();
			//Ctimestamp::incrementSimulationTime();
			handleEvent( c);
			return c.status;
		}
		return POLL_EMPTY;
	}
	if ( callback ==NULL)
	{
		return POLL_EMPTY;
	}
	Cevent event =m_events.front();
	m_events.pop_front();
	callback->onEvent( event);
	return event.status;
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
		xx =RANDOM(SQUARES_WIDTH);
		yy =RANDOM(SQUARES_HEIGHT);
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
		event->motion.x =RANDOM( SQUARES_WIDTH*8);
		event->motion.y =RANDOM( SQUARES_HEIGHT*8);
		x=RANDOM(10);
		break;

	case 2: //Mouse release
		event->type =SDL_MOUSEBUTTONUP;
		event->motion.x =RANDOM( SQUARES_WIDTH*8);
		event->motion.y =RANDOM( SQUARES_HEIGHT*8);
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
		event->motion.x =RANDOM( SQUARES_WIDTH*8);
		event->motion.y =RANDOM( SQUARES_HEIGHT*8);
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
	m_mouseStatus =MOUSE_RELEASED;
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
