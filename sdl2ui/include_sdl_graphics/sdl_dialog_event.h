/*============================================================================*/
/**  @file       sdl_dialog_event.h
 **  @ingroup    zhongcan_user_interface
 **  @brief		 Default dialog.
 **
 **  Create an event for the dialogs.
 **
 **  @author     mensfort
 **
 **  @par Classes:
 **              Cevent
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
#include <SDL.h>
#include <atomic>
#include <pthread.h>
#include <queue>
#include <vector>
#include "singleton.hpp"

#include "sdl_font.h"
#include "sdl_rect.h"
#include "my_thread.hpp"
#include "timeout.hpp"
#include "sdl_key_file.h"
#include "sdl_dialog_object.h"

/// @brief Status for message when sent to any client.
typedef enum
{
	DIALOG_EVENT_PROCESSED,		///< Event processed, stop sending.
	DIALOG_EVENT_OPEN,			///< Event still open.
	DIALOG_EVENT_GENERAL,		///< Dialog can be sent to all.
	DIALOG_EVENT_EXIT			///< Dialog request to stop the dialog.
} Estatus;

/// @brief Status for the mouse.
typedef enum
{
	MOUSE_RELEASED,				///< No activity.
	MOUSE_RELEASE_DEBOUNCE,		///< Releasing the mouse.
	MOUSE_START_DRAG,			///< Wait a second before starting to drag.
	MOUSE_START_SCROLL_OR_DRAG,	///< Start a scroll dialog.
	MOUSE_SCROLL,				///< Scrolling window.
	MOUSE_PAINT,				///< Painting with finger on object
	MOUSE_DRAG,					///< Busy dragging.
	MOUSE_PRESS_DEBOUNCE,		///< Debounce mouse
	MOUSE_PRESS,				///< Press mouse without drag, possible repeat.
	MOUSE_DRAG_DIALOG			///< Drag entire dialog.
} EmouseStatus;

/// @brief Status after polling.
typedef enum
{
	POLL_EMPTY,
	POLL_USER,
	POLL_TESTING
} EpollStatus;

typedef enum
{
	EVENT_TOUCH_PRESS,		///< Output event.
	EVENT_TOUCH_RELEASE,	///< Output event.
	EVENT_TOUCH_MOVE,   	///< Output event.
	EVENT_TOUCH_LONG,		///< Output event.
	EVENT_KEY_PRESS,		///< Input/Output event.
	EVENT_KEY_RELEASE,		///< Input event.
	EVENT_DRAG_START,		///< Input event.
	EVENT_PAINT_START,		///< Start painting.
	EVENT_DRAG_MOVE,		///< Output event.
	EVENT_PAINT_MOVE,		///< Move finger while painting.
	EVENT_MOUSE_MOVE,		////< Just move the mouse when pressed.
	EVENT_DRAG_STOP,		///< Input event.
	EVENT_PAINT_STOP,		///< Stop painting.
	EVENT_APPMOUSEFOCUS,	///< Input/Output event.
	EVENT_APPMOUSEBLUR, 	///< Input/Output event.
	EVENT_APPINPUTFOCUS,	///< Input/Output event.
	EVENT_APPINPUTBLUR, 	///< Input/Output event.
	EVENT_RESTORE,      	///< Input/Output event.
	EVENT_MINIMIZE,     	///< Input/Output event.
	EVENT_LANGUAGE_CHANGE,  ///< Output event.
	EVENT_MAIN_DIALOG,		///< Output event.
	EVENT_BUTTON,			///< Output event.
	EVENT_MOUSE_CLICK,		///< Short click for a button.
	EVENT_WHEEL_DOWN,		///< Wheel rotate (mouse).
	EVENT_WHEEL_UP,			///< Wheel rotate (mouse).
	EVENT_BARCODE,          ///< Barcode event.
	EVENT_QUIT,				///< Quit application.
	EVENT_INVALID,
} EventType;

/// @brief Definition of an event.
class Cevent
{
public:
	Cevent( SDL_Event *event, bool spaceIsLanguage, bool test);
	Cevent( EventType t, keybutton key, const Cpoint &p, bool testing)
	: type( t)
	, status( testing ? POLL_TESTING:POLL_USER)
	, point( p)
	, button( key)
	, mod( KMOD_NONE)
	, which( 0)
	{
	}
	Cevent( keybutton key, SDLMod m, bool testing=false)
	: type( EVENT_KEY_PRESS)
	, status( testing ? POLL_TESTING:POLL_USER)
	, point( 0,0)
	, button( key)
	, mod( m)
	, which( 0)
	{
	}
	Cevent( EventType t, const Cpoint &p, bool testing)
	: type(t)
	, status( testing ? POLL_TESTING:POLL_USER)
	, point( p)
	, button( KEY_NONE)
	, mod( KMOD_NONE)
	, which( 0)
	{
	}
	Cevent( const std::string &barcode, bool testing=false)
	: type(EVENT_BARCODE)
	, status( testing ? POLL_TESTING:POLL_USER)
	, point( Cpoint(0,0))
	, button( KEY_NONE)
	, mod( KMOD_NONE)
	, barcode(barcode)
	, which( 0)
	{
	}
	~Cevent() {}
	std::string toString();

public:
	EventType	type;
	EpollStatus status;
	Cpoint		point;
	keybutton   button;
	SDLMod      mod;
	std::string barcode;
	int			which;
};

/// @brief Queue for events.
class CeventQueue : private CmyLock
{
public:
	CeventQueue();
	~CeventQueue();
	void push_back( const Cevent &event);
	Cevent front();
	void pop_front();
	int size();
	bool empty();

private:
	std::atomic<int> m_size;
	std::queue<Cevent> m_eventList; ///< All events in queue.
};

/// @brief Interface for keyboard/mouse events.
class CeventInterface
{
public:
	CeventInterface() {}
	virtual ~CeventInterface() {}

public:
	virtual void lockEvent() =0;
	virtual void unlockEvent() =0;
	virtual Estatus onEvent( const Cevent &event) =0;
	virtual void onStartDrag( CdialogObjectPtr movingObject) =0; // reentrant
	virtual void onStopDrag() =0; // reentrant
	void addDialog( CeventInterface	*interface); // reentrant
	void removeDialog( CeventInterface	*interface);
	void stopDialog( CeventInterface *interface);
	virtual CdialogObjectPtr findObject( const Cpoint &p) =0;
	virtual keybutton findButton( const Cpoint &p) =0;
	virtual bool isSwypeDialog( const Cpoint &p) =0;
	virtual void onMouseWheel(bool Up, bool Down) =0;
	virtual void scrollDialog( const Cpoint &start, const Cpoint &distance) =0;
	virtual void dragObject( CdialogObjectPtr movingObject, const Cpoint &to) =0;
	virtual std::string getName() =0;
	virtual bool spaceIsLanguage() =0;
	virtual bool isScrollDragDialog( const Cpoint &p) =0;
	virtual bool isHorizontalScrollDialog( const Cpoint &p) =0;
	virtual void notifyTouch( const Cpoint &p)=0;
    virtual void onBarcode( const std::string &barcode) =0;
};

/// @brief Create events for a dialog on screen.
class CdialogEvent : public Tsingleton<CdialogEvent>, public CmyThread
{
	friend class Tsingleton<CdialogEvent>;

public:
	CdialogEvent();
	virtual ~CdialogEvent();
	EpollStatus pollEvent( CeventInterface *callback);
	virtual void stop();
	virtual void work();
	void stopDrag();
	Cpoint lastMouse() const;
	bool isMousePressed() const;
	EmouseStatus getStatus() const;
	void startIdiotTesting();
	void registerActiveDialog( CeventInterface *interface);
    void forceDrag( const Cpoint &dragPoint);
	CeventInterface *getInterface();
    void handleBarcode( const std::string &barcode);
	///< Test interface
    Cpoint getMousePosition() { return m_lastMousePos; }
	void setTouchBottom(int bottom);

private:
	// Raw events
	void stateMachine();
	void createRandomEvent();
	void logState();
	void handleRawEvent( Cevent &event);
  	  void handleRawKeyPress( SDLMod mod, keybutton key);
	  void handleRawKeyRelease();
	  void handleRawMousePress( const Cpoint &p);
	  void handleRawMouseRelease( const Cpoint &p);
	  void handleRawMouseMove( const Cpoint &p);
	void flushEvents();
	// Finall events
	void handlePostEvent( CeventInterface *callback, const Cevent &event);
  	  void handlePostMousePress( CeventInterface *callback, const Cevent &c);
	  void handlePostMouseRelease( CeventInterface *callback, const Cevent &c);
	  void handlePostMouseLong( CeventInterface *callback, const Cevent &c);
	  void handlePostMouseMove( CeventInterface *callback, const Cevent &c);
	void updateLastMouse( const SDL_Event &event);

private:
	CeventInterface		*m_interface;		///< Which interface is active.
	EmouseStatus		m_inputStat;		///< Mouse input status.
	EmouseStatus  		m_mouseStat; 		///< Mouse status.
	int					m_minimumDragTime; 	///< Minimum time to start drag.
	int					m_minimumClickTime; ///< Minimum time to send click.
	int					m_debounceDist;		///< Distance debounce.
	int					m_debounceTime;		///< Delay for debounce.
	int					m_numberOfPresses;	///< Number of press for button.
	Cpoint				m_lastMousePos;		///< Last mouse position.
	Cpoint				m_inputMousePos; 	///< Input mouse position.
	int					m_repeatDelay;		///< Delay for repeat.
	int					m_repeatSpeed;		///< Speed for repeat.
	CeventQueue			m_events;			///< Events to send.
	int					m_repeatCount;		///< Time to repeat.
	Ctimeout 			m_keyPress;			///< Time to repeat.
	Ctimeout			m_mousePress;		///< Time to press mouse.
	Ctimeout 			m_mouseDebounce;	///< Time for mouse debounce.
	Ctimeout 			m_mouseLongDebounce;///< Time for mouse debounce.
	keybutton			m_repeatButton;		///< Object event may be repeated.
	Ctimeout			m_repeatTimer; 		///< Timeout until next key press
	SDL_Event 			m_event;			///< New SDL event coming in...
	bool				m_mousePressedLeft; ///< Press mouse.
	Cpoint				m_scrollStart;		///< Start dragging in 800x600 res.
	Cpoint				m_dragPoint;		///< Dragging x-coordinate in 800x600 res.
	int					m_dragStartPixels;	///< Minimum distance to start drag.
	CkeyFile			m_keyFile;			///< Test class to remember all buttons today.
	Cpoint				m_press;			///< Where we press the screen.
	bool				m_isPressed;		///< Is the mouse pressed?
	bool				m_longDebounced;	///< Long debounce time.
    int                 m_touchBottom;      ///< Bottom of touch screen
public:
	bool				m_idiot;			///< Idiot test.
	Cpoint				m_debugPosition;	///< Position to debug
};


